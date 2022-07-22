#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>
#include<netdb.h>


void error (const char* msg){
	perror(msg);
	exit(1);
}

int main(int argc, char  * argv[]){
	if(argc<3){
		fprintf(stderr, "Data Missing. Program terminated\n" );
		exit(1);
	}

	int sockfd, newsockfd, portno, n;
	struct hostent *server;
	char buffer[255];

	struct sockaddr_in serv_addr;
	portno = atoi(argv[2]);
	
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0){
		error("Error opening socket");
	}

	server = gethostbyname(argv[1]);
	if(server==NULL)
		fprintf(stderr, "Error No Host\n");

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;
	bcopy((char*) server->h_addr, (char *)&serv_addr.sin_addr.s_addr,server->h_length);
	serv_addr.sin_port = htons(portno);
	if(connect(sockfd, (struct sockaddr *) &serv_addr , sizeof(serv_addr))<0)
		error("Connection Failed");
	else
		printf("Connected\n");
	while(1){
		
		bzero(buffer, 255);
		n = read(sockfd, buffer , 255);
		if (n<0){
			error("Error on Reading");
		}
		printf("Server : %s\n",buffer);
		bzero(buffer, 255);
		fgets(buffer, 255, stdin);
		n = write(sockfd, buffer, strlen(buffer));
		if (n<0)
			error("Error on writing");
		int i = strncmp("Close", buffer, 5);
		if (i==0)
			break;
	}
	close(sockfd);
	return 0;


}
