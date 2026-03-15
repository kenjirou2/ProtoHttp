
#ifndef PROTOHTTP_H
#define PROTOHTTP_H
#define defaultport "80"

#include <stdio.h>
#include <winsock.h>
#include <ws2tcpip.h>
#include <string.h>

typedef enum {

    GET,
    POST,
    PUT,
    rDELETE,
	UNKNOWN,

} REQUEST;

REQUEST Httpbuild(const char *type)
{
	
	
    if (strcmp(type, "GET") == 0) return GET;
    if (strcmp(type, "POST") == 0) return POST;
    if (strcmp(type, "PUT") == 0) return PUT;
    if (strcmp(type, "DELETE") == 0) return rDELETE;


    return UNKNOWN;
	
	
}

void HttpBuildRequest(char *request, const char *type, const char *HOST)
{
	
    sprintf(request,
        "%s / HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n\r\n",
        type, HOST);
		
		
}

int HttpOpenBridge(const char *HOST, const char *port)
{

	struct addrinfo *result = NULL;
	struct addrinfo *ptr = NULL;
	struct addrinfo socinfo;

	SOCKET ConnectSocket = INVALID_SOCKET;
	int res;

	ZeroMemory( &socinfo, sizeof(socinfo) );
	socinfo.ai_family   = AF_UNSPEC;
	socinfo.ai_socktype = SOCK_STREAM;
	socinfo.ai_protocol = IPPROTO_TCP;

	if (strcmp(port, defaultport) != 0)
	{
		printf("In func::HttpOpenBridge::port asigned should be 80");
		return 1;
	}

	res = getaddrinfo(HOST, port, &socinfo, &result);
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
	
	
	return ConnectSocket;
	return 0;


}

int Httpconnect(SOCKET soc, struct addrinfo *result)
{
	
	
    int res = connect(soc, result->ai_addr, (int)result->ai_addrlen);
    if (res == SOCKET_ERROR)
    {
        printf("connect failed\n");
        closesocket(soc);
        return 1;
    }



    return 0;
	
	
}

int Httpsend(SOCKET soc, const char *request)
{
    int res = send(soc, request, strlen(request), 0);

    if (res != SOCKET_ERROR)
	{
		return res;
	}


    return 1;
	
}

int Httprecv(const SOCKET soc)
{

	char buff[4096];
	int recvres;

	recvres = recv(soc, buff, 4096, 0);

	if (recvres > 0)
	{
		return buff;
	}
	else
	{
		return "\nrecv failed";
	}



}



 #endif