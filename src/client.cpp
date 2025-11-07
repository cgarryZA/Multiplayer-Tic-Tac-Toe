#include <iostream>
#include <string>
#include <limits>
#include "board.h"
#include "net.h"

int main(int argc, char* argv[])
{
    if (argc < 2) {
        std::cout << "Usage: ttt_client <server-ip>\n";
        return 0;
    }

    if (!net_init()) {
        std::cerr << "WSAStartup failed\n";
        return 1;
    }

    SOCKET sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if (sock == INVALID_SOCKET) {
        std::cerr << "socket failed\n";
        return 1;
    }

    sockaddr_in addr{};
    addr.sin_family = AF_INET;
    addr.sin_port = htons(5000);
    inet_pton(AF_INET, argv[1], &addr.sin_addr);

    if (connect(sock, (sockaddr*)&addr, sizeof(addr)) == SOCKET_ERROR) {
        std::cerr << "connect failed\n";
        closesocket(sock);
        return 1;
    }

    std::cout << "Connected. Waiting for START...\n";

    int myWins = 0;
    int draws = 0;
    int losses = 0;

    while (true)
    {
        // wait for START
        std::string line;
        if (!recv_line(sock, line)) {
            std::cout << "Server disconnected.\n";
            break;
        }

        if (line.rfind("START", 0) != 0) {
            std::cout << "Unexpected message: " << line << "\n";
            continue;
        }

        int boardSize = 3;
        char mySymbol = 'O';
        {
            int tmpSize = 0;
            char tmpSym = 'O';
            if (sscanf_s(line.c_str(), "START %d %c", &tmpSize, &tmpSym, 1) == 2) {
                boardSize = tmpSize;
                mySymbol = tmpSym;
            }
        }

        Board board(boardSize);
        std::cout << "Game starting. You are '" << mySymbol << "'. Board " << boardSize << "x" << boardSize << "\n";

        bool inGame = true;
        while (inGame)
        {
            board.print();
            std::string msg;
            if (!recv_line(sock, msg)) {
                std::cout << "Server disconnected.\n";
                goto end;
            }

            if (msg == "YOUR_TURN")
            {
                // ask user
                board.print();
                std::cout << "Your move (x y): ";
                int x, y;
                if (!(std::cin >> x >> y)) {
                    std::cout << "Input ended.\n";
                    goto end;
                }
                // eat trailing newline
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                send_line(sock, "MOVE " + std::to_string(x) + " " + std::to_string(y));
            }
            else if (msg.rfind("MOVE", 0) == 0)
            {
                int x, y;
                char sym;
                // may be: MOVE x y S
                if (sscanf_s(msg.c_str(), "MOVE %d %d %c", &x, &y, &sym, 1) == 3) {
                    board.set(x, y, sym);
                } else if (sscanf_s(msg.c_str(), "MOVE %d %d", &x, &y) == 2) {
                    // fallback: if server didn’t send symbol, use ?
                    // but our server will send symbol
                }
            }
            else if (msg.rfind("RESULT", 0) == 0)
            {
                if (msg == "RESULT DRAW") {
                    board.print();
                    std::cout << "It's a draw.\n";
                    draws++;
                } else {
                    char w;
                    if (sscanf_s(msg.c_str(), "RESULT WIN %c", &w, 1) == 1) {
                        board.print();
                        std::cout << "Player " << w << " wins!\n";
                        if (w == mySymbol) myWins++;
                        else losses++;
                    }
                }

                // show my scoreboard
                std::cout << "My score: Wins=" << myWins
                          << " Losses=" << losses
                          << " Draws=" << draws << "\n";
            }
            else if (msg == "RESET")
            {
                // back to waiting for START
                inGame = false;
            }
            else
            {
                // ignore unknown
            }
        }

        // loop: wait for next START
    }

end:
    closesocket(sock);
    net_cleanup();
    return 0;
}
