#include<fstream>
#include<iostream>
#include<string.h>
#include<stdio.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h> 

#define CHUNK_SIZE 64000
#define PORT 3456
#define SERVER_IP "127.0.0.1"

using namespace std;

void sendFile(int sockfd, string fileName);

int main(){
	int sockfd, connfd;
	struct sockaddr_in servaddr, cli;
    string fileName;

	sockfd = socket(AF_INET, SOCK_STREAM, 0);

    bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(SERVER_IP);
	servaddr.sin_port = htons(PORT);

    if (connect(sockfd, (struct sockaddr*)&servaddr, sizeof(servaddr))!= 0) {
		printf("connection with the server failed...\n");
		exit(0);
	}
    cout<<"Enter File Name:  ";
    cin>>fileName;

    sendFile(sockfd, fileName);
	// close the socket
	close(sockfd);

}

void sendFile(int sockfd, string fileName){
    int fileSize;
    int n_chunks;
    int n_bytes;
    int offset=0;
    int count=0;
    char buffer[CHUNK_SIZE];

    fstream f(fileName, ios::in|ios::binary);

    //get file size
    f.seekg(0, ios::end);
    fileSize=f.tellg();
    f.seekg(0, ios::beg);
    n_chunks=fileSize/CHUNK_SIZE+1;

    //send file size
    send(sockfd, &fileSize, sizeof(fileSize), 0);

    n_chunks=fileSize/CHUNK_SIZE+1;
    int size_to_read;
    for(int i=0;i<n_chunks;i++){
        offset=0;
        count=0;
        if (i!=n_chunks-1)
            size_to_read=CHUNK_SIZE;
        else
            size_to_read=fileSize%CHUNK_SIZE;
        f.read(buffer, size_to_read);
        n_bytes=size_to_read;
        while (n_bytes>0){
            count=send(sockfd, buffer+offset, n_bytes, 0);
            if (count>0){
                offset+=count;
                n_bytes-=count;
                //cout<<"Buffer Sent";
            }
        }
    }

    f.close();
    //sleep(5);
    recv(sockfd, buffer, 2, 0);
}