#include <iostream>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: client \"add 5 3;sub 8 2;fact 5\"\n";
        return 1;
    }

    WSADATA wsa;
    WSAStartup(MAKEWORD(2, 2), &wsa);

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);

    sockaddr_in serverAddr;
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_port = htons(8080);
    serverAddr.sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(clientSocket, (sockaddr*)&serverAddr, sizeof(serverAddr)) < 0) {
        std::cerr << "Connection failed.\n";
        return 1;
    }

    send(clientSocket, argv[1], strlen(argv[1]), 0);

    char buffer[2048];
    int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0);
    if (bytesReceived > 0) {
        buffer[bytesReceived] = '\0';
        std::cout << "Result from server:\n" << buffer;
    }

    closesocket(clientSocket);
    WSACleanup();
    return 0;
}
