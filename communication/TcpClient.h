//
// Created by Philip Heimböck (Privat) on 18.12.15.
//

#ifndef ARCTOS_ROBOT_TCPCLIENT_H
#define ARCTOS_ROBOT_TCPCLIENT_H


#include <sys/socket.h>

class TcpClient {
private:
    int fd;
    int port;
    struct sockaddr_in host_addr;
    struct hostent* server;

public:
    TcpClient(const char* host, int port);
    ~TcpClient();

    void send(const char* data, size_t size);
    size_t receive(char* buffer, size_t size);
};


#endif //ARCTOS_ROBOT_TCPCLIENT_H
