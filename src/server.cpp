#include <iostream>
#include <string>
#include "board.h"
#include "net.h"

// same winner check as before
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

int main()
{
    if (!net_init()) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    // listen
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

    // scoreboard
    int xWins = 0;
    int oWins = 0;
    int draws = 0;

    // outer loop: play many games
    while (true)
    {
        Board board(3);
        char mySymbol = 'X';
        char theirSymbol = 'O';
        bool myTurn = true;

        // inner loop: one game
        while (true)
        {
            board.print();

            if (myTurn)
            {
                std::cout << "Your move (x y): ";
                int x, y;
                if (!(std::cin >> x >> y)) {
                    std::cout << "Input ended.\n";
                    goto end; // quit whole program
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

                char w = checkWinner(board);
                if (w != ' ')
                {
                    board.print();
                    std::cout << "Player " << w << " wins!\n";

                    // update score (server is X)
                    if (w == 'X') xWins++;
                    else if (w == 'O') oWins++;

                    // tell client
                    send_line(clientSock, std::string("RESULT WIN ") + w);
                    send_line(clientSock, "RESET");

                    std::cout << "Score: X=" << xWins << " O=" << oWins << " Draws=" << draws << "\n";
                    break; // break inner, start new game
                }
                if (board.isFull())
                {
                    board.print();
                    std::cout << "It's a draw.\n";
                    draws++;

                    send_line(clientSock, "RESULT DRAW");
                    send_line(clientSock, "RESET");

                    std::cout << "Score: X=" << xWins << " O=" << oWins << " Draws=" << draws << "\n";
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
                    goto end;
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

                        if (w == 'X') xWins++;
                        else if (w == 'O') oWins++;

                        send_line(clientSock, std::string("RESULT WIN ") + w);
                        send_line(clientSock, "RESET");

                        std::cout << "Score: X=" << xWins << " O=" << oWins << " Draws=" << draws << "\n";
                        break;
                    }
                    if (board.isFull())
                    {
                        board.print();
                        std::cout << "It's a draw.\n";
                        draws++;

                        send_line(clientSock, "RESULT DRAW");
                        send_line(clientSock, "RESET");

                        std::cout << "Score: X=" << xWins << " O=" << oWins << " Draws=" << draws << "\n";
                        break;
                    }

                    myTurn = true;
                }
            }
        }
        // then loop again: new Board(3) and go
    }

end:
    closesocket(clientSock);
    net_cleanup();
    return 0;
}
