#include "protohttp.h"  // your header file with all the functions

int main() {

    // Initialize WinSock
    if (WSAIntilize() != 0) return 1;

    // Initialize OpenSSL
    OpenSSLIntilize();

    // Create TLS context
    SSL_CTX* ctx = TLSContext();
    if (!ctx) return 1;

    struct addrinfo* result = NULL;

    // Open a socket connection
    SOCKET sock = HttpOpenBridge("www.example.com", tlsdefaultport, &result);
    if (sock == 1) return 1; // failed

    // Connect to server
    if (Httpconnect("www.example.com", 443, sock, result) != 0) return 1;

    // Wrap socket in SSL, now passing the host!
    SSL* ssl = WrapSocketTLS(ctx, sock, "www.example.com");
    if (!ssl) return 1;

    // Build HTTP GET request
    char request[1024];
    HttpbuildRequest("GET", "www.example.com", request, sizeof(request));

    // Send request
    printf("%s\n", HttpsendSSL(ssl, request));

    // Receive full response
    char* response = HttprecvFullSSL(ssl);

    HTTPRESPONSE res = httparse(response);

    // Cleanup
    CloseTLS(ssl, ctx, sock);
    WSACleanup();

    return 0;

}