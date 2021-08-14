#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <sys/socket.h>
#include <netinet/in.h>

int main()
{
	int socketfd = 0;
	int NewConnectedfd = 0;
	struct sockaddr_in servaddr, clientaddr;
	int retval = 0;
	socklen_t clilen;

	/*bind the application to the network service provided by kernel as per protocol requirements*/
	socketfd = socket(AF_INET/*IPv4 protocal*/, SOCK_STREAM /*connection orinted like TCP*/, 0/*protocol id here TCP as IPv4 and stream is used*/);
	if (socketfd < 0) 
	{
		perror("sock:");
		exit(1);
	}
	printf("\n\tsocket created successfully\n");

	bzero(&servaddr, sizeof(servaddr));// clear structure like memzero
	servaddr.sin_family = AF_INET/*IPv4 protocal*/;
	// htonl converts the value used by host in the network understandable fromate Host TO Network Layer
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);	// any configured ip of server running machine
	// htons converts the value used by host in the network understandable fromate Host TO Network Service
	servaddr.sin_port = htons(8000);	//port no 8000 used for this perticular server so that network service send data to this server only

	retval = bind(socketfd, (struct sockaddr *)&servaddr, sizeof(servaddr));/* assign address and protocol parameters to created socket */
	/* if another already running service running with same sockaddr_in settings this call fails */
	if (retval < 0) 
	{
		perror("bind:");
		exit(2);
	}
        printf("\n\tbinding created successfully\n");

	retval = listen(socketfd, 5);/* listen tells network service to monitor socket and autheticate and wake up if the all pass */
	/* above is non blocking call it just tell network service to start monitring in polling/sleep-wakeup way*/
	/* second argument is the max length of queue for this created socket */
	/* this means if 5 clients started requesting simulteniously then they can be queued */
        if (retval < 0) 
        {
                perror("listen:");
                exit(2);
        }
        printf("\n\tlistening started successfully\n");

	while(1)
	{
		clilen = sizeof(clientaddr);
		// This is blocking call when listen started listing and authenticated requeste received then accept is woken up
		// It returns the parameters of the client as out parameter 
		NewConnectedfd = accept(socketfd, (struct sockaddr *)&clientaddr, &clilen);
		if (NewConnectedfd < 0) 
		{
			perror("sock:");
			exit(1);
		}
		printf("\n\tclient connected successfully\n");
		printf("\n\tclient's port no = %d\n", ntohs(clientaddr.sin_port));

		char buf[200] = {0};
		int n = 0;

		//read on the fd returned by accept 
		//hence we endup reading to client in below call
		n = read(NewConnectedfd, buf, 200);
		buf[n] = '\0';
		printf("\n\tReceived data from client is :: %s\n", buf);

		char writebuf[200] = {0};
	        printf("\n\tEnter data to be sent to server::\n");
        	fgets(writebuf, 200, stdin);

        	write(NewConnectedfd, writebuf, 200);
        	printf("\n\twrite to client successfully\n");


	}
}
