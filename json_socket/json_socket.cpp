#include "json_socket.h"
#ifndef _MSC_VER
#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <cstring>
#define SOCKET_BUFFER_TYPE void *
#else
#include <Winsock2.h>
#include <Ws2tcpip.h>
#include <stdint.h>
#define SOCKET_BUFFER_TYPE char *
#endif

json_socket::json_socket(string host,string port) {
    int bsock_fd = socket(AF_INET, SOCK_STREAM, 0);

    if(bsock_fd == -1) {
        cout << "Unable to create socket" << strerror(errno) << endl;
        throw UnableToConnect();
    }

    char reuse_addr = 1;
    setsockopt(bsock_fd, SOL_SOCKET, SO_REUSEADDR, reinterpret_cast<SOCKET_BUFFER_TYPE>(&reuse_addr), 1);

    struct addrinfo *host_addr;
    struct addrinfo hints;
    memset(&hints, 0, sizeof(struct addrinfo));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_INET;
    hints.ai_addr = NULL;
    hints.ai_next = NULL;

    int r = getaddrinfo(host.c_str(), port.c_str(), &hints, &host_addr);

    if(r != 0) {
        cout << "Unable to resolve server address: " << gai_strerror(r) << endl;
        throw UnableToConnect();
    }

    bool connected = false;
    for(struct addrinfo *rp = host_addr; rp != NULL; rp = rp->ai_next) {
        if(connect(bsock_fd, host_addr->ai_addr, host_addr->ai_addrlen) == 0) {
            connected = true;
            break;
        }
        cout << "Unable to connect to server: " << strerror(errno) << endl;
    }

    freeaddrinfo(host_addr);
    if(!connected) {
        throw UnableToConnect();
    }

    this->bsock_fd = bsock_fd;
}

json_socket::~json_socket() {
    close(this->bsock_fd);
}

bool json_socket::send_msg(Json::Value root) {
    Json::FastWriter writer;

    string jsonstr = writer.write(root);

    char jsonsize_buf[4];
    uint32_t jsonsize = htonl(jsonstr.size());
    memcpy(&jsonsize_buf, &jsonsize, 4);

    int s = send(this->bsock_fd, reinterpret_cast<SOCKET_BUFFER_TYPE>(&jsonsize_buf), 4, 0);
    s = send(this->bsock_fd, jsonstr.c_str(), jsonstr.size(),0);

    return true;
}

Json::Value json_socket::recv_msg() {
    char jsonsize_buf[4];
    int s = recv(this->bsock_fd, reinterpret_cast<SOCKET_BUFFER_TYPE>(&jsonsize_buf), 4, 0);

    if (s == 0)
        throw PeerDisconnected();
    else if (s < 0)
        throw ConnectionError();

    unsigned int jsonsize;
    memcpy(&jsonsize, &jsonsize_buf, 4);

    jsonsize = ntohl(jsonsize);

    char *jsonstr = new char[jsonsize + 1];
    int bytes_read = 0;

    while (bytes_read < jsonsize) {
        s = recv(this->bsock_fd, jsonstr+bytes_read, jsonsize, 0);

        if (s <= 0) {
            delete[] jsonstr;
            if (s == 0)
                throw PeerDisconnected();
            else
                throw ConnectionError();
        }

        bytes_read += s;
    }

    jsonstr[jsonsize] = (char)0;
    this->last_jsonstr = jsonstr;

    Json::Value root;
    Json::Reader reader;
    bool r = reader.parse(jsonstr, &jsonstr[jsonsize], root);
    if(!r) {
        cout << "Unable to parse input json: " << reader.getFormatedErrorMessages() << endl;
        cout << jsonstr << endl;

        delete[] jsonstr;
        throw NotParseableJson();
    }

    delete[] jsonstr;

    return root;
}
