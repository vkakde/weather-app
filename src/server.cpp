///\author Vishwajeet Kakde
///\cite https://github.com/pocoproject/poco/blob/develop/Net/samples/httpget/src/httpget.cpp

// standard includes
#include <iostream>
#include <string>

// includes from POCO library
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include <Poco/Net/HTTPCredentials.h>
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

///\remark Not a good idea to imply so many namespaces
using Poco::Exception;
using Poco::Path;
using Poco::StreamCopier;
using Poco::URI;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;

class server{
	std::string APIKEY = "f6973a433d45ee52c637f99755a73e40";

	public:
	server(){};
	server(const server& copy){};
	~server(){};

	std::string getAPIKey(){
		return APIKEY;
	}

	void doRequest(Poco::Net::HTTPClientSession &session, Poco::Net::HTTPRequest &request, Poco::Net::HTTPResponse &response){
		session.sendRequest(request);
		std::istream &rs = session.receiveResponse(response);
		// std::cout << response.getStatus() << " " << response.getReason() << std::endl;
		StreamCopier::copyStream(rs, std::cout);
	}
};

int main(int argc, char **argv)
{
	if (argc != 2){
		Path p(argv[0]);
		std::cout << "usage: " << p.getBaseName() << " <uri>" << std::endl;
		std::cout << "       fetches the resource identified by <uri> and print it to the standard output" << std::endl;
		return 1;
	}

	try{
		// create server instance
		server svr;

		// build uri
		URI uri("http://api.openweathermap.org/data/2.5/weather");
		uri.addQueryParameter("q", argv[1]);
		uri.addQueryParameter("APPID", svr.getAPIKey());

		std::string path(uri.getPathAndQuery());
		if (path.empty())
			path = "/";

		HTTPClientSession session(uri.getHost(), uri.getPort());
		HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		HTTPResponse response;

		svr.doRequest(session, request, response);
	}
	catch (Exception &exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
	return 0;
}
