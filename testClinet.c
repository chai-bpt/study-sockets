#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

int main()
{
	int socketfd = 0;
	struct sockaddr_in servaddr;
	int retval = 0;
	char *serv_ip = "127.0.0.1";

	/*bind the application to the network service provided by kernel as per protocol requirements*/
	socketfd = socket(AF_INET/*IPv4 protocal*/, SOCK_STREAM /*connection orinted like TCP*/, 0/*protocol id here TCP as IPv4 and stream is used*/);
	if (socketfd < 0) 
	{
		perror("sock:");
		exit(1);
	}
	printf("\n\tsocket created successfully\n");


	// Bind is the static assignment but No need to bind in the client 
	// General practice is in client go for dynamic assignment and it is randomly assigned by the system 

	// Fill down the server details to whome client wanted to connect
	bzero(&servaddr, sizeof(servaddr));// clear structure like memzero
	servaddr.sin_family = AF_INET/*IPv4 protocal*/;
	// htons converts the value used by host in the network understandable fromate Host TO Network Service
	servaddr.sin_port = htons(8000);	//port no 8000 used for this perticular server so that network service send data to this server only
	// Assign server ip in the sturcture
	inet_pton(AF_INET/*IPv4 protocal*/, serv_ip, &servaddr.sin_addr);
	
	// conneect to a perticular server set in the structure
	retval = connect(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr));
	if (retval < 0) {
		perror("connect:");
		exit(1);
	}
	printf("\n\tconnection with server established successfully\n");

}
