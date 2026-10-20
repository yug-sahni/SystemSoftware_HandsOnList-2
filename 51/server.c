#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int server_fd, client_fd;
    struct sockaddr_in server;
    char buffer[100];

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    if (bind(server_fd, (struct sockaddr *)&server,
             sizeof(server)) == -1)
    {
        perror("bind");
        close(server_fd);
        return 1;
    }

    if (listen(server_fd, 5) == -1)
    {
        perror("listen");
        close(server_fd);
        return 1;
    }

    printf("Server waiting for client...\n");

    client_fd = accept(server_fd, NULL, NULL);
    if (client_fd == -1)
    {
        perror("accept");
        close(server_fd);
        return 1;
    }

    int n = read(client_fd, buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        buffer[n] = '\0';
        printf("Message received: %s\n", buffer);

        const char *reply = "Hello from server!";
        write(client_fd, reply, strlen(reply));
    }

    close(client_fd);
    close(server_fd);

    return 0;
}
