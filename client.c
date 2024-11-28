#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg){
  
  perror("Error");
  fprintf(stderr,"ERROR: %s\n", msg);
  exit(1);
}

void usage(void){
  printf("Usage: ./client [SERVER IP] [PORT NUMBER]\n");
  exit(1);
}

int main(int argc, char *argv[]){

  if(argc < 3)
    usage();

  int sockfd , portnumber, n;
  char buffer[255];

  struct sockaddr_in serv_addr;
  struct hostent *server;
  socklen_t clilen;

  portnumber = atoi(argv[2]);

  sockfd = socket(AF_INET, SOCK_STREAM, 0);
  if(sockfd < 0){
    error("Could not open a socket.");
  }
  
  server = gethostbyname(argv[1]);
  if(server == NULL)
    error("No host located.");
  
  bzero((char *) &serv_addr, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
/*bcopy((char *) server->h_addr, (char *) &serv_addr.sin_addr.s_addr, server->h_length);*/
  memcpy(&serv_addr.sin_addr.s_addr, server->h_addr, server->h_length);
  serv_addr.sin_port = htons(portnumber);
  if(connect(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr))<0)
    error("Connection not established");
  
  while(1){
    bzero(buffer, sizeof(buffer));
    fgets(buffer, sizeof(buffer), stdin);
    
    n = write(sockfd, buffer, strlen(buffer));
    if(n < 0)
      error("Writing failure.");
    
    bzero(buffer, sizeof(buffer));
    n = read(sockfd, buffer, sizeof(buffer));
    if(n < 0)
      error("Reading failure.");
    printf("Server: %s", buffer);

    int i = strncmp("Bye", buffer, 3);
    if(i == 0)
       break;
  }
  
  close(sockfd);

  return 0;
}
