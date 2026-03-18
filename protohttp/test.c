#include <stdio.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include "protohttp.h"
#include <openssl/ssl.h>
#include <openssl/err.h>

int main()
{
    SSLInit();          // Initialize OpenSSL
    WSAIntilize();      // Initialize Winsock

    const char* type = "GET";

    char request[512];
    HttpbuildRequest(type, "example.com", request, sizeof(request));

    struct addrinfo* rslt;
    SOCKET ConnectSocket = HttpOpenBridge("example.com", "443", &rslt);
    Httpconnect("example.com", 443, ConnectSocket, rslt);

    SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());
    if (!ctx) { ERR_print_errors_fp(stderr); return 1; }

    SSL* ssl = SSL_new(ctx);
    SSL_set_fd(ssl, ConnectSocket);

    if (SSL_connect(ssl) <= 0) {
        ERR_print_errors_fp(stderr);
        SSL_free(ssl);
        SSL_CTX_free(ctx);
        closesocket(ConnectSocket);
        return 1;
    }


    HttpsendSSL(ssl, request);


    char* recved = HttprecvFullSSL(ssl);


    HTTPRESPONSE result = httparse(recved);
    printf("\n\nStatus: %d, Text: %s\n", result.status, result.status_text);


    SSL_shutdown(ssl);
    SSL_free(ssl);
    SSL_CTX_free(ctx);
    closesocket(ConnectSocket);
    WSACleanup();

    if (recved)
	{
		free(recved);
	}

    return 0;
}