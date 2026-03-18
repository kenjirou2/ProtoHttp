#ifdef _WIN32

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

	typedef enum {

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
	
	
	
	typedef struct {
		
		HTTPCODE status;
		const char *status_text;

	} HTTPRESPONSE;
	
	
	
	
	int WSAIntilize()
	{
		WSADATA wsa;
		if (WSAStartup(MAKEWORD(2,2), &wsa) != 0) {
			printf("WSAStartup failed\n");
			return 1;
		}
		
		return 0;
	
	}

	REQUEST Httpbuild(const char *type)
	{


		if (strcmp(type, "GET") == 0){return GET;}

		if (strcmp(type, "POST") == 0) {return POST;}

		if (strcmp(type, "PUT") == 0){return PUT;}

		if (strcmp(type, "DELETE_") == 0){return DELETE_;}

		else {return UNKNOWN;}
		

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



	SOCKET HttpOpenBridge(const char *HOST, const char *port, struct addrinfo **rslt)
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

	char* HttprecvFull(SOCKET soc)
	{
		
		
		char buffer[4096];
		int recvr;


		while((recvr = recv(soc, buffer, sizeof(buffer)-1, 0)) > 0)
		{
			buffer[recvr] = '\0';
			printf("%s", buffer);
		}

		if(recvr == 0)
		{
			printf("\n[Connection closed by server]\n");
		}
		
		else if(recvr == SOCKET_ERROR)
		{
			
			printf("\n[Recv failed: %d]\n", WSAGetLastError());
			
		}
		
		return buffer;
		
		
	}
	
	int StatusCode(const char *recvbuff)
	{

		int status = 0;

		if (sscanf(recvbuff, "HTTP/%*s %d", &status) == 1) {
			return status;
		}
		
		return 1;
		
	}
	
	char* HttpTextcode(int status)
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
	
	HTTPRESPONSE httparse(const char *recvbuff)
	{
		
		HTTPRESPONSE result;
		
		int code = StatusCode(recvbuff);
		
		if (code == 1)
		{
			result.status = 0;
			result.status_text = "Invalid Status";
			
			return result;
			
		}
		
		result.status = code;
		result.status_test = ""
		
	}

 #endif