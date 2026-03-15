#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "protohttp.h"

int main()
{
	char request[1024];
	struct addrinfo *result;

	SOCKET s = HttpOpenBridge("example.com", "80", &result);

	Httpconnect(s, result);

	HttpBuildRequest(request, "GET", "example.com");

	Httpsend(s, request);

	Httprecv(s);
}