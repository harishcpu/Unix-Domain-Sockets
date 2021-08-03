#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#define SOCKET_NAME "/tmp/DemoSocket"
#define BUFFER_SIZE 128

int main(int argc, char *argv[]) {
	
	struct sockaddr_un addr;
#if 0
	struct sockaddr_un {
		sa_family_t sun_family;		/* AF_UNIX */
		char	    sun_path[108];	/* pathname */
	};	
#endif
	int ret;
	int data_socket;
	int data;
	char buffer[BUFFER_SIZE];

	/* Create communication Socket */
	/* SOCK_DGRAM for Datagram based communication */
	data_socket = socket(AF_UNIX, SOCK_STREAM, 0);
	if(data_socket == -1) {
		perror("socket");
		exit(EXIT_FAILURE);
	}

	printf("communication socket created\n");

	/* initialize the struct sockaddr_un members*/
	memset(&addr, 0, sizeof(struct sockaddr_un));

	/* specify the socket credentials */
	addr.sun_family = AF_UNIX;
	strncpy(addr.sun_path, SOCKET_NAME, sizeof(addr.sun_path) - 1);
	
	ret = connect(data_socket, (const struct sockaddr *) &addr, sizeof(struct sockaddr_un));
	if(ret == -1) {
		fprintf(stderr, "The server is down");
		exit(EXIT_FAILURE);
	}

	/* Send Arguments */
	do {
		printf("Enter number to send to server: ");
		scanf("%d", &data);
		ret = write(data_socket, &data, sizeof(int));
		if(ret == -1) {
			perror("write");
			break;
		}
		printf("No of bytes sent = %d, data sent = %d\n", ret, data);
	} while(data);

	/* Request result */
	memset(buffer, 0, BUFFER_SIZE);
	ret = read(data_socket, buffer, BUFFER_SIZE);
	if(ret == -1) {
		perror("read");
		exit(EXIT_FAILURE);
	}
	printf("Received from Server: %s\n", buffer);

	/* close the socket */
	close(data_socket);
	exit(EXIT_SUCCESS);
}
