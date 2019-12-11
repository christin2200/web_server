#include <stdio.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <netinet/in.h>
#include <string.h>
#include <fcntl.h>
#define PORT 8080

char *skip(char *message, char *to_skip)
{
    char *p_message = message;
    if (strncmp(p_message, to_skip, strlen(to_skip)) == 0) {
        while (*p_message != '\n') {
            p_message += 1;
        }

        p_message += 1;
    }

    return p_message;
}

void write_to_file_until(char *message, char *end, FILE *fp)
{
    char abuffer[1024] = {};
    char *p_buffer = abuffer;
    while (message != end) {
        *p_buffer = *message;

        p_buffer += 1;
        message += 1;
    }

    *p_buffer = '\0';
    //printf("abuffer %s\n", abuffer);
    int j = 0;
    while(j<strlen(abuffer))
    {
        fprintf(fp, "%c", abuffer[j]);
        j++;

    }

    //fwrite(abuffer, sizeof(char), strlen(abuffer), fp);
}




int main(int argc, char *argv[])
{
    int server_fd = 0, new_socket = 0, exit_status = 0;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    pid_t childpid = 0;

    // 建立 socket
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("In socket");
        exit(EXIT_FAILURE);
    }

    // bind

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    memset(address.sin_zero, '\0', sizeof address.sin_zero);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
       perror("In bind");
       close(server_fd);
       exit(EXIT_FAILURE);
    }

    // listen

    if (listen(server_fd, 10) < 0)
    {
        perror("In listen");
        close(server_fd);
        exit(EXIT_FAILURE);
    }


    // accept 重點部分

    while(1)
    {
        printf("\n+++++++ Waiting for new connection ++++++++\n\n");

        if ((new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
        {
            perror("In accept");
            continue;
        }
        // fork

        childpid = fork();
        if(childpid < 0)
        {
            perror("In fork");
            close(server_fd);
            close(new_socket);
            exit(EXIT_FAILURE);
        }
        else if(childpid == 0)
        {
            char buffer[500000] = {0};
            char buffer2[50000] = {0};

            //讀取瀏覽器要求
            long valread = read( new_socket , buffer, sizeof(buffer));
            int k = 0;
            while(k<sizeof(buffer))
            {
                printf("%c",buffer[k] );
                k++;

            }


            strcpy(buffer2, buffer);

            if (strncmp(buffer,"POST ",4) == 0)
            {
                //尋找檔名
                char * ptr = buffer;
                char *position = strstr(ptr, "filename=");

                position += strlen("filename=\"");
                char filename[1024] = {};
                strcpy(filename, position);
                *strchr(filename, '"') = '\0';
                //printf( "filename: %s\n", filename );

                //開檔
                FILE *pFile = fopen( filename, "wb" );

                //尋找內容

                position = strstr(ptr, "boundary=");
                position += strlen("boundary=");
                char boundary[1024] = {};
                strcpy(boundary, position);
                char *boundary_token = strtok(boundary,"\n");
                //printf( "boundary_www:\033[1;31m%s\033[m\n", boundary_token );
                position += strlen(boundary_token);
                position = strstr(position, boundary_token);
                //printf("position: \033[1;32m%s\033[m\n", position);

                //寫入檔案
                position = skip(position, boundary_token);
                //printf("position: \033[1;33m%s\033[m\n", position);
                //printf( "boundary: \033[1;31m%s\033[m\n", boundary_token );
                char *end = strstr(position, "----");
                //printf("end: \033[1;35m%s\033[m\n", end);

                char *skip_str[] = {
            "Content-Disposition:",
            "Content-Type:"
                };

                //position = skip(position, boundary_token);
                //printf("position: \033[1;33m%s\033[m\n", position);
                position = skip(position, skip_str[0]);
                //printf("position: \033[1;34m%s\033[m\n", position);
                position = skip(position, skip_str[1]);
                //printf("position: \033[1;35m%s\033[m\n", position);
                position = skip(position, "\n\r\n\r");
                //printf("position: \033[1;36m%s\033[m\n", position);
                write_to_file_until(position, end, pFile);


                fclose(pFile);
                //printf("this is post\n");
            }

            //回傳給瀏覽器

                int file_fd, ret;
                char buf[1000000];
                char *hello = "HTTP/1.1 200 OK\nContent-Type: text/html\n\n</h1>";
                write(new_socket , hello , strlen(hello));



                /* 開啟檔案 */
                if((file_fd=open("index.html",O_RDONLY))==-1)
                write(new_socket, "Failed to open file", 19);


                /* 讀取檔案內容輸出到客戶端瀏覽器 */
                while ((ret=read(file_fd, buf, 100))>0) {
                    write(new_socket,buf,ret);
                }

                //printf("------------------Hello message sent-------------------\n");



            close(new_socket);
            exit(0);
        }
        else
        {
            //  parent process
            wait(&exit_status);
            close(new_socket);
        }
    }
    close(server_fd);

    return 0;

}
