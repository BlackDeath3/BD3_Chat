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
	struct addrinfo* info;
	struct addrinfo hints;
	struct sockaddr_in* address;
	char ip[INET6_ADDRSTRLEN];
	int i = 0;
	
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags = AI_PASSIVE;
	memset(&hints, 0, sizeof(hints));
	if(0 != getaddrinfo(argv[1], "800000", &hints, &info))
	{
		fprintf(stdout, "ERROR!\n");
		return 1;
	}
	address = (struct sockaddr_in*)info->ai_addr;
	inet_ntop(AF_INET, &(address->sin_addr), ip, sizeof(ip));
	printf("IP: %s\n", ip);
	printf("Port: %hu\n", ntohs(address->sin_port));

	return 0;
}
