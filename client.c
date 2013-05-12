#include <stdio.h>
#include <stdlib.h>
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
	struct addrinfo* client_info;
	struct addrinfo hints;
	struct sockaddr_in* address;
	char ip[INET6_ADDRSTRLEN];
	int client_socket = -1;
	char buffer[1024];
	
	// Client start up.
	fprintf(stdout, "(Client | Information): Starting up...\n");
	//
	// Set initial client configuration.
	memset(&hints, 0, sizeof(hints));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	//
	// Return address information for the specified hostname and service.
	if(3 == argc && 0 != getaddrinfo(argv[1], argv[2], &hints, &client_info))
	{
		fprintf(stdout, "(Client | Error): Could not get specified address information!\n");
		return 1;
	}
	//
	// Return address information for the default hostname and service.
	if(3 > argc && 0 != getaddrinfo(default_ip, default_port, &hints, &client_info))
	{
		fprintf(stdout, "(Client | Error): Could not get specified address information!\n");
		return 1;
	}
	//
	// Extract and print address and port information.
	address = (struct sockaddr_in*)client_info->ai_addr;
	inet_ntop(AF_INET, &(address->sin_addr), ip, sizeof(ip));
	printf("IP: %s\n", ip);
	printf("Port: %hu\n", ntohs(address->sin_port));
	//
	// Get a client socket descriptor.
	if(0 > (client_socket = socket(client_info->ai_family, client_info->ai_socktype, client_info->ai_protocol)))
	{
		fprintf(stderr, "(Client | Error): Invalid server socket descriptor!\n");
		return 1;
	}
	// Connect to the remote host.
	if(-1 == connect(client_socket, client_info->ai_addr, client_info->ai_addrlen))
	{
		fprintf(stderr, "(Client | Error): Could not connect to remote host!\n");
		return 1;
	}
	//
	// Client main loop.
	while(1)
	{
		// Receive data from server.
		if(-1 == recv(client_socket, buffer, MAX_LENGTH_DATA, 0))
		{
			fprintf(stderr, "(Client | Error): Could not receive data from server!\n");
			return 1;
		}
		fprintf(stdout, buffer);
		//
		break;
	}
	// Client shut down.
	close(client_socket);
	fprintf(stdout, "(Client | Information): Shutting down...\n");
	return 0;
	//
}
