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
	int connection_socket;
	int data_socket;
	int result;
	int data;
	char buffer[BUFFER_SIZE];

	/* Remove the socket, if already exists */
	unlink(SOCKET_NAME);

	/* Create Master Socket */
	/* SOCK_DGRAM for Datagram based communication */
	connection_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if(connection_socket == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	printf("Master socket created\n");

	/* initialize the struct sockaddr_un members*/
	memset(&name, 0, sizeof(struct sockaddr_un));

	/* specify the socket credentials */
	name.sun_family = AF_UNIX;
	strncpy(name.sun_path, SOCKET_NAME, sizeof(name.sun_path) - 1);

	/* bind() system call */
	ret = bind(connection_socket, (const struct sockaddr *) &name, sizeof(struct sockaddr_un));
	if(ret == -1) {
		perror("bind");
		exit(EXIT_FAILURE);
	}

	printf("bind() call successfull\n");

	/* listen() system call */
	ret = listen(connection_socket, 20);
	if(ret == -1) {
		perror("listen");
		exit(EXIT_FAILURE);
	}

	while(1) {
		/* wait for incoming connection request */
		printf("waiting for accept() sys call\n");

		data_socket = accept(connection_socket, NULL, NULL);
		if(data_socket == -1) {
			perror("accept");
			exit(EXIT_FAILURE);
		}

		printf("Connection accepted from client\n");

		result = 0;
		while(1) {
			/* Prepare the buffer to recv the data */
			memset(buffer, 0, BUFFER_SIZE);

			/* wait for the next data packet to arrive */
			printf("waiting for data from client");
			ret = read(data_socket, buffer, BUFFER_SIZE);
			if(ret == -1) {
				perror("read");
				exit(EXIT_FAILURE);
			}

			/* Add received command */
			memcpy(&data, buffer, sizeof(int));
			if(data == 0) break;
			result += data;
		}

		/* send computed result */
		memset(buffer, 0, BUFFER_SIZE);
		sprintf(buffer, "Result: %d", result);

		printf("sending final result back to client\n");
		ret = write(data_socket, buffer, BUFFER_SIZE);
		if(ret == -1) {
			perror("write");
			exit(EXIT_FAILURE);
		}

		/* close data socket */
		close(data_socket);
	}

	/* close the connection socket */
	close(connection_socket);
	printf("connection closed. Good Bye!\n");

	/* Server should release resources before it gets terminated.
	 * Unlink the socket. */
	unlink(SOCKET_NAME);
	exit(EXIT_SUCCESS);
}
