#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/wait.h>
#include <signal.h>

int main() {
    int server_fd, client_fd;
    struct sockaddr_in server, client;
    socklen_t len = sizeof(client);

    signal(SIGCHLD, SIG_IGN);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == -1)
    {
        perror("socket");
        return 1;
    }

    int opt = 1;
    setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = htonl(INADDR_ANY);
    server.sin_port = htons(8080);

    if (bind(server_fd, (struct sockaddr *)&server, sizeof(server)) == -1)
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

    printf("Server waiting for clients...\n");

    while (1) {
        client_fd = accept(server_fd, (struct sockaddr *)&client, &len);

        if (client_fd < 0)
        {
            perror("accept");
            continue;
        }

        pid_t pid = fork();

        if (pid < 0)
        {
            perror("fork");
            close(client_fd);
            continue;
        }

        if (pid == 0) {
            char buffer[1024] = {0};

            close(server_fd);

            int n = read(client_fd, buffer, sizeof(buffer) - 1);
            if (n > 0)
            {
                buffer[n] = '\0';
                printf("Client message: %s\n", buffer);

                char *reply = "Hello from fork server!";
                write(client_fd, reply, strlen(reply));
            }

            close(client_fd);
            exit(0);
        }

        close(client_fd);
    }

    close(server_fd);
    return 0;
}
