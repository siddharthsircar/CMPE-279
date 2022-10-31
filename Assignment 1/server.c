// Server side C/C++ program to demonstrate Socket programming

#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <pwd.h>
#include <string.h>

#define PORT 80
int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Attaching socket to port 80
    if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
                   &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // Forcefully attaching socket to the port 80
    if (bind(server_fd, (struct sockaddr *)&address,
             sizeof(address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(server_fd, 3) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    // Privileges need to be dropped at this point in the program
    drop_privilege();

    if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
                             (socklen_t *)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }
    valread = read(new_socket, buffer, 1024);
    printf("%s\n", buffer);
    send(new_socket, hello, strlen(hello), 0);
    printf("Hello message sent\n");
    return 0;
}

void drop_privilege(){
    printf("UID is %d \n", getuid());
    int childp_status;
    struct passwd *pw;
    pid_t c_pid = fork();

    // Case for 0, if it's child process
    if(c_pid == 0) {
        pw = getpwnam("nobody");
        if( pw == NULL ) {
            printf("Not able to detect nobody.\n");
            exit(1);
        }
        printf("UID of Nobody %ld \n", (long)pw->pw_uid);
        long cp_uid = setuid(pw->pw_uid);
        if(setuid(pw->pw_uid) == -1) {
            perror("setuid failed");
            exit(EXIT_FAILURE);
        }
    } else if (c_pid > 0) { // Case for positive if it's the parent process
        wait(NULL);
        printf("Parent process\n");
        exit(1);
    } else { // Case for negative, when fork error occurs
        perror("fork failed");
        exit(EXIT_FAILURE);
    }
}
