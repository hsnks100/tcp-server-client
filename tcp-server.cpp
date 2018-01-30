#include <stdio.h>
#include <stdlib.h>
#include <memory.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/select.h>
#include <map>
#include "packet.h"
#include "circular-buffer.h"


#define PORTNUM 3600
#define SOCK_SETSIZE 1021

pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t thread_cond = PTHREAD_COND_INITIALIZER;
 

std::map<int, CircularBuffer> buffers;


void* recvProcessor(void* b) {
    while(1) {
        pthread_mutex_lock(&mutex); // 잠금을 생성한다. 
        for(auto iter=buffers.begin(); iter != buffers.end(); ++iter) {
            iter->second.read(
        }
        Packet pk;
        if(buffers

        pthread_mutex_unlock(&mutex); // 잠금을 생성한다. 

    }
}
int main(int argc, char **argv)
{
    int listen_fd, client_fd;
    socklen_t addrlen;
    int fd_num;
    int maxfd = 0;
    int sockfd;
    int readn;
    int i= 0;
    fd_set readfds, allfds;
 
    struct sockaddr_in server_addr, client_addr;
 
    if((listen_fd = socket(AF_INET, SOCK_STREAM, 0)) == -1)
    {
        perror("socket error");
        return 1;
    }   

    int sockopt = 1;
    if(setsockopt(listen_fd, SOL_SOCKET, SO_REUSEADDR, 
                (char*) &sockopt, sizeof(sockopt)) == -1){
        puts("error socket option : REUSEADDR");
        return -1;
    }
    memset((void *)&server_addr, 0x00, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(PORTNUM);
     
    if(bind(listen_fd, (struct sockaddr *)&server_addr, sizeof(server_addr)) == -1)
    {
        perror("bind error");
        return 1;
    }   
    if(listen(listen_fd, 5) == -1)
    {
        perror("listen error");
        return 1;
    }   
     
    


    pthread_t thread;
    pthread_create(&thread, NULL, recvProcessor, (void *)nullptr);
    FD_ZERO(&readfds);
    FD_SET(listen_fd, &readfds);
 
    maxfd = listen_fd;
    while(1)
    {
        allfds = readfds;
        fd_num = select(maxfd + 1 , &allfds, (fd_set *)0,
                      (fd_set *)0, NULL);
 
        if (FD_ISSET(listen_fd, &allfds))
        {
            addrlen = sizeof(client_addr);
            client_fd = accept(listen_fd,
                    (struct sockaddr *)&client_addr, &addrlen);
 
            FD_SET(client_fd,&readfds);
 
            if (client_fd > maxfd)
                maxfd = client_fd;
            printf("Accept OK\n");
            continue;
        }
 
        for (i = 0; i <= maxfd; i++)
        {
            sockfd = i;
            PACKET pk, *pPk;
            if (FD_ISSET(sockfd, &allfds))
            {
                char bigBuffer[2048];
                if( (readn = read(sockfd, bigBuffer, 2048)) == 0)
                {
                    printf("close\n");
                    close(sockfd);
                    FD_CLR(sockfd, &readfds);
                }
                else {
                    pthread_mutex_lock(&mutex); // 잠금을 생성한다. 
                    buffers[sockfd].write(bigbuffer, readn);
                    pthread_mutex_unlock(&mutex); // 잠금을 생성한다.  
                }

                if (--fd_num <= 0)
                    break;
            }
        }
    }
}

