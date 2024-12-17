#define WIN32_LEAN_AND_MEAN
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <iostream>

#pragma comment(lib, "ws2_32.lib")

using namespace std;

int main() {
    WORD wVersionRequested = MAKEWORD(2, 2);
    WSADATA wsaData;
    int err = WSAStartup(wVersionRequested, &wsaData);
    if (err != 0) {
        printf("WSAStartup failed with error: %d\n", err);
        return 1;
    }

    SOCKET clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (clientSocket == INVALID_SOCKET) {
        cerr << "Could not create socket: " << WSAGetLastError() << endl;
        WSACleanup();
        return 1;
    }
    cout << "Socket created." << endl;

    struct sockaddr_in server;
    server.sin_family = AF_INET;
    server.sin_port = htons(32460);
    if (inet_pton(AF_INET, "192.168.56.1", &server.sin_addr) <= 0) {
        cerr << "Invalid address/ Address not supported" << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }

    if (connect(clientSocket, (struct sockaddr*)&server, sizeof(server)) == SOCKET_ERROR) {
        cerr << "Connect error: " << WSAGetLastError() << endl;
        closesocket(clientSocket);
        WSACleanup();
        return 1;
    }
    cout << "Connected to server." << endl;
    char buffer[1024]; int bytesReceived = recv(clientSocket, buffer, sizeof(buffer) - 1, 0); if (bytesReceived == SOCKET_ERROR) { cerr << "Recv failed with error: " << WSAGetLastError() << endl; closesocket(clientSocket); WSACleanup(); return 1; } 
    buffer[bytesReceived] = '\0'; 
    cout << "Message from server: " << buffer;
    closesocket(clientSocket);
    WSACleanup();

    return 0;
}
