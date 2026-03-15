
#ifndef PROTOHTTP_H
#define PROTOHTTP_H
#define defaultport "80"

#include <stdio.h>
#include <winsock.h>

typedef enum {

    GET,
    POST,
    PUT,
    DELETE,
	UNKNOWN,

} REQUEST;



REQUEST Httpbuild(const char *type, const char* HOST)
{

	const char request[1024];
	
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

	ConnectSocket = socket(AF_INET, SOCK_STREAM, 0);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("In func::HttpOpenBridge::failed to create socket::: %ld\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	return 0;



}

int Httpconnect(const char *HOST, const int port, const SOCKET soc, struct addrinfo *result)
{

	int res;

	res = connect(soc, result->ai_addr, (int)result->ai_addrlen);
	if (res == SOCKET_ERROR)
	{
		printf("In func::Httpconnect::failed to connect to server:::");
		closesocket(soc);
		soc = INVALID_SOCKET;
	}

	freeaddrinfo(result);

	return 0;

}

char *Httpsend(const SOCKET soc, const char RequestType[1024])
{

	char buff[4096];
	int sendres;

	sendres = send(soc, RequestType, (int)strlen(RequestType), 0);

	if (sendres != SOCKET_ERROR)
	{
		return "\nrequest sent";
	}


}

char *Httprecv(const SOCKET soc)
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