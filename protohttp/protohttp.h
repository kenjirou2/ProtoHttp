
#ifndef PROTOHTTP_H
#define PROTOHTTP_H

#define defaultport "80"

#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <string.h>

typedef enum {

    GET,
    POST,
    PUT,
    DELETE_,
	UNKNOWN,

} REQUEST;


int WSAIntilize()
{
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
		printf("WSAStartup failed\n");
		return 1;
	}
	
}
REQUEST Httpbuild(const char *type, REQUEST rtype)
{


	if (strcmp(type, "GET") == 0){
		rtype = GET;
		return rtype;
	}

	if (strcmp(type, "POST") == 0) {
		rtype = POST;
		return rtype;
	}

	if (strcmp(type, "PUT") == 0){
		rtype = PUT;
		return rtype;
	}

	if (strcmp(type, "DELETE") == 0){
		rtype = DELETE;
		return rtype;
	}

	else {;;;}


}

void HttpbuildRequest(const char *rtype, const char *HOST, char *request, size_t sizeb)
{


    snprintf(request, sizeb,
        "%s / HTTP/1.1\r\n"
        "Host: %s\r\n"
        "Connection: close\r\n"
        "\r\n",
        rtype, HOST);



}

int HttpOpenBridge(const char *HOST, const char *port, struct addrinfo **rslt)
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

	*rslt = result;
	return ConnectSocket;



}

int Httpconnect(const char *HOST, const int port, const SOCKET soc, struct addrinfo *rslt)
{

	int res;

	res = connect(soc, rslt->ai_addr, (int)rslt->ai_addrlen);
	if (res == SOCKET_ERROR)
	{
		printf("In func::Httpconnect::failed to connect to server:::");
		freeaddrinfo(rslt);
		WSACleanup();
		closesocket(soc);
		return 1;
	}

	freeaddrinfo(rslt);

	return 0;

}

const char* Httpsend(const SOCKET soc, const char *RequestType)
{

    int sendres = send(soc, RequestType, (int)strlen(RequestType), 0);

    if (sendres != SOCKET_ERROR){ return "request sent"; }


    else { return "send failed"; }



}

const char* Httprecv(const SOCKET soc)
{

    static char recvbuff[4096];
    int recvr = recv(soc, recvbuff, sizeof(recvbuff)-1, 0);

    if(recvr > 0)
    {
        recvbuff[recvr] = '\0';
        return recvbuff;
    }
    else if(recvr == 0)
    {
        return "connection closed by server";
    }

    else
    {
        int err = WSAGetLastError();
        static char errbuf[64];
        snprintf(errbuf, sizeof(errbuf), "recv failed: %d", err);
        return errbuf;
    }

}


 #endif