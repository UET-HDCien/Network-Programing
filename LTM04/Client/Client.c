/*
	Author:Hoang Dang Kien
	Student ID:15021363
	Description: User enter server IP ,server port and buffer receive size, client create socket,set buffer size and connect to the socket that listen in Server in port entered, server opened port 8899. 
	After that Client can send file name to server, Server send file to Client , client can press QUIT to close connection!! .
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[]) {
	int sockfd;	//socket descripter
	int port;	// port to connect
	char serverIP[50];	//server IP
	int w; 		//number byte write to socket
	int r;		//number byte read from socket
	struct sockaddr_in serv_addr; // server address struct
	char buffer[1024];
	FILE * fp;
	int sum;
	size_t size;

	sockfd=socket(AF_INET,SOCK_STREAM,0);	//create socket IPv4
	if (sockfd<0) {
		perror("Error:");
		return 1;
	}
	size_t size1;
	socklen_t size2=sizeof(size1);
	getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size1, &size2);
	printf("Buffer Size default is:%lu\n",size1);
	
	printf("Note : You should connect to port 8899 in server!\n");
	printf("Enter server IP:");
	scanf("%s",serverIP);	//get serverIP from user
	printf("Enter server port:");
	scanf("%d",&port);	//get serverIP from user
	printf("Enter buffer Size:");
	scanf("%lu",&size);
	
	setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));
	printf("Buffer Size is set to:");
	getsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size1,&size2);
	printf("%lu\n",size1);

	bzero((char *) &serv_addr, sizeof(serv_addr));
	serv_addr.sin_family=AF_INET;		//IP v4 
	serv_addr.sin_addr.s_addr = inet_addr(serverIP); // Choose IP server to connect
	serv_addr.sin_port=htons(port);		//Choose port to connect
	if (connect(sockfd,(struct sockaddr *) &serv_addr,sizeof(serv_addr)) < 0) {	//Connect to server
		perror("Error connecting:");
		return 1;
	}
	else printf("Connected\n");
	
	//setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(int));
	char str[1024];
	printf("Enter \"QUIT\" to stop communiacation with server:\n");
	while (1) {
		sum=0;
		printf("Client need file:");
		scanf("%s",str);
		str[strlen(str)]='\0';
		//if (str[0]=='@') break;
		w = write(sockfd,str,strlen(str));		//send message to server
	    	if (w <= 0) {
			perror("Error writing to socket:\n");
			break;
		}
		if (str[0]=='Q' && str[1]=='U' && str[2]=='I' && str[3]=='T' && str[4]=='\0') {
			printf("Close conection!!\n");
			break;
		}
		//printf("Server message:");
		//char[4] lSize;
		unsigned int lSize;
		/*unsigned char res[4];
		read(sockfd, res, 4);
		//printf("Halo %02x %02x %02x %02x\n",res[0],res[1],res[2],res[3]);
		unsigned int a0=res[0];
		unsigned int a1=res[1];
		unsigned int a2=res[2];
		unsigned int a3=res[3];
		if (a0==255 && a1==255 && a2==255 && a3==255) {
			printf("File not found!\n");
			continue;
		}

		lSize=a0*256*256*256+a1*256*256+a2*256+a3;*/
		
		read(sockfd, &lSize, 4);
		if (lSize==0) {
			printf("File not found!\n");
			continue;
		}
		printf("File size is:%d\n",lSize);
		fp=fopen(str,"w");
		//lSize=lSize+200;
		//for ( int i=0;i<1024;i++) buffer[i]='/0';
		clock_t t;
		t=clock();
		while(lSize>0) {	
			r=read(sockfd,buffer,1024);
			buffer[r]='\0';
			fwrite(buffer,1,r,fp);
			//int r2=fwrite(buffer , 1 , strlen(buffer) , fw );
			sum=sum+r;
			lSize=lSize-r;
		}
		t=clock()-t;
		double time_taken = ((double)t)/CLOCKS_PER_SEC;
		fclose(fp);
		printf("Total bytes received :%d\n",sum);
		printf("Total time: %f\n",time_taken);
		//read message from server
		//if (r<=0) {
			//perror("Error read from socket!\n");
			//break;
		//}
		//
		//int r=read(sockfd,buffer,255);
		//printf("Message from server:%s",buffer);
	}
	close(sockfd);
	return 0;
}
