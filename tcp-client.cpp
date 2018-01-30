#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstdlib>
#include <memory.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "packet.h"

#define BUF_LEN 128

int main(int argc, char *argv[])
{
    int s, n;
    struct sockaddr_in server_addr;
    //struct sockaddr_in server_addr : 서버의 소켓주소 구조체
    char buf[BUF_LEN+1];


    if((s = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    {//소켓 생성과 동시에 소켓 생성 유효검사
        printf("can't create socket\n");
        exit(0);
    }


    bzero((char *)&server_addr, sizeof(server_addr));
    //서버의 소켓주소 구조체 server_addr을 NULL로 초기화

    server_addr.sin_family = AF_INET;
    //주소 체계를 AF_INET 로 선택
    server_addr.sin_addr.s_addr = inet_addr("192.168.0.31");
    //32비트의 IP주소로 변환
    server_addr.sin_port = htons(3600);
    //daytime 서비스 포트 번호

    if(connect(s, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0)
    {//서버로 연결요청
        printf("can't connect.\n");
        exit(0);
    }

    int seq = 0;
    while(1) {
        getchar();
        const int arrSize = 30;
        char hh[arrSize];
        PACKET* pk = (PACKET*)malloc(sizeof(PACKET) + sizeof(char) * arrSize);
        pk->head[0] = 0x55;
        pk->head[1] = 0x33;
        pk->head[2] = 0xaa;
        pk->totalLen = sizeof(PACKET) + sizeof(char) * arrSize;
        pk->cmdType = 1;

        for(int i=0; i<arrSize; i++) {
            pk->raw[i] = htonl(rand() % 10);
        }
        write(s, pk, sizeof(PACKET) + sizeof(char) * arrSize);
        free(pk); 
    }

    close(s);
    //사용이 완료된 소켓을 닫기
}


