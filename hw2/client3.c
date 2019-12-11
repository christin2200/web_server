#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#define MAX 256
#define PORT 8080
#define SA struct sockaddr
void func(int sockfd)
{
    char buff[MAX];
    int choice;
    int n, check;
    for (;;) {
        check = 0;
        bzero(buff, sizeof(buff));
        printf("\nEnter login : ");
        n = 0;
        while ((buff[n++] = getchar()) != '\n')
            ;
        write(sockfd, buff, sizeof(buff));

        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        fprintf(stderr, "%s", buff);

        if(strcmp("enter passwd : ", buff) == 0)
        {
            bzero(buff, sizeof(buff));
            n = 0;
            while ((buff[n++] = getchar()) != '\n')
                ;
            write(sockfd, buff, sizeof(buff));
        }


        bzero(buff, sizeof(buff));
        read(sockfd, buff, sizeof(buff));
        printf("\n%s\n", buff);
        if(strcmp("u r logging", buff) == 0)
        {
            while(1)
            {
                printf("(1) online\n");
                printf("(2) game \n");
                printf("(3) log out \n");
                printf("enter : ");
                scanf("%d", &choice);
                getchar();
                printf("\n");
                if( choice == 1)
                {
                    bzero(buff, sizeof(buff));
                    strcpy(buff, "online");
                    write(sockfd, buff, sizeof(buff));

                    bzero(buff, sizeof(buff));
                    read(sockfd, buff, sizeof(buff));
                    fprintf(stderr, "%s\n", buff);


                }
                else if( choice == 2)
                {
                
                }
                else
                {
                    bzero(buff, sizeof(buff));
                    strcpy(buff, "logout");
                    write(sockfd, buff, sizeof(buff));

                    bzero(buff, sizeof(buff));
                    read(sockfd, buff, sizeof(buff));
                    printf("%s\n", buff);
                    break;
                }

            }
        }

    }
}

int main()
{
    int sockfd, connfd;
    struct sockaddr_in servaddr, cli;

    // socket create and varification
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd == -1) {
        printf("socket creation failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully created..\n");
    bzero(&servaddr, sizeof(servaddr));

    // assign IP, PORT
    servaddr.sin_family = AF_INET;
    servaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
    servaddr.sin_port = htons(PORT);

    // connect the client socket to server socket
    if (connect(sockfd, (SA*)&servaddr, sizeof(servaddr)) != 0) {
        printf("connection with the server failed...\n");
        exit(0);
    }
    else
        printf("connected to the server..\n");

    // function for chat
    func(sockfd);

    // close the socket
    close(sockfd);
}
