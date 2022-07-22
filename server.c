#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<unistd.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<netinet/in.h>

void error (const char* msg){
	perror(msg);
	exit(1);
}

int main(int argc, char  * argv[]){
	if(argc<2){
		fprintf(stderr, "Port no not provided. Program terminated\n" );
		exit(1);
	}
	
	int sockfd, newsockfd,newsockfd1, portno, n;
	char buffer[255];
	
	struct sockaddr_in serv_addr, clin_addr, clin_addr1;
	socklen_t clinlen,clinlen1;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd<0){
		error("Error opening socket");
	}
	
	bzero((char *) &serv_addr , sizeof(serv_addr));
	portno = atoi(argv[1]);

	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = INADDR_ANY;
	serv_addr.sin_port = htons(portno);
	
	
	
	if(bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr))<0)
		error("Binding Failed");
	listen(sockfd, 3);
	clinlen = sizeof(clin_addr);
	newsockfd = accept(sockfd, (struct sockaddr *) &clin_addr , &clinlen);
	clinlen1 = sizeof(clin_addr1);
	newsockfd1 = accept(sockfd, (struct sockaddr *) &clin_addr1 , &clinlen1);

	if (newsockfd<0 && newsockfd1<0)
		error("Error on accepting");
	bzero(buffer, 255);
	fgets(buffer, 255, stdin);
	n = write(newsockfd, buffer, strlen(buffer));
	n = write(newsockfd1, buffer, strlen(buffer));


	while(1){
		bzero(buffer, 255);
		//fgets(buffer, 255, stdin);
		n = read(newsockfd, buffer, 255);
		n = write(newsockfd1, buffer, strlen(buffer));
		if (n<0)
			error("Error on writing");
			
		bzero(buffer, 255);
		n = read(newsockfd1, buffer, 255);
		n = write(newsockfd, buffer, strlen(buffer));
		if(n<0)
			error("Error on reading");
		//printf("Client : %s\n", buffer);
		

		int i = strncmp("Close", buffer, 5);
		if(i==0)
			break;

	}
	close(newsockfd);
	close(sockfd);
	return 0;
}
