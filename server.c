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
	struct addrinfo* server_info;
	struct addrinfo hints;
	struct sockaddr_in* address;
	char ip[INET6_ADDRSTRLEN];
	int i = 0;
	int server_socket = -1;
	int connected_socket = -1;
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	memset(&hints, 0, sizeof(hints));
	if(3 == argc && 0 != getaddrinfo(argv[1], argv[2], &hints, &server_info))
	{
		fprintf(stderr, "ERROR (Server): Could not get give address information!\n");
		return 1;
	}
	else if(3 > argc && 0 != getaddrinfo(default_ip, default_port, &hints, &server_info))
	{
		fprintf(stderr, "ERROR (Server): Could not get default address information!\n");
		return 1;
	}
	address = (struct sockaddr_in*)server_info->ai_addr;
	inet_ntop(server_info->ai_family, &(address->sin_addr), ip, sizeof(ip));
	printf("IP: %s\n", ip);
	printf("Port: %hu\n", ntohs(address->sin_port));
	if(0 > (server_socket = socket(server_info->ai_family, server_info->ai_socktype, server_info->ai_protocol)))
	{
		fprintf(stderr, "ERROR (Server): Invalid socket descriptor!\n");
		return 1;
	}
	if(-1 == bind(server_socket, server_info->ai_addr, server_info->ai_addrlen))
	{
		fprintf(stderr, "ERROR (Server): Could not bind to socket!\n");
		return 1;
	}
	freeaddrinfo(server_info);
	listen(server_socket, 3);
	while(1)
	{
/*		connection_socket = accept(server_socket, */
	}

	return 0;
}
