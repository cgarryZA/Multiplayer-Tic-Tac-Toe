#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <atomic>
#include <algorithm>
#include "board.h"
#include "net.h"

// ====== helpers from your earlier code ======

char checkWinner(const Board& b)
{
    std::size_t size = b.getSize();

    // rows
    for (std::size_t y = 0; y < size; ++y)
    {
        char first = b.get(0, y);
        if (first == ' ') continue;
        bool allSame = true;
        for (std::size_t x = 1; x < size; ++x)
            if (b.get(x, y) != first) { allSame = false; break; }
        if (allSame) return first;
    }

    // columns
    for (std::size_t x = 0; x < size; ++x)
    {
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
        if (first != ' ')
        {
            bool allSame = true;
            for (std::size_t i = 1; i < size; ++i)
                if (b.get(i, i) != first) { allSame = false; break; }
            if (allSame) return first;
        }
    }

    // anti diag
    {
        char first = b.get(size - 1, 0);
        if (first != ' ')
        {
            bool allSame = true;
            for (std::size_t i = 1; i < size; ++i)
                if (b.get(size - 1 - i, i) != first) { allSame = false; break; }
            if (allSame) return first;
        }
    }

    return ' ';
}

// generate a list of symbols: X, O, then ASCII that is not awful
std::vector<char> generateSymbols(std::size_t count)
{
    std::vector<char> result;
    if (count == 0) return result;

    result.push_back('X');
    if (count == 1) return result;
    result.push_back('O');

    std::vector<char> pool;
    for (int c = 33; c <= 126; ++c)
    {
        char ch = static_cast<char>(c);
        if (ch == '|' || ch == '+' || ch == '-' || ch == ' ') continue;
        if (ch == 'X' || ch == 'O') continue;
        pool.push_back(ch);
    }

    // just take from the pool in order
    for (std::size_t i = 0; i + 2 < count && i < pool.size(); ++i)
        result.push_back(pool[i]);

    return result;
}

// broadcast helper
void broadcast(const std::vector<SOCKET>& clients, const std::string& line)
{
    for (SOCKET s : clients)
    {
        send_line(s, line);
    }
}

int main()
{
    if (!net_init()) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET listenSock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (listenSock == INVALID_SOCKET) {
        std::cerr << "socket failed\n";
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    addr.sin_addr.s_addr = INADDR_ANY;

    if (bind(listenSock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "bind failed\n";
        closesocket(listenSock);
        return 1;
    }

    if (listen(listenSock, SOMAXCONN) == SOCKET_ERROR) {
        std::cerr << "listen failed\n";
        closesocket(listenSock);
        return 1;
    }

    std::cout << "Server listening on port 5000.\n";

    // lobby state
    std::vector<SOCKET> clients;
    std::mutex clientsMutex;
    std::atomic<bool> startGame{false};
    std::atomic<bool> stopAccept{false};

    // accept thread: keeps accepting until we say stop
    std::thread acceptThread([&](){
        while (!stopAccept.load())
        {
            SOCKET cs = accept(listenSock, nullptr, nullptr);
            if (cs == INVALID_SOCKET) {
                // could be interrupted, just continue
                continue;
            }
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                clients.push_back(cs);
                std::cout << "Client connected. Total: " << clients.size() << "\n";
            }
        }
    });

    // main loop: lobby → game → lobby ...
    while (true)
    {
        // LOBBY PHASE
        std::cout << "Lobby: type 'start' to begin with current players (" 
                  << "[host] + " << clients.size() << " remote)." << std::endl;
        std::cout << "You can just wait here while people connect." << std::endl;

        std::string cmd;
        std::getline(std::cin, cmd);
        if (!std::cin) break;
        if (cmd == "start")
        {
            // stop accepting new players
            stopAccept.store(true);
            // wait for the accept thread to finish
            if (acceptThread.joinable())
                acceptThread.join();

            // snapshot client list
            std::vector<SOCKET> gameClients;
            {
                std::lock_guard<std::mutex> lock(clientsMutex);
                gameClients = clients;
            }

            // number of players = server + remote clients
            std::size_t numPlayers = 1 + gameClients.size();
            std::vector<char> symbols = generateSymbols(numPlayers);
            std::size_t boardSize = numPlayers + 1;

            // tell every client their START info
            for (std::size_t i = 0; i < gameClients.size(); ++i)
            {
                char mySym = symbols[i + 1]; // 0 = server
                send_line(gameClients[i], "START " + std::to_string(boardSize) + " " + std::string(1, mySym));
            }

            // now run the game
            Board board(boardSize);
            bool gameOver = false;
            std::size_t currentPlayer = 0; // 0 = server

            // simple scoreboard just for this session
            std::vector<int> wins(numPlayers, 0);
            int draws = 0;

            while (!gameOver)
            {
                board.print();
                char currentSymbol = symbols[currentPlayer];

                if (currentPlayer == 0)
                {
                    // server's turn (read from console)
                    std::cout << "Your move (x y): ";
                    int x, y;
                    if (!(std::cin >> x >> y)) {
                        std::cout << "Input ended.\n";
                        gameOver = true;
                        break;
                    }
                    // eat leftover newline for next getline later
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                    if (x < 0 || y < 0 || x >= (int)boardSize || y >= (int)boardSize)
                    {
                        std::cout << "Invalid move, try again.\n";
                        continue;
                    }
                    if (board.get(x, y) != ' ')
                    {
                        std::cout << "That cell is taken.\n";
                        continue;
                    }

                    board.set(x, y, currentSymbol);

                    // broadcast to all clients: MOVE x y S
                    broadcast(gameClients, "MOVE " + std::to_string(x) + " " + std::to_string(y) + " " + currentSymbol);
                }
                else
                {
                    // remote player's turn
                    SOCKET theirSock = gameClients[currentPlayer - 1];

                    // tell only that client it's their turn
                    send_line(theirSock, "YOUR_TURN");

                    // receive their move
                    std::string line;
                    if (!recv_line(theirSock, line)) {
                        std::cout << "Player " << currentPlayer << " disconnected.\n";
                        gameOver = true;
                        break;
                    }

                    int x, y;
                    if (sscanf_s(line.c_str(), "MOVE %d %d", &x, &y) != 2)
                    {
                        std::cout << "Bad move from client.\n";
                        gameOver = true;
                        break;
                    }

                    // apply
                    if (x < 0 || y < 0 || x >= (int)boardSize || y >= (int)boardSize || board.get(x, y) != ' ')
                    {
                        std::cout << "Client sent invalid move.\n";
                        gameOver = true;
                        break;
                    }
                    board.set(x, y, currentSymbol);

                    // broadcast to others (including that client for consistency)
                    broadcast(gameClients, "MOVE " + std::to_string(x) + " " + std::to_string(y) + " " + currentSymbol);
                }

                // check end
                char w = checkWinner(board);
                if (w != ' ')
                {
                    // tell everyone
                    std::size_t winnerIndex = std::distance(symbols.begin(),
                                          std::find(symbols.begin(), symbols.end(), w));
                    if (winnerIndex < wins.size())
                        wins[winnerIndex]++;

                    board.print();
                    std::cout << "Player " << w << " wins!\n";

                    broadcast(gameClients, std::string("RESULT WIN ") + w);
                    broadcast(gameClients, "RESET");

                    // print server-side scoreboard
                    std::cout << "Scoreboard:\n";
                    for (std::size_t i = 0; i < symbols.size(); ++i)
                    {
                        std::cout << "  Player " << symbols[i] << ": " << wins[i] << "\n";
                    }
                    std::cout << "  Draws: " << draws << "\n";

                    gameOver = true;
                    break;
                }

                if (board.isFull())
                {
                    draws++;
                    board.print();
                    std::cout << "It's a draw.\n";

                    broadcast(gameClients, "RESULT DRAW");
                    broadcast(gameClients, "RESET");

                    std::cout << "Scoreboard:\n";
                    for (std::size_t i = 0; i < symbols.size(); ++i)
                    {
                        std::cout << "  Player " << symbols[i] << ": " << wins[i] << "\n";
                    }
                    std::cout << "  Draws: " << draws << "\n";

                    gameOver = true;
                    break;
                }

                // next player
                currentPlayer = (currentPlayer + 1) % numPlayers;
            }

            // after a game, go back to lobby mode
            // re-enable accepting
            stopAccept.store(false);
            acceptThread = std::thread([&](){
                while (!stopAccept.load())
                {
                    SOCKET cs = accept(listenSock, nullptr, nullptr);
                    if (cs == INVALID_SOCKET) {
                        continue;
                    }
                    {
                        std::lock_guard<std::mutex> lock(clientsMutex);
                        clients.push_back(cs);
                        std::cout << "Client connected. Total: " << clients.size() << "\n";
                    }
                }
            });

            // and loop to lobby again
        }
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
