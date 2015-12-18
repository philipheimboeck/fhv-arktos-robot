//
// Created by Philip Heimböck (Privat) on 18.12.15.
//

#include <netinet/in.h>
#include <netdb.h>
#include <stddef.h>
#include <strings.h>
#include <unistd.h>
#include "TcpClient.h"

TcpClient::TcpClient(const char* host, int port) {
    fd = socket(AF_INET, SOCK_STREAM, 0);
    if (fd < 0) {
        throw "Error while opening the socket!";
    }

    server = gethostbyname(host);
    if (server == NULL) {
        throw "No such host!";
    }

    bzero((char*) &host_addr, sizeof(host_addr));
    host_addr.sin_family = AF_INET;
    bcopy(server->h_addr, (char*) &host_addr.sin_addr.s_addr, server->h_length);
    host_addr.sin_port = htons(port);

    if (connect(fd, (struct sockaddr*) &host_addr, sizeof(host_addr)) < 0) {
        throw "Could not connect";
    }
}

TcpClient::~TcpClient() {
    close(fd);
}

void TcpClient::send(const char* data, size_t size) {
    ssize_t n = write(fd, data, size);
    if (n < 0) {
        throw "Error while sending!";
    }
}

size_t TcpClient::receive(char* buffer, size_t size) {
    ssize_t n = read(fd, buffer, size);
    if (n < 0) {
        throw "Error while reading!";
    }
    return (size_t) n;
}