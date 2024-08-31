#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>
#include "../shared/Data.h"

using namespace std;

int main() {
    // Create TCP client socket
    int clientSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

    sockaddr_in serverAddress;
    serverAddress.sin_family = AF_INET;
    serverAddress.sin_port = htons(8888);
    inet_aton("127.0.0.1", (in_addr*) &serverAddress.sin_addr.s_addr);

    // Connect to server
    if (connect(clientSocket, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) == -1) {
        cout << "Error connecting to socket: " << strerror(errno) << endl;
        shutdown(clientSocket, SHUT_RDWR);
        return 0;
    } else {
        cout << "Client connect successful!" << endl;
        cout << "Client can start sending and receiving data." << endl;
    }

    char buffer[1024];
    printf("Enter your message: ");
    cin.getline(buffer, 1024);
    int byteCount = send(clientSocket, buffer, 1024, 0);
    if (byteCount == -1) {
        cout << "Client error sending message: " << strerror(errno) << endl;
        return -1;
    } else {
        cout << "Successfully sent " << byteCount << " bytes of data." << endl;
    }

    memset(buffer, 0, sizeof(buffer));
    byteCount = recv(clientSocket, buffer, sizeof(buffer), 0);
    if (byteCount < 0) {
        cout << "Client error receiving data: " << strerror(errno) << endl;
        return 0;
    } else {
        cout << "Message from server: " << buffer << endl;
    }


    // Create UDP client socket
    int clientSocketUDP = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);

    sockaddr_in serverAddressUDP;
    serverAddressUDP.sin_family = AF_INET;
    serverAddressUDP.sin_port = htons(9999);
    inet_aton("127.0.0.1", (in_addr*) &serverAddressUDP.sin_addr.s_addr);

    // Send a Data structure to the server
    memset(buffer, 0, sizeof(buffer));
    Data data("Client Bob the Flying Pig");
    data.setHealth(85);
    strcpy(buffer, data.serialize().c_str());

    int bytesSent = sendto(clientSocketUDP, (char *) buffer, strlen(buffer), 0, 
            (struct sockaddr*) &serverAddressUDP, sizeof(serverAddressUDP));
    if (bytesSent < 0) {
        cout << "Error transmitting UDP data." << endl;
        return 0;
    } else {
        cout << "Data sent: " << buffer << endl;
    }

    shutdown(clientSocket, SHUT_RDWR);
    shutdown(clientSocketUDP, SHUT_RDWR);

    return 0;
}