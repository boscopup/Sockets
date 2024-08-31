#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include "../shared/Data.h"

using namespace std;

int main() {
    char buffer[1024] = {0};
    
    // Create UDP server socket
    int serverSocketUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    // Define server address
    sockaddr_in serverAddressUDP;
    serverAddressUDP.sin_family = AF_INET;
    serverAddressUDP.sin_port = htons(9999);
    serverAddressUDP.sin_addr.s_addr = INADDR_ANY;

    // Bind server socket
    if (bind(serverSocketUDP, (struct sockaddr*) &serverAddressUDP, sizeof(serverAddressUDP)) == -1) {
        cout << "Error binding socket: " << strerror(errno) << endl;
        shutdown(serverSocketUDP, SHUT_RDWR);
        return 0;
    } else {
        cout << "Bind sucessful!" << endl;
    }

    // Create TCP server socket
    int serverSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    // Define server address
    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    serverAddress.sin_addr.s_addr = INADDR_ANY;

    // Bind server socket
    if (bind(serverSocket, (struct sockaddr*) &serverAddress, sizeof(serverAddress)) == -1) {
        cout << "Error binding socket: " << strerror(errno) << endl;
        shutdown(serverSocket, SHUT_RDWR);
        return 0;
    } else {
        cout << "Bind sucessful!" << endl;
    }

    // Listen for connections
    if (listen(serverSocket, 1) == -1) {
        cout << "Error listening on socket: " << strerror(errno) << endl;
    } else {
        cout << "Listening successful!" << endl;
    }

    // Accept client connection
    int clientSocket = accept(serverSocket, nullptr, nullptr);
    if (clientSocket == -1) {
        cout << "Error accepting socket: " << strerror(errno) << endl;
    }

    // Receive data from client

    int byteCount = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (byteCount < 0) {
        cout << "Server error receiving data: " << strerror(errno) << endl;
        return 0;
    } else {
        cout << "Message from client: " << buffer << endl;
    }

    memset(buffer, 0, sizeof(buffer));

    printf("Enter your message: ");
    cin.getline(buffer, 1024);
    byteCount = send(clientSocket, buffer, 1024, 0);
    if (byteCount == -1) {
        cout << "Server error sending message: " << strerror(errno) << endl;
    } else {
        cout << "Server: Successfully sent " << byteCount << " bytes of data." << endl;
    }



    int flags = 0;
    if (ioctl(serverSocketUDP, FIONBIO, &flags) == -1) {
        cout << "Error setting blocking mode: " << strerror(errno) << endl;
        shutdown(serverSocketUDP, SHUT_RDWR);
        return 0;
    } else {
        cout << "Blocking mode enabled." << endl;
    }

    // Receive UDP message
    sockaddr_in clientAddressUDP;
    socklen_t clientAddressUDPLength = (socklen_t) sizeof(clientAddressUDP);

    int bytesReceived = recvfrom(serverSocketUDP, buffer, 1024, 0,
            (struct sockaddr*) &clientAddressUDP, &clientAddressUDPLength);
    if (bytesReceived < 0) {
        cout << "Error recieving UDP message: " << strerror(errno) << endl;
    } else {
        cout << "Data recieved from client: " << buffer << endl;
    }
    Data data("");
    data.deserialize(buffer);
    cout << "Data received... " << endl;
    cout << "\tClient: " << data.getServerName() << endl;
    cout << "\tHealth: " << data.getHealth() << "%" << endl;

    shutdown(serverSocket, SHUT_RDWR);
    shutdown(serverSocketUDP, SHUT_RDWR);
    return 0;
}