#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include<arpa/inet.h>

#define MAXLINE 255



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
   char server_message[255], client_message[255];
   struct sockaddr_in serv_addr,cli_addr;
    int client_struct_length = sizeof(cli_addr);


   sockfd = socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
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
   	      printf("Waiting for data...");
	      fflush(stdout);
          bzero(client_message, 255);
          bzero(server_message, 255);
   
          n = recvfrom(sockfd, client_message, sizeof(client_message), 0,(struct sockaddr*)&cli_addr, &client_struct_length);
          if (n<0)
              error("Error on reading");
          printf("Received packet from %s:%d\n", inet_ntoa(cli_addr.sin_addr), ntohs(cli_addr.sin_port));
          printf("Client: %s\n", client_message);
          fgets(server_message,255,stdin); 

          n = sendto(sockfd, server_message, strlen(server_message), 0,(struct sockaddr*)&cli_addr, client_struct_length);
          if (n<0)
              error("Error on writting");
          int i = strncmp("exit", server_message, 4);
          if (i==0)
          break;
   }

   close(sockfd);
   return 0;


}
