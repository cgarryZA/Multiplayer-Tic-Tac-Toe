#include <iostream>
#include <string>
#include "board.h"
#include "net.h"

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

    std::cout << "Connected to server.\n";

    // scoreboard (client is O)
    int myWins = 0;
    int oppWins = 0;
    int draws = 0;

    // outer loop: many games
    while (true)
    {
        Board board(3);
        char mySymbol = 'O';
        char theirSymbol = 'X';
        bool myTurn = false; // client goes second

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
                    goto end;
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
                send_line(sock, "MOVE " + std::to_string(x) + " " + std::to_string(y));

                // We don't decide winner here; we wait for server's RESULT
                myTurn = false;
            }
            else
            {
                std::string line;
                if (!recv_line(sock, line)) {
                    std::cout << "Server disconnected.\n";
                    goto end;
                }

                // Could be MOVE ... or RESULT ...
                if (line.rfind("MOVE", 0) == 0)
                {
                    int x, y;
                    if (sscanf_s(line.c_str(), "MOVE %d %d", &x, &y) == 2)
                    {
                        board.set(x, y, theirSymbol);
                        myTurn = true;
                    }
                }
                else if (line.rfind("RESULT", 0) == 0)
                {
                    // server will send RESULT WIN X / RESULT WIN O / RESULT DRAW
                    if (line == "RESULT DRAW")
                    {
                        draws++;
                        board.print();
                        std::cout << "It's a draw.\n";
                    }
                    else
                    {
                        // parse winner
                        char winner = ' ';
                        if (sscanf_s(line.c_str(), "RESULT WIN %c", &winner, 1) == 1)
                        {
                            board.print();
                            std::cout << "Player " << winner << " wins!\n";
                            if (winner == mySymbol) myWins++;
                            else oppWins++;
                        }
                    }

                    // print scoreboard
                    std::cout << "Score: Me=" << myWins
                              << " Opp=" << oppWins
                              << " Draws=" << draws << "\n";

                    // after RESULT we expect RESET
                    std::string resetLine;
                    if (!recv_line(sock, resetLine)) {
                        std::cout << "Server disconnected.\n";
                        goto end;
                    }
                    // resetLine should be "RESET"
                    break; // break inner, start new game
                }
            }
        }
        // and outer while goes again
    }

end:
    closesocket(sock);
    net_cleanup();
    return 0;
}
