#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

int main(int argc, char *argv[]) {
	
	struct sockaddr_un name;
#if 0
	struct sockaddr_un {
		sa_family_t sun_family;		/* AF_UNIX */
		char	    sun_path[108];	/* pathname */
	};	
#endif
	int ret;
	int data_socket;
	int result;
	int data;
	char buffer[BUFFER_SIZE];

	/* Create communication Socket */
	/* SOCK_DGRAM for Datagram based communication */
	data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if(connection_socket == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	printf("communication socket created\n");

	/* initialize the struct sockaddr_un members*/
	memset(&name, 0, sizeof(struct sockaddr_un));

	/* specify the socket credentials */
	name.sun_family = AF_UNIX;
	strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);
	
	ret = connect(data_socket, (const struct sockaddr *) &addr, sizeof(struct sockaddr_un));
	if(ret == -1) {
		fprintf(stderr, "The server is down");
		exit(EXIT_FAILURE);
	}

	/* Send Arguments */
}
