#include "protohttp.h"


void OpenSSLIntilize(void)
{

	SSL_library_init();
	SSL_load_error_strings();
	OpenSSL_add_all_algorithms();

}

SSL_CTX* SSLCTX(void)
{

	SSL_CTX* ctx = SSL_CTX_new(TLS_client_method());

	if (ctx == NULL) {
		fprintf(stderr, "\nSSL context creation failed");
		return NULL;
	}

	return ctx;

}

int WSAIntilize(void)
{
	WSADATA wsa;
	if (WSAStartup(0x0032, &wsa) != 0) {
		fprintf(stderr, "\nWSAStartup failed\n");
		return 1;
	}
	return 0;
}

REQUEST Httpbuild(const char* type)
{

	if (strcmp(type, "GET") == 0) { return GET; }

	if (strcmp(type, "POST") == 0) { return POST; }

	if (strcmp(type, "PUT") == 0) { return PUT; }

	if (strcmp(type, "DELETE_") == 0) { return DELETE_; }

	return UNKNOWN;

}

void HttpbuildRequest(const char* Type, const char* HOST, char* request, size_t sizeb)
{
	snprintf(request, sizeb,
		"%s / HTTP/1.1\r\n"
		"Host: %s\r\n"
		"Connection: close\r\n"
		"\r\n",
		Type, HOST);
}


SOCKET HttpOpenBridge(const char* HOST, const char* port, struct addrinfo** rslt)
{

	struct addrinfo* result = NULL;
	struct addrinfo socinfo;

	SOCKET ConnectSocket = INVALID_SOCKET;

	SecureZeroMemory(&socinfo, sizeof(socinfo));
	socinfo.ai_family = AF_UNSPEC;
	socinfo.ai_socktype = SOCK_STREAM;
	socinfo.ai_protocol = IPPROTO_TCP;

	if (strcmp(port, DEFAULT_PORT) != 0 && strcmp(port, TLS_DEFAULT_PORT) != 0)
	{
		printf("In func::HttpOpenBridge::port asigned should be 80 or 443");
		return 1;
	}

	int res = getaddrinfo(HOST, port, &socinfo, &result);
	if (res != 0)
	{
		printf("In func::HttpOpenBridge::Getaddrinfo failed::: %d\n", res);
		WSACleanup();
		return 1;
	}

	ConnectSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
	if (ConnectSocket == INVALID_SOCKET)
	{
		printf("In func::HttpOpenBridge::failed to create socket::: %d\n", WSAGetLastError());
		freeaddrinfo(result);
		WSACleanup();
		return 1;
	}

	*rslt = result;
	return ConnectSocket;

}

SSL* WrapSocketTLS(SSL_CTX* ctx, SOCKET sock, const char* HOST)
{

	SSL* ssl = SSL_new(ctx);
	if (ssl == NULL)
	{
		fprintf(stderr, "\n failed to create SSl object");
		return NULL;
	}

	SSL_set_fd(ssl, (int)sock);
	SSL_set_tlsext_host_name(ssl, HOST);

	if (SSL_connect(ssl) <= 0) {
		ERR_print_errors_fp(stderr);
		SSL_free(ssl);
		return NULL;
	}

	return ssl;

}

void CloseTLS(SSL* ssl, SSL_CTX* ctx, SOCKET sock)
{

	if (ssl) SSL_shutdown(ssl);
	if (ssl) SSL_free(ssl);
	if (ctx) SSL_CTX_free(ctx);

	closesocket(sock);
	return;

}

int HttpConnect(const SOCKET soc, struct addrinfo* rslt)
{

	int res = connect(soc, rslt->ai_addr, (int)rslt->ai_addrlen);
	if (res == SOCKET_ERROR)
	{
		fprintf(stderr, "\nfailed to connect to server");
		freeaddrinfo(rslt);
		WSACleanup();
		closesocket(soc);
		return 1;
	}

	freeaddrinfo(rslt);

	return 0;

}

int HttpsendSSL(SSL* ssl, const char* buffer, size_t* written)
{

	if (SSL_write_ex(ssl, buffer, strlen(buffer), written) != 1)
	{
		fprintf(stderr, "\nfailed to send data stream to server");
		return 1;
	}
	else if (*written != strlen(buffer))
	{
		fprintf(stderr, "\nfailed to sent full data stream");
		return 1;
	}

	return 0;

}

char* HttprecvFullSSL(SSL* ssl)
{
	char* buffer = (char*)malloc(4096);
	if (!buffer) return NULL;

	int bytes;

	while ((bytes = SSL_read(ssl, buffer, 4095)) > 0)
	{
		buffer[bytes] = '\0';
		printf("%s", buffer);
	}

	if (bytes < 0) {
		free(buffer);
		return NULL;
	}

	return buffer;

}

int StatusCode(const char* recvbuff)
{

	int status = 0;

	if (sscanf(recvbuff, "HTTP/%*[^ ] %d", &status) == 1) {
		return status;
	}

	return 1;

}

const char* HttpTextcode(int status)
{

	switch (status)
	{
	case 100: return "Continue";
	case 101: return "Switching Protocols";
	case 102: return "Processing";
	case 103: return "Early Hints";
	case 200: return "OK";
	case 201: return "Created";
	case 202: return "Accepted";
	case 203: return "Non-Authoritative Information";
	case 204: return "No Content";
	case 205: return "Reset Content";
	case 206: return "Partial Content";
	case 207: return "Multi-Status";
	case 208: return "Already Reported";
	case 226: return "IM Used";
	case 300: return "Multiple Choices";
	case 301: return "Moved Permanently";
	case 302: return "Found";
	case 303: return "See Other";
	case 304: return "Not Modified";
	case 307: return "Temporary Redirect";
	case 308: return "Permanent Redirect";
	case 400: return "Bad Request";
	case 401: return "Unauthorized";
	case 402: return "Payment Required";
	case 403: return "Forbidden";
	case 404: return "Not Found";
	case 405: return "Method Not Allowed";
	case 406: return "Not Acceptable";
	case 407: return "Proxy Authentication Required";
	case 408: return "Request Timeout";
	case 409: return "Conflict";
	case 410: return "Gone";
	case 411: return "Length Required";
	case 412: return "Precondition Failed";
	case 413: return "Payload Too Large";
	case 414: return "URI Too Long";
	case 415: return "Unsupported Media Type";
	case 416: return "Range Not Satisfiable";
	case 417: return "Expectation Failed";
	case 418: return "I'm a teapot";
	case 421: return "Misdirected Request";
	case 422: return "Unprocessable Entity";
	case 423: return "Locked";
	case 424: return "Failed Dependency";
	case 425: return "Too Early";
	case 426: return "Upgrade Required";
	case 428: return "Precondition Required";
	case 429: return "Too Many Requests";
	case 431: return "Request Header Fields Too Large";
	case 451: return "Unavailable For Legal Reasons";
	case 500: return "Internal Server Error";
	case 501: return "Not Implemented";
	case 502: return "Bad Gateway";
	case 503: return "Service Unavailable";
	case 504: return "Gateway Timeout";
	case 505: return "HTTP Version Not Supported";
	case 506: return "Variant Also Negotiates";
	case 507: return "Insufficient Storage";
	case 508: return "Loop Detected";
	case 510: return "Not Extended";
	case 511: return "Network Authentication Required";
	default:  return "Unknown Status";
	}



}

HTTPRESPONSE httparse(const char* recvbuff)
{

	HTTPRESPONSE result;

	int code = StatusCode(recvbuff);
	const char* status = HttpTextcode(code);

	if (code == 1)
	{
		result.status = 0;
		result.status_text = "Invalid Status";

		return result;

	}

	result.status = code;
	result.status_text = status;

	return result;

}