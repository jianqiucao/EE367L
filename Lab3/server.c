/*
** server.c -- a stream socket server demo
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>

#define PORT "3532"  // the port users will be connecting to
#define MAXDATASIZE 1000
#define BACKLOG 10	 // how many pending connections queue will hold

void sigchld_handler(int s)
{
	while(waitpid(-1, NULL, WNOHANG) > 0);
}

// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa)
{
	if (sa->sa_family == AF_INET) {
		return &(((struct sockaddr_in*)sa)->sin_addr);
	}

	return &(((struct sockaddr_in6*)sa)->sin6_addr);
}

// execute a command, and send the output to the client
int exec_send(char **paramList, char *buf, int new_fd)
{
	int out[2];
	int n;
	if (pipe(out) < 0) perror("pipe out"); 

	if (!fork()) { // this is the grandchild process
		close(1);
		dup2(out[1], 1);
		close(out[0]);
				
		execv(*paramList, paramList);
		//execl("/usr/bin/ls", "ls", (char *)NULL);
		perror("Could not exec the command");
		return -1;
	}
	close(out[1]);
	while ((n = read(out[0], buf, MAXDATASIZE)) == MAXDATASIZE) {
		buf[n] = 0;
		if (send(new_fd, buf, strlen(buf), 0) == -1) {
			perror("send");
			return -1;
		}
		//printf("%s", buf);
	}
	buf[n] = '\x04';
	buf[n + 1] = 0;
	if (send(new_fd, buf, strlen(buf), 0) == -1) {
		perror("send");
		return -1;
	}
	//printf("%s", buf);
	return 0;
}
int main(void)
{
	int sockfd, new_fd;  // listen on sock_fd, new connection on new_fd
	struct addrinfo hints, *servinfo, *p;
	struct sockaddr_storage their_addr; // connector's address information
	socklen_t sin_size;
	struct sigaction sa;
	int yes=1;
	char s[INET6_ADDRSTRLEN];
	int rv;

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE; // use my IP

	if ((rv = getaddrinfo(NULL, PORT, &hints, &servinfo)) != 0) {
		fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(rv));
		return 1;
	}

	// loop through all the results and bind to the first we can
	for(p = servinfo; p != NULL; p = p->ai_next) {
		if ((sockfd = socket(p->ai_family, p->ai_socktype,
				p->ai_protocol)) == -1) {
			perror("server: socket");
			continue;
		}

		if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes,
				sizeof(int)) == -1) {
			perror("setsockopt");
			exit(1);
		}

		if (bind(sockfd, p->ai_addr, p->ai_addrlen) == -1) {
			close(sockfd);
			perror("server: bind");
			continue;
		}

		break;
	}

	if (p == NULL)  {
		fprintf(stderr, "server: failed to bind\n");
		return 2;
	}

	freeaddrinfo(servinfo); // all done with this structure

	if (listen(sockfd, BACKLOG) == -1) {
		perror("listen");
		exit(1);
	}

	sa.sa_handler = sigchld_handler; // reap all dead processes
	sigemptyset(&sa.sa_mask);
	sa.sa_flags = SA_RESTART;
	if (sigaction(SIGCHLD, &sa, NULL) == -1) {
		perror("sigaction");
		exit(1);
	}

	printf("server: waiting for connections...\n");

	while(1) {  // main accept() loop
		sin_size = sizeof their_addr;
		new_fd = accept(sockfd, (struct sockaddr *)&their_addr, &sin_size);
		if (new_fd == -1) {
			perror("accept");
			continue;
		}

		inet_ntop(their_addr.ss_family,
			get_in_addr((struct sockaddr *)&their_addr),
			s, sizeof s);
		printf("server: got connection from %s\n", s);

		char cmd[MAXDATASIZE];
		int numbytes;
		if ((numbytes = recv(new_fd, cmd, MAXDATASIZE, 0)) == -1) {
                	perror("recv");
                	continue;
                }
		cmd[numbytes] = 0;
		//printf("server: command received: '%s'\n", cmd);

		if (!fork()) { // this is the child process
			close(sockfd); // child doesn't need the listener
			
			char buf[MAXDATASIZE + 1];
			//int n;
			char *token, *save_ptr;

			token = strtok_r (cmd, " ", &save_ptr);
			if (strcmp(token, "l") == 0){
				/*int out[2];
				
				if (pipe(out) < 0) perror("pipe out"); 

				if (!fork()) { // this is the grandchild process
					close(1);
					dup2(out[1], 1);
					close(out[0]);
				
					execl("/usr/bin/ls", "ls", (char *)NULL);
					perror("Could not exec 'ls'");
				}
				close(out[1]);
				n = read(out[0], buf, 1000);
				buf[n] = 0;*/
				char *paramList[2];
				paramList[0] = "/usr/bin/ls";
				paramList[1] = NULL;
				exec_send(paramList, buf, new_fd);
				
				//if (send(new_fd, buf, strlen(buf), 0) == -1)
				//	perror("send");

			} else if (strcmp(token, "c") == 0){
				token = strtok_r(NULL, " ", &save_ptr);	// file name
				//printf("server: file name: '%s'\n", token);
				if( access( token, F_OK ) != -1 ) {	// file exists
					strcpy(buf, "File '");
					strcat(buf, token);
					strcat(buf, "' exists\x04");
				}
				else {
					strcpy(buf, "File '");
					strcat(buf, token);
					strcat(buf, "' not found\x04");
				}
				if (send(new_fd, buf, strlen(buf), 0) == -1)
					perror("send");

			} else if (strcmp(token, "p") == 0){	// Display
				token = strtok_r(NULL, " ", &save_ptr);	// file name
				if( access( token, F_OK ) != -1 ) {	// file exists
					/*strcpy(buf, "File '");
					strcat(buf, token);
					strcat(buf, "' exists");
					n = strlen(buf);*/
					char *paramList[3]; 
					paramList[0] = "/usr/bin/cat";
					paramList[1] = token;
					paramList[2] = NULL;
					exec_send(paramList, buf, new_fd);
				}
				else {
					strcpy(buf, "File '");
					strcat(buf, token);
					strcat(buf, "' not found\x04");
					if (send(new_fd, buf, strlen(buf), 0) == -1)
						perror("send");
				}
			} else if (strcmp(token, "d") == 0) {	// Download
				token = strtok_r(NULL, " ", &save_ptr);	// file name
				if( access( token, F_OK ) != -1 ) {	// file exists
					if (send(new_fd, "y", 2, 0) == -1) {
						perror("send");
					} else {
						char *paramList[3]; 
						paramList[0] = "/usr/bin/cat";
						paramList[1] = token;
						paramList[2] = NULL;
						exec_send(paramList, buf, new_fd);
					}
				}
				else {
					strcpy(buf, "n");
					if (send(new_fd, buf, strlen(buf), 0) == -1)
						perror("send");
				}
			} else{
				strcpy(buf, "Invalid command\x04");
				if (send(new_fd, buf, strlen(buf), 0) == -1)
					perror("send");

			}
			
			close(new_fd);
			exit(0);
		}	// child process
		close(new_fd);  // parent doesn't need this
	}

	return 0;
}

