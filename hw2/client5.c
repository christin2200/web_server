#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <curses.h>
#define MAX 256
#define PORT 8080
#define SA struct sockaddr

char square[10] = { 'o', '1', '2', '3', '4', '5', '6', '7', '8', '9' };

int checkwin();
void board();
void func(int sockfd)
{
    char buff[MAX];
    int choice;
    int n, check;
    char number[30];
    char mark1 = 'O';
    char mark2 = 'X';
    int cho, i;
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
                printf("(3) accpet \n");
                printf("(4) log out \n");
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
                    // chanllenge
                    bzero(buff, sizeof(buff));
                    strcpy(buff, "challenge");
                    write(sockfd, buff, sizeof(buff));

                    bzero(buff, sizeof(buff));
                    read(sockfd, buff, sizeof(buff));
                    fprintf(stderr, "%s\n", buff);

                    bzero(buff, sizeof(buff));
                    n = 0;
                    while ((buff[n++] = getchar()) != '\n')
                        ;
                    write(sockfd, buff, sizeof(buff));

                    bzero(buff, sizeof(buff));
                    read(sockfd, buff, sizeof(buff));
                    fprintf(stderr, "%s\n", buff);

                    // game start



                    do
                    {
                        board();

                        printf("enter a number:  ");
                        scanf("%d", &cho);



                            square[cho] = mark1;

                            i = checkwin();
                            if(i == 1)
                            {
                                bzero(buff, sizeof(buff));
                                strcpy(buff, "win");
                                write(sockfd, buff, sizeof(buff));
                                printf("game over result : win\n");
                                break;

                            }
                            if(i == 0)
                            {
                                bzero(buff, sizeof(buff));
                                strcpy(buff, "fair");
                                write(sockfd, buff, sizeof(buff));
                                printf("game over result : 平手\n");
                                break;

                            }

                        // wait react
                        bzero(buff, sizeof(buff));
                        sprintf(number, "%d", cho);
                        strcpy(buff, number);
                        write(sockfd, buff, sizeof(buff));

                        bzero(buff, sizeof(buff));
                        read(sockfd, buff, sizeof(buff));

                        if (strncmp("fair", buff, 3) == 0)
                        {
                            printf("game over result : 平手\n");
                            break;
                        }
                        if (strncmp("lose", buff, 3) == 0)
                        {
                            printf("game over result : win\n");
                            break;
                        }
                        if (strncmp("win", buff, 3) == 0)
                        {
                            printf("game over result : lose\n");
                            break;
                        }

                        cho = atoi(buff);
                        square[cho] = mark2;

                        i = checkwin();
                        if(i == 1)
                        {
                            bzero(buff, sizeof(buff));
                            strcpy(buff, "lose");
                            write(sockfd, buff, sizeof(buff));
                            printf("game over result : lose\n");
                            break;

                        }
                        if(i == 0)
                        {
                            bzero(buff, sizeof(buff));
                            strcpy(buff, "fair");
                            write(sockfd, buff, sizeof(buff));
                            printf("game over result : 平手\n");
                            break;

                        }

                    }while (i ==  - 1);


                }
                else if( choice == 3)
                {
                    //accept
                    bzero(buff, sizeof(buff));
                    strcpy(buff, "accept");
                    write(sockfd, buff, sizeof(buff));

                    bzero(buff, sizeof(buff));
                    read(sockfd, buff, sizeof(buff));

                    bzero(buff, sizeof(buff));
                    strcpy(buff, "non");
                    write(sockfd, buff, sizeof(buff));

                    // game start

                    do
                    {
                        // wait react

                        bzero(buff, sizeof(buff));
                        read(sockfd, buff, sizeof(buff));

                        if (strncmp("fair", buff, 3) == 0)
                        {
                            printf("game over result : 平手\n");
                            break;
                        }
                        if (strncmp("lose", buff, 3) == 0)
                        {
                            printf("game over result : win\n");
                            break;
                        }
                        if (strncmp("win", buff, 3) == 0)
                        {
                            printf("game over result : lose\n");
                            break;
                        }

                        cho = atoi(buff);
                        square[cho] = mark1;

                        i = checkwin();
                        if(i == 1)
                        {
                            bzero(buff, sizeof(buff));
                            strcpy(buff, "lose");
                            write(sockfd, buff, sizeof(buff));
                            printf("game over result : lose\n");
                            break;

                        }
                        if(i == 0)
                        {
                            bzero(buff, sizeof(buff));
                            strcpy(buff, "fair");
                            write(sockfd, buff, sizeof(buff));
                            printf("game over result : 平手\n");
                            break;

                        }

                        board();

                        printf("enter a number:  ");
                        scanf("%d", &cho);



                            square[cho] = mark2;

                            i = checkwin();
                            if(i == 1)
                            {
                                bzero(buff, sizeof(buff));
                                strcpy(buff, "win");
                                write(sockfd, buff, sizeof(buff));
                                printf("game over result : win\n");
                                break;

                            }
                            if(i == 0)
                            {
                                bzero(buff, sizeof(buff));
                                strcpy(buff, "fair");
                                write(sockfd, buff, sizeof(buff));
                                printf("game over result : 平手\n");
                                break;

                            }
                        bzero(buff, sizeof(buff));
                        sprintf(number, "%d", cho);
                        strcpy(buff, number);
                        write(sockfd, buff, sizeof(buff));


                    }while (i ==  - 1);
                    //printf("game over result : %d\n", i);


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

int checkwin()
{
    if (square[1] == square[2] && square[2] == square[3])
        return 1;

    else if (square[4] == square[5] && square[5] == square[6])
        return 1;

    else if (square[7] == square[8] && square[8] == square[9])
        return 1;

    else if (square[1] == square[4] && square[4] == square[7])
        return 1;

    else if (square[2] == square[5] && square[5] == square[8])
        return 1;

    else if (square[3] == square[6] && square[6] == square[9])
        return 1;

    else if (square[1] == square[5] && square[5] == square[9])
        return 1;

    else if (square[3] == square[5] && square[5] == square[7])
        return 1;

    else if (square[1] != '1' && square[2] != '2' && square[3] != '3' &&
        square[4] != '4' && square[5] != '5' && square[6] != '6' && square[7]
        != '7' && square[8] != '8' && square[9] != '9')

        return 0;
    else
        return  - 1;
}


/*******************************************************************
FUNCTION TO DRAW BOARD OF TIC TAC TOE WITH PLAYERS MARK
 ********************************************************************/


void board()
{

    printf("\n\n\tTic Tac Toe\n\n");

    printf("     |     |     \n");
    printf("  %c  |  %c  |  %c \n", square[1], square[2], square[3]);

    printf("_____|_____|_____\n");
    printf("     |     |     \n");

    printf("  %c  |  %c  |  %c \n", square[4], square[5], square[6]);

    printf("_____|_____|_____\n");
    printf("     |     |     \n");

    printf("  %c  |  %c  |  %c \n", square[7], square[8], square[9]);

    printf("     |     |     \n\n");
}
