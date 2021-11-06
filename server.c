#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<arpa/inet.h>

#define MAXLINE 1024



void error(const char *msg ){
    perror(msg);
    exit(1);
}

int main(int argc, char *argv[])
{
   if (argc < 2){
        fprintf(stderr, "port No not provided. Program terminated\n");
        exit(1);}

   int sockfd,portno,n;
   char buffer[MAXLINE];
   struct sockaddr_in serv_addr,cli_addr;
   int clilen = sizeof(cli_addr);

   sockfd = socket(AF_INET,SOCK_DGRAM,0);
   if (sockfd<0)
       perror("Socket is not created.");
   printf("Socket is Created.\n");

   bzero((char *)&serv_addr, sizeof(serv_addr));
   portno = atoi(argv[1]);
   serv_addr.sin_family = AF_INET;
   serv_addr.sin_addr.s_addr = INADDR_ANY;
   serv_addr.sin_port = htons(portno);

   if (bind(sockfd,(struct sockaddr*)&serv_addr,sizeof(serv_addr))<0)
       perror("Binding is failed");
   printf("binding is done.\n");

   while(1){
          bzero(buffer, 255);


          n = recvfrom(sockfd,(char *)buffer,MAXLINE,MSG_WAITALL, ( struct sockaddr *) &cli_addr,&clilen);
          if (n<0)
              error("Error on reading");
          printf("Client: %s\n", buffer);
          bzero(buffer, 255);
          fgets(buffer,255,stdin);

          n = sendto(sockfd,(char *)buffer,sizeof(&buffer),MSG_WAITALL, ( struct sockaddr *) &cli_addr,clilen);

          bzero((char *)&cli_addr, sizeof(cli));

          if (n<0)
              error("Error on writting");
          int i = strncmp("exit", buffer, 4);
          if (i==0)
          break;
   }

   close(sockfd);
   return 0;


}
