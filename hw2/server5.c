#include <stdio.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <arpa/inet.h>

#define MAX 256
#define PORT 8080
#define SA struct sockaddr

// Function designed for chat between client and server.
void func(int sockfd)
{
    char buff[MAX];
    int n;
    // infinite loop for chat
    for (;;) {
        bzero(buff, MAX);

        // read the message from client and copy it in buffer
        while(read(sockfd, buff, sizeof(buff)) <= 0)
            ;
        // print buffer which contains the client contents
        printf("From client: %s\t To client : ", buff);
        bzero(buff, MAX);
        n = 0;
        // copy server message in the buffer
        while ((buff[n++] = getchar()) != '\n')
            ;

        // and send that buffer to client
        write(sockfd, buff, sizeof(buff));

        // if msg contains "Exit" then server exit and chat ended.
        if (strncmp("exit", buff, 4) == 0) {
            printf("Server Exit...\n");
            break;
        }
    }
}

// Driver function
int main()
{
    int sockfd, connfd;
    socklen_t len;
    struct sockaddr_in servaddr, cli;

    // socket create and verification
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
    servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    servaddr.sin_port = htons(PORT);

    // Binding newly created socket to given IP and verification
    if ((bind(sockfd, (SA*)&servaddr, sizeof(servaddr))) != 0) {
        printf("socket bind failed...\n");
        exit(0);
    }
    else
        printf("Socket successfully binded..\n");

    // Now server is ready to listen and verification
    if ((listen(sockfd, 5)) != 0) {
        printf("Listen failed...\n");
        exit(0);
    }
    else
        printf("Server listening..\n");

    // test
    fd_set master; // master file descriptor 清單
  fd_set read_fds; // 給 select() 用的暫時 file descriptor 清單
  int fdmax; // 最大的 file descriptor 數目
  char buf[256]; // 儲存 client 資料的緩衝區
  char cpbuf[256];
  int nbytes;

  char remoteIP[INET6_ADDRSTRLEN];

  int yes=1; // 供底下的 setsockopt() 設定 SO_REUSEADDR
  int i, j, rv, n;
  int user_no[2];
  char* user[] = {"apple", "banana"};
  char* passwd[] = {"000", "111"};
  char number[30];
  int status[100];
  int pair[2];
  pair[0] = 9;
  pair[1] = 9;


  for(i=0 ; i<100 ; i++)
  {
     status[i] = 0;
  }

  struct addrinfo hints, *ai, *p;

  FD_ZERO(&master); // 清除 master 與 temp sets
  FD_ZERO(&read_fds);


  // 將 sockfd 新增到 master set
  FD_SET(sockfd, &master);

  // 持續追蹤最大的 file descriptor
  fdmax = sockfd; // 到此為止，就是它了

  while(1)
  {
      read_fds = master; // 複製 master

    if (select(fdmax+1, &read_fds, NULL, NULL, NULL) == -1) {
      perror("select");
      exit(4);
    }

    // 在現存的連線中尋找需要讀取的資料
    for(i = 0; i <= fdmax; i++) {
        if (FD_ISSET(i, &read_fds))
        { // 我們找到一個！！
             if (i == sockfd) {
                 // handle new connections
                 len = sizeof(cli);
                 connfd = accept(sockfd, (SA*)&cli, &len);
                 if (connfd < 0) {
                     printf("server acccept failed...\n");
                     exit(0);
                 }
                 else
                 {
                     FD_SET(connfd, &master); // 新增到 master set
                     if (connfd > fdmax) { // 持續追蹤最大的 fd
                         fdmax = connfd;
                    }
                    printf("selectserver: new connection from socket %d\n",connfd);
                 }
             } // end if it is listen socket
             else
             {
                 bzero(buf, MAX);
                 bzero(cpbuf, MAX);
                 read(i, buf, sizeof(buf));
                 for(int k=0 ; k<strlen(buf); k++ )
                 {
                     if(buf[k] == '\n')
                     {
                         cpbuf[k] = '\0';
                         break;
                     }
                     else cpbuf[k] = buf[k];
                 }

                 // check status
                 if(status[i] == 0)
                 {
                     for(j=0; j<2; j++)
                     {
                         if(strcmp(user[j], cpbuf) == 0)
                         {
                             status[i] = j+1;
                             bzero(buf, MAX);
                             strcpy(buf, "enter passwd : ");
                             write(i, buf, sizeof(buf));
                         }
                     }
                     if(status[i] == 0)
                     {
                         bzero(buf, MAX);
                         strcpy(buf, "user not exist");
                         write(i, buf, sizeof(buf));
                     }
                 }
                 else if(status[i] == 1)
                 {
                     if(strcmp(passwd[0], cpbuf) == 0)
                     {
                         status[i] = 3;
                         user_no[0] = i;
                         bzero(buf, MAX);
                         strcpy(buf, "u r logging");
                         write(i, buf, sizeof(buf));
                     }
                     else
                     {
                         bzero(buf, MAX);
                         strcpy(buf, "wrong passwd");
                         write(i, buf, sizeof(buf));
                     }

                 }
                 else if(status[i] == 2)
                 {
                     if(strcmp(passwd[1], cpbuf) == 0)
                     {
                         status[i] = 4;
                         user_no[1] = i;
                         bzero(buf, MAX);
                         strcpy(buf, "u r logging");
                         write(i, buf, sizeof(buf));
                     }
                     else
                     {
                         bzero(buf, MAX);
                         strcpy(buf, "wrong passwd");
                         write(i, buf, sizeof(buf));
                     }

                 }
                 else if(status[i] == 3)
                 {
                     if (strncmp("challenge", cpbuf, 9) == 0) {

                         status[i] = 5;

                         bzero(buf, MAX);
                         strcpy(buf, "challenge who : ");
                         write(i, buf, sizeof(buf));
                     }
                     else if (strncmp("logout", cpbuf, 6) == 0) {

                         status[i] = 0;
                         user_no[0] = 0;
                         bzero(buf, MAX);
                         strcpy(buf, "u r logout");
                         write(i, buf, sizeof(buf));
                     }
                     else if (strncmp("online", cpbuf, 6) == 0) {

                         bzero(buf, MAX);
                         strcpy(buf, "online user : ");
                         for(int k=0 ; k<2 ; k++)
                         {
                             if(user_no[k] != 0)
                             {
                                 sprintf(number, "%d", k);
                                 strcat(buf, "user");
                                 strcat(buf, number);
                                 strcat(buf, " ");
                             }
                         }
                         write(i, buf, sizeof(buf));
                     }

                 }
                 else if(status[i] == 4)
                 {
                     if (strncmp("logout", cpbuf, 6) == 0) {

                         status[i] = 0;
                         user_no[1] = 0;
                         bzero(buf, MAX);
                         strcpy(buf, "u r logout");
                         write(i, buf, sizeof(buf));
                     }
                     else if (strncmp("accept", cpbuf, 6) == 0)
                     {
                         if(pair[i] != 9)
                         {
                             bzero(buf, MAX);
                             strcpy(buf, "accept your challenge");
                             write(user_no[pair[i]], buf, sizeof(buf));
                             status[i] = 14;
                             status[user_no[pair[i]]] = 13;
                         }

                     }
                     else if (strncmp("online", cpbuf, 6) == 0) {

                         bzero(buf, MAX);
                         strcpy(buf, "online user : ");
                         for(int k=0 ; k<2 ; k++)
                         {
                             if(user_no[k] != 0)
                             {
                                 sprintf(number, "%d", k);
                                 strcat(buf, "user");
                                 strcat(buf, number);
                                 strcat(buf, " ");
                             }
                         }
                         write(i, buf, sizeof(buf));
                     }

                 }
                 else if(status[i] == 5)
                 {
                     int value;
                     value = atoi(cpbuf);

                     pair[value] = i;

                     bzero(buf, MAX);
                     strcpy(buf, cpbuf);
                     strcat(buf, " challenge u !");
                     write(user_no[value], buf, sizeof(buf));

                 }
                 else if(status[i] == 13)
                 {
                     if (strncmp("win", cpbuf, 3) == 0)
                     {
                         status[i] = 3;
                         status[user_no[pair[i]]] = 4;

                     }
                     else if (strncmp("lose", cpbuf, 3) == 0)
                     {
                         status[i] = 3;
                         status[user_no[pair[i]]] = 4;

                     }
                     else if (strncmp("fair", cpbuf, 3) == 0)
                     {
                         status[i] = 3;
                         status[user_no[pair[i]]] = 4;

                     }

                         bzero(buf, MAX);
                         strcpy(buf, cpbuf);
                         write(user_no[1], buf, sizeof(buf));




                 }
                 else if(status[i] == 14)
                 {
                     if (strncmp("non", cpbuf, 3) == 0) {
                     }
                     else if (strncmp("win", cpbuf, 3) == 0)
                     {
                         status[i] = 4;
                         status[user_no[pair[i]]] = 3;

                     }
                     else if (strncmp("fair", cpbuf, 3) == 0)
                     {
                         status[i] = 4;
                         status[user_no[pair[i]]] = 3;

                     }
                     else if (strncmp("lose", cpbuf, 3) == 0)
                     {
                         status[i] = 4;
                         status[user_no[pair[i]]] = 3;
                     }

                         bzero(buf, MAX);
                         strcpy(buf, cpbuf);
                         write(user_no[0], buf, sizeof(buf));




                 }
                 //printf("status[%d] = %d\n", i, status[i]);

             }
        }
   }
  }

//test
    // After chatting close the socket
    close(sockfd);
}
