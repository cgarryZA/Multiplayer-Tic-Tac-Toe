#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>
#include <limits>
#include "board.h"
#include "net.h"

// ---------- win check ----------
char checkWinner(const Board& b)
{
    std::size_t size = b.getSize();

    // rows
    for (std::size_t y = 0; y < size; ++y) {
        char first = b.get(0, y);
        if (first == ' ') continue;
        bool allSame = true;
        for (std::size_t x = 1; x < size; ++x)
            if (b.get(x, y) != first) { allSame = false; break; }
        if (allSame) return first;
    }

    // columns
    for (std::size_t x = 0; x < size; ++x) {
        char first = b.get(x, 0);
        if (first == ' ') continue;
        bool allSame = true;
        for (std::size_t y = 1; y < size; ++y)
            if (b.get(x, y) != first) { allSame = false; break; }
        if (allSame) return first;
    }

    // main diag
    {
        char first = b.get(0, 0);
        if (first != ' ') {
            bool allSame = true;
            for (std::size_t i = 1; i < size; ++i)
                if (b.get(i, i) != first) { allSame = false; break; }
            if (allSame) return first;
        }
    }

    // anti diag
    {
        char first = b.get(size - 1, 0);
        if (first != ' ') {
            bool allSame = true;
            for (std::size_t i = 1; i < size; ++i)
                if (b.get(size - 1 - i, i) != first) { allSame = false; break; }
            if (allSame) return first;
        }
    }

    return ' ';
}

// ---------- symbols ----------
std::vector<char> generateSymbols(std::size_t count)
{
    std::vector<char> result;
    if (count == 0) return result;

    result.push_back('X');               // host
    if (count == 1) return result;
    result.push_back('O');               // first client

    std::vector<char> pool;
    for (int c = 33; c <= 126; ++c) {
        char ch = static_cast<char>(c);
        if (ch == '|' || ch == '+' || ch == '-' || ch == ' ') continue;
        if (ch == 'X' || ch == 'O') continue;
        pool.push_back(ch);
    }
    for (std::size_t i = 0; i + 2 < count && i < pool.size(); ++i)
        result.push_back(pool[i]);

    return result;
}

void broadcast(const std::vector<SOCKET>& clients, const std::string& line)
{
    for (SOCKET s : clients)
        send_line(s, line);
}

// helper to create a listening socket
SOCKET make_listen_socket()
{
    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET)
        return INVALID_SOCKET;

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port   = htons(5000);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        closesocket(listenSock);
        return INVALID_SOCKET;
    }

    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        closesocket(listenSock);
        return INVALID_SOCKET;
    }

    return listenSock;
}

int main()
{
    if (!net_init()) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET listenSock = make_listen_socket();
    if (listenSock == INVALID_SOCKET) {
        std::cerr << "failed to listen on 5000\n";
        return 1;
    }

    std::cout << "Server listening on port 5000.\n";

    std::vector<SOCKET> clients;
    std::mutex          clientsMutex;
    std::atomic<bool>   stopAccept{false};

    // accept thread lambda
    auto start_accepting = [&](SOCKET ls) {
        return std::thread([&]() {
            while (!stopAccept.load()) {
                SOCKET cs = accept(ls, nullptr, nullptr);
                if (cs == INVALID_SOCKET) {
                    // this happens when we close(ls) to break accept
                    break;
                }
                {
                    std::lock_guard<std::mutex> lock(clientsMutex);
                    clients.push_back(cs);
                    std::cout << "[lobby] client joined. total = " << clients.size() << "\n";
                }
            }
        });
    };

    std::thread acceptThread = start_accepting(listenSock);

    while (true)
    {
        // ----- LOBBY -----
        std::cout << "\n[lobby] type 'start' to begin, or 'list' to see players.\n";

        std::string cmd;
        std::getline(std::cin, cmd);
        if (!std::cin) break;

        if (cmd.empty())
            continue;

        if (cmd == "list") {
            std::lock_guard<std::mutex> lock(clientsMutex);
            std::cout << "[lobby] host + " << clients.size() << " client(s)\n";
            continue;
        }

        if (cmd != "start") {
            std::cout << "[lobby] unknown command\n";
            continue;
        }

        // ====== START GAME ======
        // stop accepting: close the listening socket to unblock accept()
        stopAccept.store(true);
        closesocket(listenSock);           // this wakes the accept thread
        if (acceptThread.joinable())
            acceptThread.join();

        // snapshot clients
        std::vector<SOCKET> gameClients;
        {
            std::lock_guard<std::mutex> lock(clientsMutex);
            gameClients = clients;
        }

        std::size_t numPlayers = 1 + gameClients.size();
        std::vector<char> symbols = generateSymbols(numPlayers);
        std::size_t boardSize = numPlayers + 1;

        // tell clients their start info
        for (std::size_t i = 0; i < gameClients.size(); ++i) {
            char sym = symbols[i + 1]; // 0 is host
            send_line(gameClients[i],
                      "START " + std::to_string(boardSize) + " " + std::string(1, sym));
        }

        // ----- GAME PHASE -----
        Board board(boardSize);
        bool gameOver = false;
        std::size_t currentPlayer = 0;

        while (!gameOver)
        {
            board.print();
            char sym = symbols[currentPlayer];

            if (currentPlayer == 0) {
                // host turn
                std::cout << "Your move (x y): ";
                int x, y;
                if (!(std::cin >> x >> y)) {
                    std::cout << "Input ended.\n";
                    gameOver = true;
                    break;
                }
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (x < 0 || y < 0 || x >= (int)boardSize || y >= (int)boardSize) {
                    std::cout << "Invalid move.\n";
                    continue;
                }
                if (board.get(x, y) != ' ') {
                    std::cout << "That cell is taken.\n";
                    continue;
                }

                board.set(x, y, sym);
                broadcast(gameClients, "MOVE " + std::to_string(x) + " " + std::to_string(y) + " " + sym);
            }
            else {
                // remote player's turn
                SOCKET s = gameClients[currentPlayer - 1];
                send_line(s, "YOUR_TURN");

                std::string line;
                if (!recv_line(s, line)) {
                    std::cout << "player " << currentPlayer << " disconnected.\n";
                    gameOver = true;
                    break;
                }

                int x, y;
                if (sscanf_s(line.c_str(), "MOVE %d %d", &x, &y) != 2) {
                    std::cout << "bad move from client.\n";
                    gameOver = true;
                    break;
                }

                if (x < 0 || y < 0 || x >= (int)boardSize || y >= (int)boardSize || board.get(x, y) != ' ') {
                    std::cout << "client sent invalid move.\n";
                    gameOver = true;
                    break;
                }

                board.set(x, y, sym);
                broadcast(gameClients, "MOVE " + std::to_string(x) + " " + std::to_string(y) + " " + sym);
            }

            char w = checkWinner(board);
            if (w != ' ') {
                board.print();
                std::cout << "Player " << w << " wins!\n";
                broadcast(gameClients, std::string("RESULT WIN ") + w);
                broadcast(gameClients, "RESET");
                gameOver = true;
            } else if (board.isFull()) {
                board.print();
                std::cout << "It's a draw.\n";
                broadcast(gameClients, "RESULT DRAW");
                broadcast(gameClients, "RESET");
                gameOver = true;
            }

            if (!gameOver)
                currentPlayer = (currentPlayer + 1) % numPlayers;
        }

        // ----- back to lobby: recreate listen socket and accept -----
        listenSock = make_listen_socket();
        if (listenSock == INVALID_SOCKET) {
            std::cerr << "failed to re-open listen socket\n";
            break;
        }
        stopAccept.store(false);
        acceptThread = start_accepting(listenSock);
    }

    // cleanup
    stopAccept.store(true);
    if (acceptThread.joinable())
        acceptThread.join();
    {
        std::lock_guard<std::mutex> lock(clientsMutex);
        for (SOCKET s : clients) closesocket(s);
    }
    closesocket(listenSock);
    net_cleanup();
    return 0;
}
