#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <netdb.h>
//#include <netinet/in.h>
//#include <arpa/inet.h>
#include <sys/types.h>
#include <sys/socket.h>

int main(int argc, char** argv)
{
	char* default_ip = "localhost";
	char* default_port = "33333";
	const int MAX_LENGTH_DATA = 1024;
	struct addrinfo* server_info;
	struct addrinfo hints;
	struct sockaddr_in* address;
	struct sockaddr_storage connection_addr;
	socklen_t address_size;
	char ip[INET6_ADDRSTRLEN];
	int server_socket = -1;
	int connection_socket = -1;
	char buffer[1024];
	
	// Server start up.
	fprintf(stdout, "(Server | Information): Starting up...\n");
	//
	// Set initial server configuration.
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	//
	// Return address information for the specified hostname and service.
	if(3 == argc && 0 != getaddrinfo(argv[1], argv[2], &hints, &server_info))
	{
		fprintf(stderr, "(Server | Error): Could not get specified address information!\n");
		return 1;
	}
	//
	// Return address information for the default hostname and service.
	else if(3 > argc && 0 != getaddrinfo(default_ip, default_port, &hints, &server_info))
	{
		fprintf(stderr, "(Server | Error): Could not get default address information!\n");
		return 1;
	}
	//
	// Extract and print address and port information.
	address = (struct sockaddr_in*)server_info->ai_addr;
	inet_ntop(server_info->ai_family, &(address->sin_addr), ip, sizeof(ip));
	printf("IP: %s\n", ip);
	printf("Port: %hu\n", ntohs(address->sin_port));
	//
	// Get a server socket descriptor.
	if(0 > (server_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol)))
	{
		fprintf(stderr, "(Server | Error): Invalid server socket descriptor!\n");
		return 1;
	}
	//
	// Associate socket with port.
	if(-1 == bind(server_socket, server_info->ai_addr, server_info->ai_addrlen))
	{
		fprintf(stderr, "(Server | Error): Could not bind to socket!\n");
		return 1;
	}
	//
	// Finished with the server information structure.
	freeaddrinfo(server_info);
	//
	// Begin listening on the socket.
	listen(server_socket, 3);
	//
	// Server main loop.
	while(1)
	{
		// Get a connection socket descriptor or restart the loop.
		address_size = sizeof(connection_addr);
		if(-1 == (connection_socket = accept(server_socket, (struct sockaddr *)&connection_addr, &address_size)))
		{
			fprintf(stderr, "(Server | Error): Invalid connection socket descriptor!\n");
			continue;
		}
		fprintf(stdout, "(Server | Information): Accepted connection from client.\n");
		//
		// Get input.
		fprintf(stdout, "(Server | Prompt): Enter: ");
		fgets(buffer, MAX_LENGTH_DATA, stdin);
		//
		// Send data to client.
		if(-1 == send(connection_socket, buffer, MAX_LENGTH_DATA, 0))
		{
			fprintf(stderr, "(Server | Error): Could not send data to client!\n");
			return 1;
		}
		//
		// Close connection socket.
		close(connection_socket);
		//
	}
	//
	// Server shut down.
	close(server_socket);
	fprintf(stdout, "(Server | Information): Shutting down...\n");
	return 0;
	//
}
