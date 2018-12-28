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
//#include "kien.h"
#define clientport 1508
#define serverport 8051

typedef struct FileLocationRequest
{
	char type;
	char filename[20];	
};

typedef struct ClientAddress
{
	int IP;
	uint16_t bandwidth; //Mbps
};

typedef struct FileLocationRespond
{
	uint64_t fileSize;
	struct ClientAddress address[20];
};

typedef struct RequestData
{
	char fileName[20];
	uint64_t StartByte;
	uint64_t FinishByte;
};

struct FileLocationRespond requestListClient(char *srvIP, int srvPort, char *filename ) {
	int sockfd;
	struct sockaddr_in srv_addr;
	//int listIP[20];
	int IP;
	uint16_t bandwidth;
	int count=0;
	//int r;
	//int w;
	struct FileLocationRespond *respond=malloc(sizeof(struct FileLocationRespond));;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd<0) {
		perror("Error:");
		//return NULL;
	}
	bzero((char *) &srv_addr, sizeof(srv_addr));
	srv_addr.sin_family=AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr(srvIP);
	srv_addr.sin_port=htons(srvPort);
	if (connect(sockfd,(struct sockaddr *) &srv_addr,sizeof(srv_addr)) < 0) {	//Connect to server
		perror("Error connecting:");
		//return NULL;
	}
	char type=1	;
	write(sockfd,&type,sizeof(type));
	write(sockfd,filename,sizeof(filename));
	uint64_t fileSize;
	read(sockfd,&fileSize,sizeof(fileSize));
	respond->fileSize=fileSize;
	struct ClientAddress address[20];
	while (1==1) {
		read(sockfd,&IP,sizeof(IP));
		read(sockfd,&bandwidth,sizeof(bandwidth));
		respond->address[count].IP=IP;
		respond->address[count].bandwidth=bandwidth;
		count++;
	}
	return *respond;

}

int downloadfile(char *IP, int port, char *filename, uint64_t start, uint64_t finish, char *tmpfile) {
	int sockfd;
	struct sockaddr_in addr;
	char buffer[1024];
	int r;
	int w;
	int l;
	sockfd=socket(AF_INET,SOCK_STREAM,0);
	if (sockfd<0) {
		perror("Error:");
		return 1;
	}
	bzero((char *) &addr, sizeof(addr));
	addr.sin_family=AF_INET;
	addr.sin_addr.s_addr = inet_addr(IP);
	addr.sin_port=htons(port);
	if (connect(sockfd,(struct sockaddr *) &addr,sizeof(addr)) < 0) {	//Connect to server
		perror("Error connecting:");
		return 1;
	}
	l=strlen(filename);
	w=write(sockfd,&l,sizeof(int));
	if (w<0) {
		printf("%s\n","Write socket Error!" );
	}
	w=write(sockfd,filename,strlen(filename));
	if (w<0) {
		printf("%s\n","Write socket Error!" );
	}
	w=write(sockfd,&start,sizeof(uint64_t));
	if (w<0) {
		printf("%s\n","Write socket Error!" );
	}
	w=write(sockfd,&finish,sizeof(uint64_t));
	if (w<0) {
		printf("%s\n","Write socket Error!" );
	}
	size_t fileSize=finish-start+1;
	FILE *fp;
	fp=fopen(tmpfile,"a");
	fseek (fp , start , SEEK_SET);
	while (fileSize>0) {
		r=read(sockfd,buffer,1024);
		fileSize-=r;
		buffer[r]='\0';
		fwrite(buffer,1,r,fp);
	}
	fclose(fp);
	close(sockfd);

}

int main(int argc, char *argv[]) {
	char IP[15];
	strcpy(IP,"127.0.0.1");
	char filename[20];
	strcpy(filename,"lab2");
	char tmpfile[20];
	strcpy(tmpfile,"sample1.txt");
	int port=1508;
	uint64_t start=0;
	uint64_t finish1=90;
	uint64_t finish=175;
	downloadfile(IP,port,filename,start,finish1,tmpfile);
	downloadfile(IP,port,filename,finish1+1,finish,tmpfile);
	return 0;
}