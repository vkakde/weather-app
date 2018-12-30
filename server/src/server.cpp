///\author Vishwajeet Kakde
///\cite https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/hwserver.cpp

// standard includes
#include <iostream>
#include <string>
#include <unistd.h>

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

// includes from ZMQ
#include "zmq.hpp"

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
	zmq::context_t *context;
	zmq::socket_t *socket;

	public:
	server(){};
	server(const server& copy){};
	~server(){
		// delete zmq context and socket
		delete(context);
		delete(socket);
	};

	void init(){
	    //  Prepare context and socket
    	context = new zmq::context_t(1);
    	socket = new zmq::socket_t(*context, ZMQ_REP);
    	socket->bind("tcp://*:5555");
	}

	zmq::socket_t* getSocket(){
		return socket;
	}

	zmq::context_t* getContext(){
		return context;
	}

	std::string getAPIKey(){
		return APIKEY;
	}

	std::string getCityWeatherData(std::string cityName, HTTPResponse &response){
		// build uri
		URI uri("http://api.openweathermap.org/data/2.5/weather");
		uri.addQueryParameter("q", cityName);
		uri.addQueryParameter("APPID", APIKEY);

		std::string path(uri.getPathAndQuery());
		if (path.empty())
			path = "/";

		// create http req/res objects
		HTTPClientSession session(uri.getHost(), uri.getPort());
		HTTPRequest request(HTTPRequest::HTTP_GET, path, HTTPMessage::HTTP_1_1);

		// send HTTP request and record response head and response body
		std::string s_responseBody = http_doHTTPRequest(session, request, response);

		// Convert the response head to ostream, then to string
		///\remark Response body not needed - could be retained/returned if needed
		/*
		std::stringbuf sb;
		std::ostream os(&sb);
		response.write(os);
		std::string s_responseHead = sb.str();
		*/

		return s_responseBody;
	}

	///\cite https://github.com/pocoproject/poco/blob/develop/Net/samples/httpget/src/httpget.cpp
	std::string http_doHTTPRequest(Poco::Net::HTTPClientSession &session, Poco::Net::HTTPRequest &request, Poco::Net::HTTPResponse &response){
		session.sendRequest(request);
		std::istream &is = session.receiveResponse(response);
		std::stringbuf sb;
		std::ostream os(&sb);
		StreamCopier::copyStream(is, os);
		// StreamCopier::copyStream(is, std::cout);
		return sb.str();
	}

	///\cite https://stackoverflow.com/a/18776035
	std::string json_getValue(Object::Ptr aoJsonObject, std::string key) {
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
	try{
		// create server instance
		server svr;

		// initialize server
		svr.init();

		// begin accepting client requests
    	while (true) {
        	zmq::message_t request;

        	//  Wait for next request from client
        	svr.getSocket()->recv(&request);
			///\cite Convert cstring to string: https://stackoverflow.com/a/8960101
			std::string s_request((char*)request.data());

        	std::cout << "Received " + s_request + "\n";

        	// Process request
			HTTPResponse response;
			std::string responseBody = svr.getCityWeatherData(s_request, response);

			// Send response to client
			if(response.getStatus()!=Poco::Net::HTTPResponse::HTTP_OK){
				std::cout<<"\nFailed. HTTP Response Code: "<<response.getStatus();
			}
			else{
	        	//  Send reply back to client
    	    	zmq::message_t reply (responseBody.length());
        		memcpy (reply.data (), responseBody.c_str(), responseBody.length());
        		svr.getSocket()->send (reply);
			}
    	}

		/*
		// Parse the JSON and get the Results
		Parser loParser;
    	Poco::Dynamic::Var loParsedJson = loParser.parse(s_responseBody);
    	Poco::Dynamic::Var loParsedJsonResult = loParser.result();

    	// Get the JSON Object
    	Object::Ptr loJsonObject = loParsedJsonResult.extract<Object::Ptr>();

		// Get values for specific keys of the JSON object (name = city, main = key weather metrics)
	    std::cout<<"\n\nKey-value pairs of parsed JSON -- \n";
		std::cout << "Name: " << svr.json_getValue(loJsonObject, "name");
	    std::cout << "\nMain: " << svr.json_getValue(loJsonObject, "main");
		*/
	}
	catch (Exception &exc)
	{
		std::cerr << exc.displayText() << std::endl;
		return 1;
	}
	
	return 0;
}