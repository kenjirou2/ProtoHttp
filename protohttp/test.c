#include <stdio.h>
#include "protohttp.h"

int main()
{
	const char* type = "GET";
	WSAIntilize();
	Httpbuild(type);
	
	char request[512];
	size_t size = sizeof(request);
	HttpbuildRequest(type, "example.com", request, size);
	
	struct addrinfo *rslt;
	
	SOCKET ConnectSocket = HttpOpenBridge("example.com", "80", &rslt);
	Httpconnect("example.com", 80, ConnectSocket, rslt);
	
	Httpsend(ConnectSocket, request);
	
	char* recved = HttprecvFull(ConnectSocket);
	HTTPRESPONSE result = httparse(recved);
	printf("\n\n\nStatus: %d, Text: %s\n", result.status, result.status_text);
	
	
	return 0;
	
}