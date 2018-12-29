///\author Vishwajeet Kakde

// standard includes
#include <iostream>
#include <string>

// includes from POCO Net library
#include "Poco/Net/HTTPClientSession.h"
#include "Poco/Net/HTTPRequest.h"
#include "Poco/Net/HTTPResponse.h"
#include <Poco/Net/HTTPCredentials.h>
#include "Poco/StreamCopier.h"
#include "Poco/NullStream.h"
#include "Poco/Path.h"
#include "Poco/URI.h"
#include "Poco/Exception.h"

// includes from POCO JSON library
#include "Poco/JSON/Parser.h"
#include "Poco/JSON/ParseHandler.h"
#include "Poco/JSON/JSONException.h"

using Poco::Exception;
using Poco::Path;
using Poco::StreamCopier;
using Poco::URI;
using Poco::Net::HTTPClientSession;
using Poco::Net::HTTPMessage;
using Poco::Net::HTTPRequest;
using Poco::Net::HTTPResponse;
using namespace Poco::JSON;

class server{
	std::string APIKEY = "f6973a433d45ee52c637f99755a73e40";

	public:
	server(){};
	server(const server& copy){};
	~server(){};

	std::string getAPIKey(){
		return APIKEY;
	}

	///\cite https://github.com/pocoproject/poco/blob/develop/Net/samples/httpget/src/httpget.cpp
	std::string doHTTPRequest(Poco::Net::HTTPClientSession &session, Poco::Net::HTTPRequest &request, Poco::Net::HTTPResponse &response){
		session.sendRequest(request);
		std::istream &is = session.receiveResponse(response);
		std::stringbuf sb;
		std::ostream os(&sb);
		StreamCopier::copyStream(is, os);
		// std::cout << response.getStatus() << " " << response.getReason() << std::endl;
		// StreamCopier::copyStream(is, std::cout);
		return sb.str();
	}

	///\cite https://stackoverflow.com/a/18776035
	std::string getValue(Object::Ptr aoJsonObject, std::string key) {
    	Poco::Dynamic::Var loVariable;
    	std::string lsReturn;
  
    	// Get the member variable
    	loVariable = aoJsonObject->get(key);

    	// Return value as string
    	return loVariable.convert<std::string>();
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

		// create http req/res objects
		HTTPClientSession session(uri.getHost(), uri.getPort());
		HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);
		HTTPResponse response;

		// send HTTP request and record response head and response body
		std::string s_responseBody = svr.doHTTPRequest(session, request, response);

		// Convert the response head to ostream, then to string
		std::stringbuf sb;
		std::ostream os(&sb);
		response.write(os);
		std::string s_responseHead = sb.str();
		
		// TEST
		std::cout<<"\nResponse head-- \n"<<s_responseHead;
		std::cout<<"Response body-- \n"<<s_responseBody;
	
		// Parse the JSON and get the Results
		Parser loParser;
    	Poco::Dynamic::Var loParsedJson = loParser.parse(s_responseBody);
    	Poco::Dynamic::Var loParsedJsonResult = loParser.result();

    	// Get the JSON Object
    	Object::Ptr loJsonObject = loParsedJsonResult.extract<Object::Ptr>();

		// Get values for specific keys of the JSON object (name = city, main = key weather metrics)
	    std::cout<<"\n\nKey-value pairs of parsed JSON -- \n";
		std::cout << "Name: " << svr.getValue(loJsonObject, "name");
	    std::cout << "\nMain: " << svr.getValue(loJsonObject, "main");
	}
	catch (Exception &exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
	return 0;
}