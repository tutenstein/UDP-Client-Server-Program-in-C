#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#define MAXLINE 255



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
    int server_struct_length = sizeof(serv_addr);
    char server_message[255], client_message[255];

    portno = atoi(argv[1]);
    sockfd = socket(AF_INET,SOCK_DGRAM,0);
    if (sockfd<0)
        perror("Socket is not created.");
    printf("Socket is Created.\n");


    bzero((char *)&serv_addr,sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(portno);
    serv_addr.sin_addr.s_addr = INADDR_ANY;



    while(1){

          bzero(client_message, 255);
          bzero(server_message, 255);
          printf("Enter the mesage: ");
          fgets(client_message,255,stdin); 
        
          n = sendto(sockfd, client_message, strlen(client_message), 0,(struct sockaddr*)&serv_addr, server_struct_length);
          if (n<0)
              error("Error on writting");
          printf("Waiting for reply from server...\n ");
          n = recvfrom(sockfd, server_message, sizeof(server_message), 0,(struct sockaddr*)&serv_addr, &server_struct_length);
          if (n<0)
             error("Error on reading");
          printf("Server: %s\n", server_message);
          int i = strncmp("exit", server_message, 4);
          if (i==0)
              break;
   }

   close(sockfd);
   return 0;

}
