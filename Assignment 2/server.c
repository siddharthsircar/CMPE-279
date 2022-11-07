// Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <pwd.h>
#define PORT 8080

void re_exec_child(int new_socket)
{
    printf("Child UID : %d \n", getuid());
    pid_t c_pid = fork();
    int c_status;

    if (c_pid < 0)
    { // Error case - negative value
        perror("Fork failed");
        exit(EXIT_FAILURE);
    }
    else if (c_pid == 0)
    { // Child process case - zero value
        char socket_add[12];
        sprintf(socket_add, "%d", new_socket);
        char *argument_list[] = {"./server", socket_add, NULL};
        /*
            Ref: https://www.digitalocean.com/community/tutorials/execvp-function-c-plus-plus
            execvp() takes in the name of the UNIX command to run, as the first argument ( unistd header file is imported for this )
            followed by the array of command line arguments we want to pass to the UNIX command
         */
        if (execvp(argument_list[0], argument_list) < 0)
        {
            perror("EXECVP Failure");
            exit(EXIT_FAILURE);
        }
    }
    else if (c_pid > 0)
    { // Parent process case - positive value
        printf("Parent process waiting after fork\n");
        waitpid(c_pid, &c_status, 0);
        printf("Parent process exiting after child finished\n");
    }
}

int main(int argc, char const *argv[])
{
    int server_fd, new_socket, valread;
    struct sockaddr_in address;
    int opt = 1;
    int addrlen = sizeof(address);
    char buffer[1024] = {0};
    char *hello = "Hello from server";

    // Runs when no command line argument is passed. It will run in initial execution
    if (argc == 1)
    {
        // Creating socket file descriptor
        if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
        {
            perror("socket failed");
            exit(EXIT_FAILURE);
        }
        // Attaching socket to port 80
        if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)))
        {
            perror("setsockopt");
            exit(EXIT_FAILURE);
        }
        address.sin_family = AF_INET;
        address.sin_addr.s_addr = INADDR_ANY;
        address.sin_port = htons(PORT);

        // Forcefully attaching socket to the port 80
        if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0)
        {
            perror("bind failed");
            exit(EXIT_FAILURE);
        }
        if (listen(server_fd, 3) < 0)
        {
            perror("listen");
            exit(EXIT_FAILURE);
        }
        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t *)&addrlen)) < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        // This funtion re-executes server code from child process
        re_exec_child(new_socket);
    }
    else
    { // Runs when 2 command line arguments are passed. It will run when socket argument is passed
        struct passwd *pw;
        pw = getpwnam("nobody"); // getting nobody user

        if (pw == NULL)
        {
            printf("Not able to detect nobody.\n");
            exit(1);
        }
        printf("UID of Nobody %ld \n", (long)pw->pw_uid);
        long cp_uid = setuid(pw->pw_uid);

        if (setuid(pw->pw_uid) == -1)
        {
            perror("setuid failed");
            exit(EXIT_FAILURE);
        }
        // Reading the new socket from ./server runs
        new_socket = atoi(argv[1]);
        valread = read(new_socket, buffer, 1024);
        printf("%s\n", buffer);
        send(new_socket, hello, strlen(hello), 0);
        printf("Hello message sent\n");
        return 0;
    }
}