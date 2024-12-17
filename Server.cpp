#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdio.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

int __cdecl main()
{
    WORD wVersionRequested;
    WSADATA wsaData;
    int err;
    SOCKET serverSocket;
    struct sockaddr_in server;

    // Инициализация Winsock
    wVersionRequested = MAKEWORD(2, 2);
    err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    // Создание сокета
    if ((serverSocket = socket(AF_INET, SOCK_STREAM, 0)) == INVALID_SOCKET) {
        std::cerr << "Could not create socket: " << WSAGetLastError() << std::endl;
        WSACleanup();
        return 1;
    }
    std::cout << "Socket created." << std::endl;

    // Подготовка структуры sockaddr_in
    server.sin_family = AF_INET;
    server.sin_port = htons(32460);

    // Конвертация IP-адреса в сетевой порядок байт
    if (inet_pton(AF_INET, "192.168.56.1", &server.sin_addr) <= 0) {
        std::cerr << "Invalid address/ Address not supported" << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }

    // Привязка сокета
    if (bind(serverSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        std::cerr << "Bind failed with error code: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Bind done." << std::endl;

    // Прослушивание входящих соединений
    if (listen(serverSocket, 3) == SOCKET_ERROR) {
        std::cerr << "Listen failed with error code: " << WSAGetLastError() << std::endl;
        closesocket(serverSocket);
        WSACleanup();
        return 1;
    }
    std::cout << "Listening..." << std::endl;
    SOCKET clientSocket;
    struct sockaddr_in client; 
    int clientSize = sizeof(client);
    clientSocket = accept(serverSocket, (struct sockaddr*)&client, &clientSize);
    if (clientSocket == INVALID_SOCKET) { std::cerr << "Accept failed with error code: " << WSAGetLastError() << std::endl; closesocket(serverSocket); WSACleanup(); return 1; } std::cout << "Client connected." << std::endl;
    const char* message = "Hello, client!";
    int messageLength = strlen(message);
    if (send(clientSocket, message, messageLength, 0) == SOCKET_ERROR) 
    { std::cerr << "Send failed with error code: " << WSAGetLastError() << std::endl; closesocket(clientSocket); closesocket(serverSocket); WSACleanup(); return 1; } std::cout << "Message sent to client." << std::endl; 
    closesocket(clientSocket);
    closesocket(serverSocket);
    WSACleanup();

    return 0;
}
