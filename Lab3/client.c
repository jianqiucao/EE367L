/*
** client.c -- a stream socket client demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <netdb.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/socket.h>

#include <arpa/inet.h>

#define PORT "3532" // the port client will be connecting to 

#define MAXDATASIZE 1000 // max number of bytes we can get at once

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

int main(int argc, char *argv[])
{
	int sockfd, numbytes;  
	char buf[MAXDATASIZE + 1];
	struct addrinfo hints, *servinfo, *p;
	int rv;
	char s[INET6_ADDRSTRLEN];
	
	if (argc != 2) {
	    fprintf(stderr,"usage: client hostname\n");
	    exit(1);
	}

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	//while(1){
	if ((rv = getaddrinfo(argv[1], PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}
	
	while(1){
		char cmdline[MAXDATASIZE] = "\0";
		char *cmd_name, *file_name, *save_ptr;
		char cmd_send[MAXDATASIZE] = "\0";

		//fflush(stdin);
		printf("client: Command(type 'h' for help): ");
		//scanf("%[^\n]%*c", cmdline);
		fgets(cmdline, MAXDATASIZE, stdin);
		cmdline[strlen(cmdline) - 1] = '\0';
		//printf("client: entered command: '%s'\n", cmdline);
		
		
		cmd_name = strtok_r (cmdline, " ", &save_ptr);
		if (cmd_name == NULL){
			printf("client: invalid command!\n");
			continue; 
		}

		if (strcmp(cmd_name, "q") == 0 || strcmp(cmd_name, "Q") == 0) {		// command Quit
			break;
		} else if (strcmp(cmd_name, "l") == 0 || strcmp(cmd_name, "L") == 0) {	// command List
			strcpy(cmd_send, "l");
		} else if (strcmp(cmd_name, "c") == 0 || strcmp(cmd_name, "C") == 0 ||	// command Check
			strcmp(cmd_name, "p") == 0 || strcmp(cmd_name, "P") == 0){	// command Display 
			if (strcmp(cmd_name, "c") == 0 || strcmp(cmd_name, "C") == 0)
				strcpy(cmd_send, "c");
			else
				strcpy(cmd_send, "p");
			file_name = strtok_r(NULL, " ", &save_ptr);
			if (file_name == NULL){
				perror("client: invalid command!");
				continue;
			}
			strcat(cmd_send, " ");
			strcat(cmd_send, file_name);
		} else if (strcmp(cmd_name, "d") == 0 || strcmp(cmd_name, "D") == 0){	// command Download
			strcpy(cmd_send, "d");
			file_name = strtok_r(NULL, " ", &save_ptr);
			if (file_name == NULL){
				perror("client: invalid command!");
				continue;
			}
			if (access(file_name, F_OK) != -1){	// file exist
				char input[100] = "\0";
				printf("File '%s' exists. Overwrite?(y/n): ", file_name);
				fgets(input, MAXDATASIZE, stdin);
				input[strlen(input) - 1] = '\0';
			//	scanf("%s", input);
				while(strcmp(input, "y") != 0 && strcmp(input, "Y") != 0 &&
					strcmp(input, "n") != 0 && strcmp(input, "N") != 0){	// invalid input
					printf("Please enter 'y' or 'n': ");
					//scanf("%s", input);
					fgets(input, MAXDATASIZE, stdin);
					input[strlen(input) - 1] = '\0';
				}
				if (strcmp(input, "n") == 0 || strcmp(input, "N") == 0)
					continue;
			}
			strcat(cmd_send, " ");
			strcat(cmd_send, file_name);
		} else if (strcmp(cmdline, "h") == 0 || strcmp(cmdline, "H") == 0) {
			printf(" l: List\n c: Check <file name>\n p: Display <file name>\n d: Download <file name>\n q: Quit\n h: Help\n");
			continue;
		} else
			continue;

		//printf("client: command sent to server: '%s'\n", cmd_send);	
		// loop through all the results and connect to the first we can
		for(p = servinfo; p != NULL; p = p->ai_next) {
			if ((sockfd = socket(p->ai_family, p->ai_socktype,
					p->ai_protocol)) == -1) {
				perror("client: socket");
				continue;
			}

			if (connect(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
				close(sockfd);
				perror("client: connect");
				continue;
			}
	
			break;
		}

		if (p == NULL) {
			fprintf(stderr, "client: failed to connect\n");
			return 2;
		}

		inet_ntop(p->ai_family, get_in_addr((struct sockaddr *)p->ai_addr),
				s, sizeof s);
		printf("client: connecting to %s\n", s);
	
		//freeaddrinfo(servinfo); // all done with this structure
		
		if (send(sockfd, cmd_send, strlen(cmd_send), 0) == -1)
			perror("send");
	
		/*if ((numbytes = recv(sockfd, buf, MAXDATASIZE-1, 0)) == -1) {
	    		perror("recv");
	    		continue;
		}
		buf[numbytes] = '\0';
		*/
		if (strcmp(cmd_name, "d") == 0){	// command Download
			//scanf("%s", cmdline);
			//break;
			if ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) == -1) {
	    			perror("recv");
				close(sockfd);
	    			continue;
			}
			buf[numbytes] = '\0';

			if (buf[0] == 'y'){	// file exists in server
				printf("client: File '%s' exists in server\n", file_name);
				FILE *fp = fopen(file_name, "w");
				if (fp == NULL) {
					perror("file open");
					close(sockfd);
					continue;
				}
				while ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) != -1) {
					if (buf[numbytes - 1] == '\x04') {	// Last packet
						buf[numbytes - 1] = '\0';
						fputs(buf, fp);
						printf("client: File '%s' created\n", file_name);
						break;
					} else {
						buf[numbytes] = '\0';
						//printf("client: received '%s'\n",buf);
						fputs(buf, fp);
					}
				}
				if (numbytes == -1) {
					perror("recv");
				} 
				fclose(fp);
			} else {		// file doesn't exist in server
				printf("client: File '%s' not fount\n", file_name);
			}	
		} else {				// other commands
			printf("client: received '");

			while ((numbytes = recv(sockfd, buf, MAXDATASIZE, 0)) != -1) {
				if (buf[numbytes - 1] == '\x04') {	// Last byte
					buf[numbytes - 1] = '\0';
					printf("%s'\n", buf);
					break;
				} else {
					buf[numbytes] = '\0';
					printf("%s", buf);
				}
			}
			if (numbytes == -1) {
	    			perror("recv");
			}
							
		}
		//printf("client: received '%s'\n",buf);

		close(sockfd);
	}
	freeaddrinfo(servinfo); // all done with this structure
	return 0;
}

