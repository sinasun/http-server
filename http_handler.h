#ifndef HTTP_HANDLER_H
#define HTTP_HANDLER_H

#include <string>
#include <iostream>
#include <sstream>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <fstream>
#include <thread>
#include <mutex>

struct RequestHeader
{
    std::string type;
    std::string request_url;
    std::string user_agent;
    std::string respond_type;
    std::string host;
    std::string content_type;
    int content_length;
};

class HttpHandler
{
public:
    int socketfd;
    HttpHandler(int socketfd);
    RequestHeader parseRequestHeader(std::string request_header);
    void handlePost();
    void receiveRequest();
    void sendRepond();
};

#endif