#ifndef PROTOHTTP_H
#define PROTOHTTP_H

#define PROTOHTTP_DEFAULT_HTTP_PORT "80"
#define PROTOHTTP_DEFAULT_HTTPS_PORT "443"

#define PROTOHTTP_LIB_VERSION 0.1

#define OPENSSL_LIB 0
#define WOLFCRYPT   1
#define CRYPTLIB    2

#define EMPTY 0
#define MAX 100

#define SOCKET_TIMEOUT_DEFAULT 5
#define SOCKET_TIMEOUT_SET (x) (x > 0 && x > 5)

#ifndef PROTOHTTPLIB_PROTOCOL_TCP
#define PROTOHTTPLIB_PROTOCOL_TCP 0

#ifndef PROTOHTTPLIB_BUFFERSIZE
#define PROTOHTTPLIB_BUFFERSIZE (EMPTY)

#ifndef LISTENING_TIMOUT_MAX
#define LISTENING_TIMOUT_MAX (MAX)

#ifndef LISTENING_TIMOUT
#define LISTENING_TIMOUT (x) (x > EMPTY)

#ifndef PROTOHTTP_PAYLOAD_MAXSIZE
#define PROTOHTTP_PAYLOAD_MAXSIZE 100000

#ifndef PROTOHTTP_PAYLOAD_SIZE
#define PROTOHTTP_PAYLOAD_SIZE (x) (x > EMPTY)

#ifndef PROTOHTTP_TOTAL_TIME
#define PROTOHTTP_TOTAL_TIME (x) (x > -1.000)

#define NO_REQUESTS_DONE (EMPTY)
#define INVALID_TEXT_STATUS "INVALID HTTP RESULT CODE"

typedef time_t size_t;

typedef enum
{
    
    SOCKET_CREATION_FAILED                  = (int)0x00,
    OPENSSL_INITIALIZE                      = (int)0x01,
    OPENSSL_CONTEXT_CREATION_FAILED         = (int)0x02,
    WINSOCK_LIBRARY_INITILIZATION_FAILED    = (int)0x03,
    WINSOCK_LIBRARY_NOT_INITILIZED          = (int)0x04,
    INVALID_HTTP_METHOD                     = (int)0x05,
    INVALID_PORT                            = (int)0x06,
    HTTP_BUILD_FAILED                       = (int)0x07,
    OPENSSL_WEBSOCKET_WRAP_FAILURE          = (int)0x08,
    OPENSSL_WEBSOCKET_CLOSING_FAILURE       = (int)0x09,
    OPENSSL_WEBSOCKET_RECVING_FAILED        = (int)0x0A,
    OPENSSL_WEBSOCKET_WRITING_FAILED        = (int)0x0B,
    WEBSOCKET_WRITING_FAILED                = (int)0x0C,
    WEBSOCKET_RECVING_FAILED                = (int)0x0D,
    FAILED_TOGET_STATUSCODE                 = (int)0x0E,
    PARSING_HTTP_BUFFER_FAILED              = (int)0x0F,
    INVALID_STATUS                          = (int)0x10
    
} errno;

#if defined(_WIN32)

#include <winsock2.h>
#include <ws2tcpip.h>

#elif defined (__LINUX__) || defined(__APPLE__)

#define INVALID_SOCKET  (-1)
#define SOCKET_ERROR    (-1)

typedef int SOCKET;


#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>
#include <errno.h>

#endif

#include <stdio.h>
#include <string.h>
#include <

typedef enum
{

	GET,
	POST,
	PUT,
	DDELETE,
	UNKNOWN,

} REQUEST;


typedef enum
{

	HTTP_CONTINUE = 100,
	HTTP_SWITCHING_PROTOCOLS = 101,
	HTTP_PROCESSING = 102,
	HTTP_EARLY_HINTS = 103,
	HTTP_OK = 200,
	HTTP_CREATED = 201,
	HTTP_ACCEPTED = 202,
	HTTP_NON_AUTHORITATIVE_INFORMATION = 203,
	HTTP_NO_CONTENT = 204,
	HTTP_RESET_CONTENT = 205,
	HTTP_PARTIAL_CONTENT = 206,
	HTTP_MULTI_STATUS = 207,
	HTTP_ALREADY_REPORTED = 208,
	HTTP_IM_USED = 226,
	HTTP_MULTIPLE_CHOICES = 300,
	HTTP_MOVED_PERMANENTLY = 301,
	HTTP_FOUND = 302,
	HTTP_SEE_OTHER = 303,
	HTTP_NOT_MODIFIED = 304,
	HTTP_USE_PROXY = 305,
	HTTP_TEMPORARY_REDIRECT = 307,
	HTTP_PERMANENT_REDIRECT = 308,
	HTTP_BAD_REQUEST = 400,
	HTTP_UNAUTHORIZED = 401,
	HTTP_PAYMENT_REQUIRED = 402,
	HTTP_FORBIDDEN = 403,
	HTTP_NOT_FOUND = 404,
	HTTP_METHOD_NOT_ALLOWED = 405,
	HTTP_NOT_ACCEPTABLE = 406,
	HTTP_PROXY_AUTHENTICATION_REQUIRED = 407,
	HTTP_REQUEST_TIMEOUT = 408,
	HTTP_CONFLICT = 409,
	HTTP_GONE = 410,
	HTTP_LENGTH_REQUIRED = 411,
	HTTP_PRECONDITION_FAILED = 412,
	HTTP_PAYLOAD_TOO_LARGE = 413,
	HTTP_URI_TOO_LONG = 414,
	HTTP_UNSUPPORTED_MEDIA_TYPE = 415,
	HTTP_RANGE_NOT_SATISFIABLE = 416,
	HTTP_EXPECTATION_FAILED = 417,
	HTTP_IM_A_TEAPOT = 418,
	HTTP_MISDIRECTED_REQUEST = 421,
	HTTP_UNPROCESSABLE_ENTITY = 422,
	HTTP_LOCKED = 423,
	HTTP_FAILED_DEPENDENCY = 424,
	HTTP_TOO_EARLY = 425,
	HTTP_UPGRADE_REQUIRED = 426,
	HTTP_PRECONDITION_REQUIRED = 428,
	HTTP_TOO_MANY_REQUESTS = 429,
	HTTP_REQUEST_HEADER_FIELDS_TOO_LARGE = 431,
	HTTP_UNAVAILABLE_FOR_LEGAL_REASONS = 451,
	HTTP_INTERNAL_SERVER_ERROR = 500,
	HTTP_NOT_IMPLEMENTED = 501,
	HTTP_BAD_GATEWAY = 502,
	HTTP_SERVICE_UNAVAILABLE = 503,
	HTTP_GATEWAY_TIMEOUT = 504,
	HTTP_HTTP_VERSION_NOT_SUPPORTED = 505,
	HTTP_VARIANT_ALSO_NEGOTIATES = 506,
	HTTP_INSUFFICIENT_STORAGE = 507,
	HTTP_LOOP_DETECTED = 508,
	HTTP_NOT_EXTENDED = 510,
	HTTP_NETWORK_AUTHENTICATION_REQUIRED = 511

} HTTPCODE;

typedef struct
{

	HTTPCODE status;
	const char* status_text;
    time_t totaltime;
    size_t totalrequests;
    
} HTTPRESPONSE;

int WINDOWSInitialize(void);
int GetError(void);

void CloseWebSocket(SOCKET Socket);

void OpenSSLIntilize(void);
SSL_CTX* SSLCTX(void);

REQUEST Httpbuild(const char* type);
void HttpBuildRequest(REQUEST request_t, char* buffer, const char* host, const char* data);
SOCKET HttpOpenBridge(const char* HOST, const char* port, struct addrinfo** rslt);
SSL* WrapSocketTLS(SSL_CTX* ctx, SOCKET sock, const char* HOST);

void CloseTLS(SSL* ssl, SSL_CTX* ctx, SOCKET sock);

int HttpConnect(const SOCKET soc, struct addrinfo* rslt);
int HttpsendSSL(SSL *ssl, const char* buffer, size_t* written);
char* HttprecvFullSSL(SSL* ssl);
const char* HttpTextcode(int status);
HTTPRESPONSE httparse(const char* recvbuff);
int StatusCode(const char* recvbuff);


#endif
