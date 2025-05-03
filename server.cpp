#include <iostream>
#include <winsock2.h>
#include <string>
#include <sstream>
#include <algorithm>
#pragma comment(lib, "ws2_32.lib")

int add(int a, int b)        { return a + b; }
int subtract(int a, int b)   { return a - b; }
int multiply(int a, int b)   { return a * b; }
int factorial(int n) {
    int f = 1;
    for (int i = 2; i <= n; ++i) f *= i;
    return f;
}

static std::string trim(const std::string& s) {
    auto f = s.find_first_not_of(" \t\r\n");
    auto l = s.find_last_not_of (" \t\r\n");
    return (f == std::string::npos) ? "" : s.substr(f, l - f + 1);
}

std::string processCommand(const std::string& cmd) {
    std::istringstream iss(cmd);
    std::string op; int a, b;
    iss >> op;
    std::ostringstream out;

    if (op == "add") {
        iss >> a >> b;
        out << a << " + " << b << " = " << add(a,b);
    }
    else if (op == "sub") {
        iss >> a >> b;
        out << a << " - " << b << " = " << subtract(a,b);
    }
    else if (op == "mul") {
        iss >> a >> b;
        out << a << " * " << b << " = " << multiply(a,b);
    }
    else if (op == "fact") {
        iss >> a;
        out << a << "! = " << factorial(a);
    }
    else {
        out << "Unknown: " << cmd;
    }
    return out.str();
}

int main() {
    WSADATA wsa;
    WSAStartup(MAKEWORD(2,2), &wsa);

    SOCKET srv = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in addr{ AF_INET, htons(8080), INADDR_ANY };
    bind(srv, (sockaddr*)&addr, sizeof(addr));
    listen(srv, 1);

    std::cout << "Server listening on 8080...\n";
    SOCKET cli = accept(srv, nullptr, nullptr);

    char buf[1024];
    int len = recv(cli, buf, sizeof(buf)-1, 0);
    if (len > 0) {
        buf[len] = '\0';
        std::string request = buf;
        std::cout << "[Debug] Received: \"" << request << "\"\n";

        std::istringstream splitter(request);
        std::string part, reply;
        while (std::getline(splitter, part, ';')) {
            std::string cmd = trim(part);
            if (!cmd.empty())
                reply += processCommand(cmd) + "\n";
        }

        send(cli, reply.c_str(), (int)reply.size(), 0);
        std::cout << "[Debug] Sent: \n" << reply;
    }

    closesocket(cli);
    closesocket(srv);
    WSACleanup();
    return 0;
}
