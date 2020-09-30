#ifndef HEADERS_H_
#define HEADERS_H_
#include<stdio.h>
#include<errno.h>
#include<unistd.h>
#include<netdb.h>
#include<arpa/inet.h>
#include<sys/wait.h>
#include<stdlib.h>
#include<string.h>
#include<netinet/in.h>
#include<sys/socket.h>
#include<netdb.h>
#include<sys/types.h>
#include<signal.h>
#include <time.h>
#endif
#include <time.h>





#define MAXDATASIZE 100
void *get_in_addr(struct sockaddr *sa) { // Function to get input socket address
 if (sa->sa_family == AF_INET) {
 return &(((struct sockaddr_in *) sa)->sin_addr);
 }
 return &(((struct sockaddr_in6 *) sa)->sin6_addr);
}
int writen(int sockfd, char *buf) {
 int numbytes;  // variable to store the total bytes
 while ((numbytes = send(sockfd, buf, MAXDATASIZE - 1, 0)) == -1 && errno ==
EINTR) {
 // manually restarting
 continue;
 }


 return numbytes;  //it returns numbytes from the function
}


int readline(int sockfd, char *recvbuf) { // readline from the socket
 int numbytes;
 while ((numbytes = recv(sockfd, recvbuf, MAXDATASIZE - 1, 0)) == -1 && errno
== EINTR) {
 // manually restarting here
 }
 return numbytes;
}
int server_lookup_connect(char *host, char *server_port) {   //this function looks up to connect
 struct addrinfo hints, *server_info, *p;
 int status;
 int sock_fd;
 memset(&hints, 0, sizeof(hints));
 hints.ai_family = AF_UNSPEC;
 hints.ai_socktype = SOCK_STREAM;
 // hints.ai_flags = AI_PASSIVE; // fill in my IP
 // argv[1]: IPAdr
 // argv[2]: Port
 if ((status = getaddrinfo(host, server_port, &hints, &server_info)) != 0) {
 fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));    // print statement to get the address info 
 return 2;
 }
 for (p = server_info; p != NULL; p = p->ai_next) { //loop through link list
 sock_fd = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
 if (sock_fd == -1) { //socket creation failed
 perror("client: socket");
 continue;
 }
 if (connect(sock_fd, p->ai_addr, p->ai_addrlen) == -1) { //connection failed
 close(sock_fd);
 perror("client: connect"); //client connection
 continue;
 }
 break;
 }
 if (p == NULL) {
 fprintf(stderr, "client: failed to connect\n");
 return 2;
 }
 printf("client: connected to %s:%s\n", host, server_port);
 freeaddrinfo(server_info);
 return sock_fd;
}


int main(int argc, char *argv[]) {  // Main function 
 int sock_fd;
time_t local_time; 
int numbytes_send, numbytes_recv;
 char buf[MAXDATASIZE], recvbuf[MAXDATASIZE];
 char *host, *server_port;
 if (argc == 3) {
 host = argv[1];
 server_port = argv[2];
 printf("Echo Service starting \n");
 printf("client: client started\n");
 } else {
 fprintf(stderr, "usage: echo IPAdr Port\n");
 exit(1);
 }
 sock_fd = server_lookup_connect(host, server_port);
 // reading off the stdin
 while (fgets(buf, MAXDATASIZE, stdin)) {
 numbytes_send = writen(sock_fd, buf);
 if (numbytes_send == -1) { //writen and error handling
 perror("send");


 exit(1);
 }
 numbytes_recv = readline(sock_fd, recvbuf); //numbytes received
 if (numbytes_recv == -1) {//readline and error handling
 perror("recv");
 exit(1);
 }
 local_time = time(NULL);                                       //successful write and receive echo
printf(" Client time is %s \n",asctime(localtime(&local_time)));
printf("%s\n", recvbuf);
 int comparison;
 comparison = strcmp(buf, recvbuf);
 if (strcmp(buf, recvbuf) != 0) { //buffer not matching
 if (strncmp(buf, recvbuf, numbytes_recv) == 0) { // recv string is a substring of send string
 printf(" !recv buffer out of space.\n");
 } else { //send string does not match recv string
 perror(" !client: recv string did not match");
 exit(1);
 }
 }
 }
 printf("Closing the socket \n"); // buf[numbytes] = '\0';
 close(sock_fd);
 return 0;
}

