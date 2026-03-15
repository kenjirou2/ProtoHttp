#include <stdio.h>
#include "protohttp.h"

int main()
{
	
	WSAIntilize();
	
	SOCKET sock;
	struct addrinfo *addr = NULL;
	sock = HttpOpenBridge("example.com", "80", &addr);
	if(sock == INVALID_SOCKET) { printf("Failed to open bridge\n"); return 1; }

	if(Httpconnect("example.com", 80, sock, addr) != 0) {
		printf("Connect failed\n");
		closesocket(sock);
		freeaddrinfo(addr);
		WSACleanup();
		return 1;
	}


	char request[1024];
	HttpbuildRequest("GET", "example.com", request, sizeof(request));
	printf("%s\n", Httpsend(sock, request));
	printf("Response:\n%s\n", Httprecv(sock));


	closesocket(sock);
	freeaddrinfo(addr);
	WSACleanup();
}