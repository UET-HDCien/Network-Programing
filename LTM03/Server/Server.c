/*
	Author:Hoang Dang Kien
	Student ID:15021363
	Description: Server open a port(8899) and wait connect from client , after client connect , Client send file name and server send file in server folder to client.
	If client entered QUIT, server close connection.
*/
#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h> 
#include <arpa/inet.h>
#include <errno.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>

void sig_chld(int signo)
{
	pid_t	pid;
	int		stat;

	while ( (pid = waitpid(-1, &stat, WNOHANG)) > 0)
		printf("child %d terminated\n", pid);
	return;
}

int main(int argc, char *argv[]) {
	int sockfd;	// Server listen socket descripter
	int clientsockfd;	//Client connect socket descripter
	struct sockaddr_in serv_addr, cli_addr;	//Server address and Client address
	int w;	//num bytes write to socket 
	int r;	//num bytes read from socket 
	FILE * fp; //File to send to Client
	socklen_t length;	
	char buffer_read[1024];	//
	char buffer[1024];		//
	int lSize,sizeToRead;	//
	int blockSize;
	int BLOCK_SIZE=512;
	int sum=0;	//total bytes send
	pid_t	childpid;	//child process id
	size_t result;		//
	void sig_chld(int);
	//unsigned int SIGCHLD;
	//signal_t SIGCHLD;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);	//Create a socket IPv4
     	if (sockfd < 0) {
        	perror("Error while creating socket!");
		return 1;
	}
	int optval = 1;
    setsockopt(sockfd, SOL_SOCKET, SO_REUSEPORT, &optval, sizeof(optval));

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family = AF_INET;		//IP v4
     	serv_addr.sin_addr.s_addr = INADDR_ANY;	//default IP address
     	serv_addr.sin_port = htons(8899);	//Port 8899

	if (bind(sockfd, (struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) { 	//Associate socket with IP address and port number
        perror("Error while binding");
		return 1;
	}
	listen(sockfd,10);	//Listen in sockfd, Accept for 5 connects
	signal(SIGCHLD, sig_chld);
	while(1) {
		length = sizeof(cli_addr);
		//clientsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &length);		//Accept connect from client
		if ( (clientsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, &length))<0) {
			if (errno == EINTR)
				continue;		/* back to for() */
			else
				perror("Accept error!");
		}
		if ( (childpid = fork()) == 0) {	/* child process */
			close(sockfd);	/* close listening socket */
			//str_echo(connfd);	/* process the request */
			while (1) {
			sum=0;	
			for (int i=0;i<1024;i++) buffer_read[i]='\0'; 	//init buffer=null
			//for (int i=0;i<1024;i++) buffer_write[i]='\0'; 	
			r = read(clientsockfd,buffer_read,1024);	//Read message from client by socket
		    	if (r <= 0) {
				perror("Error reading from socket!!");
				break;
			}
			printf("Client need file:%s\n",buffer_read);
			if (buffer_read[0]=='Q' && buffer_read[1]=='U' && buffer_read[2]=='I' && buffer_read[3]=='T' && buffer_read[4]=='\0') {
				printf("Close conection!!\n");
				break;
			}
			fp=fopen(buffer_read,"rb");
			if (fp==NULL) {
        		unsigned int a=0; 
				write(clientsockfd, &a, 4);
        		continue;
    		}
			fseek (fp , 0 , SEEK_END);
    		lSize = ftell (fp);
    		rewind (fp);
    		sizeToRead = lSize;
			write(clientsockfd, &lSize, 4);
			printf("The size of the file is:%d\n",lSize);
			while(sizeToRead > 0){
        		if (sizeToRead >= BLOCK_SIZE){
            			blockSize = BLOCK_SIZE;
        		}
        		else{
            			blockSize = sizeToRead;
        		}
        		result = fread (buffer, 1, blockSize, fp);
        		printf("Size %d\n",blockSize);
				buffer[result] = '\0';
				w=write(clientsockfd,buffer,result);
				sum=sum+w;
        		sizeToRead -= result;
    		}
    		
			printf("Total bytes sent:%d\n",sum);
			
			fclose(fp);
			
	     }
			exit(0);
		}		
		
	     close(clientsockfd);	
	}
	close(sockfd);
	return 0;
}
