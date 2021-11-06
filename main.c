#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAXLINE 1024



void error(const char *msg ){
    perror(msg);
    exit(1);
}


int main(int argc, char *argv[])
{

   if (argc < 2){
        fprintf(stderr, "usage %s port\n", argv[0]);
        exit(0);
   }
    int sockfd, portno, n;
    struct sockaddr_in serv_addr;
    int addr_size = sizeof(serv_addr);
    char buffer[255];

    portno = atoi(argv[2]);
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd<0)
        perror("Socket is not created.");
    printf("Socket is Created.\n");


    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;



    while(1){

          bzero(buffer, 255);
          printf("Enter the mesage: ");
          fgets(buffer,255,stdin);
          n = sendto(sockfd,(char *)buffer,sizeof(&buffer),MSG_WAITALL, ( struct sockaddr *) &serv_addr,addr_size);
          if (n<0)
              error("Error on writting");
          printf("Waiting for reply from server...\n ");
          bzero(buffer, 255);
          n = recvfrom(sockfd,(char *)buffer, MAXLINE,MSG_WAITALL, ( struct sockaddr *) &serv_addr,&addr_size);
          if (n<0)
             error("Error on reading");
          printf("Server: %s\n", buffer);
          int i = strncmp("exit", buffer, 3);
          if (i==0)
              break;
   }

   close(sockfd);
   return 0;

}
