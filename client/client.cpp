#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <cerrno>
#include <cstring>
#include <arpa/inet.h>

using namespace std;

int main() {
    // Create client socket
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

    return 0;
}