#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>

int main()
{
    int sock;
    struct sockaddr_in server;
    char buffer[100];

    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock == -1)
    {
        perror("socket");
        return 1;
    }

    memset(&server, 0, sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = htons(8080);

    if (inet_pton(AF_INET, "127.0.0.1",
                  &server.sin_addr) != 1)
    {
        printf("Invalid server IP address.\n");
        close(sock);
        return 1;
    }

    if (connect(sock, (struct sockaddr *)&server,
                sizeof(server)) == -1)
    {
        perror("connect");
        close(sock);
        return 1;
    }

    printf("Enter message: ");
    fflush(stdout);

    if (fgets(buffer, sizeof(buffer), stdin) != NULL)
        write(sock, buffer, strlen(buffer));

    int n = read(sock, buffer, sizeof(buffer) - 1);

    if (n > 0)
    {
        buffer[n] = '\0';
        printf("Server reply: %s\n", buffer);
    }

    close(sock);

    return 0;
}
