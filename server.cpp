#include "server.h"

int main(int arg)
{
    int server_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (server_socket == -1)
    {
        std::cerr << "Failed to create socket." << std::endl;
        return 1;
    }

    struct sockaddr_in server_address
    {
    };
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_port = htons(8080);

    if (bind(server_socket, (struct sockaddr *)&server_address, sizeof(server_address)) == -1)
    {
        std::cerr << "Failed to bind socket to address." << std::endl;
        close(server_socket);
        return 1;
    }

    if (listen(server_socket, SOMAXCONN) == -1)
    {
        std::cerr << "Failed to listen for connections." << std::endl;
        close(server_socket);
        return 1;
    }

    std::vector<int> client_fds;
    std::vector<std::thread> threads;
    std::mutex client_mutex;

    while (true)
    {
        int new_socket = accept(server_socket, nullptr, nullptr);
        if (new_socket == -1)
        {
            std::cerr << "Error accepting connection from client" << std::endl;
        }
        else
        {
            std::cout << "Accepted new client connection with socket fd " << new_socket << std::endl;
            HttpHandler *handler = new HttpHandler(new_socket);

            std::lock_guard<std::mutex> lock(client_mutex);
            client_fds.push_back(new_socket);

            std::thread thread(&HttpHandler::receiveRequest, handler);
            threads.emplace_back(std::move(thread));
        }
    }

    for (int fd : client_fds)
    {
        close(fd);
    }

    close(server_socket);

    return 0;
}