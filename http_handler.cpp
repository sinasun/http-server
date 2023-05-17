#include "http_handler.h"

HttpHandler::HttpHandler(int new_socket) : socketfd(new_socket)
{
}

void HttpHandler::receiveRequest()
{
    char buffer[1024] = {0};
    int bytes_read = read(socketfd, buffer, 1024);
    if (bytes_read == -1)
    {
        std::cerr << "Error reading from client socket" << std::endl;
    }
    else if (bytes_read == 0)
    {
        std::cout << "Client with socket fd " << socketfd << " disconnected" << std::endl;
        close(socketfd);
    }
    else
    {
        std::cout << "Received request from client with socket fd " << socketfd << std::endl;
        std::string request = buffer;

        RequestHeader request_header = parseRequestHeader(request.substr(0, request.find("\r\n\r\n")));
        std::string request_body = request.substr(request.find("\r\n\r\n") + 1, request.length());
        }
}

RequestHeader HttpHandler::parseRequestHeader(std::string header)
{
    RequestHeader request_header;
    std::istringstream iss(header);
    std::string line;

    std::getline(iss, line);
    std::istringstream line_stream(line);
    line_stream >> request_header.type >> request_header.request_url;

    while (std::getline(iss, line) && line != "\r")
    {
        std::istringstream header_stream(line);
        std::string header_name, header_value;
        std::getline(header_stream, header_name, ':');
        std::getline(header_stream, header_value);

        header_value.erase(0, header_value.find_first_not_of(' '));

        if (header_name == "User-Agent")
        {
            request_header.user_agent = header_value;
        }
        else if (header_name == "Accept")
        {
            request_header.respond_type = header_value;
        }
        else if (header_name == "Host")
        {
            request_header.host = header_value;
        }
        else if (header_name == "Content-Type")
        {
            request_header.content_type = header_value;
        }
        else if (header_name == "Content-Length")
        {
            request_header.content_length = std::stoi(header_value);
        }
    }
}