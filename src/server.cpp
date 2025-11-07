#include <iostream>
#include <string>
#include "board.h"
#include "net.h"

// copy the winner/draw logic so we don't touch game.cpp
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

    // cols
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

    if (listen(listenSock, 1) == SOCKET_ERROR) {
        std::cerr << "listen failed\n";
        closesocket(listenSock);
        return 1;
    }

    std::cout << "Server: waiting on port 5000...\n";
    SOCKET clientSock = accept(listenSock, nullptr, nullptr);
    if (clientSock == INVALID_SOCKET) {
        std::cerr << "accept failed\n";
        closesocket(listenSock);
        return 1;
    }
    std::cout << "Client connected.\n";
    closesocket(listenSock);

    // game state
    Board board(3);
    char mySymbol = 'X';
    char theirSymbol = 'O';
    bool myTurn = true;

    while (true)
    {
        board.print();

        if (myTurn)
        {
            std::cout << "Your move (x y): ";
            int x, y;
            if (!(std::cin >> x >> y)) {
                std::cout << "Input ended.\n";
                break;
            }

            if (x < 0 || y < 0 || x >= (int)board.getSize() || y >= (int)board.getSize())
            {
                std::cout << "Invalid move, try again.\n";
                continue;
            }
            if (board.get(x, y) != ' ')
            {
                std::cout << "That cell is taken.\n";
                continue;
            }

            board.set(x, y, mySymbol);

            // tell client
            send_line(clientSock, "MOVE " + std::to_string(x) + " " + std::to_string(y));

            // check end
            char w = checkWinner(board);
            if (w != ' ')
            {
                board.print();
                std::cout << "Player " << w << " wins!\n";
                break;
            }
            if (board.isFull())
            {
                board.print();
                std::cout << "It's a draw.\n";
                break;
            }

            myTurn = false;
        }
        else
        {
            std::cout << "Waiting for opponent...\n";
            std::string line;
            if (!recv_line(clientSock, line)) {
                std::cout << "Client disconnected.\n";
                break;
            }

            int x, y;
            if (sscanf_s(line.c_str(), "MOVE %d %d", &x, &y) == 2)
            {
                board.set(x, y, theirSymbol);

                char w = checkWinner(board);
                if (w != ' ')
                {
                    board.print();
                    std::cout << "Player " << w << " wins!\n";
                    break;
                }
                if (board.isFull())
                {
                    board.print();
                    std::cout << "It's a draw.\n";
                    break;
                }

                myTurn = true;
            }
        }
    }

    closesocket(clientSock);
    net_cleanup();
    return 0;
}
