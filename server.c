#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg){
  
  perror("Error");
  fprintf(stderr,"ERROR: %s\n", msg);
  exit(1);
}

void usage(void){
  printf("Usage: ./server [LISTENING PORT]\n");
  exit(1);
}

int main(int argc, char *argv[]){

  if(argc < 2)
    usage();

  int sockfd , newsockfd, portnumber, n;
  char buffer[255];

  struct sockaddr_in serv_addr, cli_addr;
  socklen_t clilen;

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    error("Could not open a socket");
  }
  
  bzero((char *)&serv_addr, sizeof(serv_addr));

  portnumber = atoi(argv[1]);

  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  serv_addr.sin_port = htons(portnumber);

  if(bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0)
    error("Binding Failed.");
  
  listen(sockfd, 5);
  clilen = sizeof(cli_addr);

  newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &clilen);

  if(newsockfd < 0)
    error("Accept conection failed.");

  while(1){
    bzero(buffer, sizeof(buffer));
    n = read(newsockfd, buffer, sizeof(buffer));
    if(n < 0)
      error("Reading message failed.");
    printf("Client: %s", buffer);
    bzero(buffer, sizeof(buffer));
    fgets(buffer, sizeof(buffer), stdin);

    n = write(newsockfd, buffer, strlen(buffer));
    if(n < 0)
      error("Writing message failed.");

    int i = strncmp("Bye", buffer, 3);
    if(i == 0)
      break;
  }

  close(newsockfd);
  close(sockfd);
  return 0;
}
