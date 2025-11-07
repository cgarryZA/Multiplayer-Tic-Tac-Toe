#pragma once
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string>

// link winsock on MSVC; CMake will link ws2_32 anyway
#pragma comment(lib, "ws2_32.lib")

inline bool net_init()
{
    WSADATA wsa;
    return WSAStartup(MAKEWORD(2,2), &wsa) == 0;
}

inline void net_cleanup()
{
    WSACleanup();
}

inline bool send_line(SOCKET s, const std::string& line)
{
    std::string out = line;
    out.push_back('\n');
    int sent = send(s, out.c_str(), (int)out.size(), 0);
    return sent == (int)out.size();
}

inline bool recv_line(SOCKET s, std::string& out)
{
    out.clear();
    char ch;
    while (true)
    {
        int r = recv(s, &ch, 1, 0);
        if (r <= 0) return false;
        if (ch == '\n') break;
        out.push_back(ch);
    }
    return true;
}
