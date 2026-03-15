
#ifndef PROTOHTTP_H
#define PROTOHTTP_H
#define defultport 80

#include <stdio.h>
#include <winsoch.h>

typedef enum {

    GET,
    POST,
    PUT,
    DELETE,
	UNKNOWN,

} REQUEST;



REQUEST Httpbuild(const char *type, const char* HOST)
{

	const char request;
	
	if (strcmp(type, "GET") == 0) {return GET;}
	if (strcmp(type, "POST") == 0) {return POST;}
	if (strcmp(type, "PUT") == 0) {return PUT;}
	if (strcmp(type, "DELETE") == 0) {return DELETE;}

	else {return UNKNOWN;}

	request = {
				"%s, / HTTP/1.1\r\n",
				"Host: %s\r\n",
				"Connection: close\r\n\r\n",
				type, HOST
	}

}

int HttpOpenBridge(const char *URL, const int *port)
{

	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo socinfo;

	SOCKET ConnectSocket = INVALID_SOCKET;
	int res;

	ZeroMemory( &hints, sizeof(hints) );
	socinfo.ai_family   = AF_UNSPEC;
	socinfo.ai_socktype = SOCK_STREAM;
	socinfo.ai_protocol = IPPROTO_TCP;

	if (port != defultport){return 1;}

	res = getaddrinfo(URL, port, &socinfo, &result);
	if (res != 0)
	{
		printf("In func::HttpOpenBridge::Getaddrinfo failed::: %d\n", res);
		WSACleanup();
		return 1;
	}

	ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("In func::HttpOpenBridge::failed to create socket::: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	return 0;

}

int Httpconnect(const char *HOST, const int *port)
{


	

}

char *Httpsend()
{


}

char *Httprec()
{


}




 #endif