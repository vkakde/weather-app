///\author Vishwajeet Kakde
///\cite https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/hwserver.cpp

// standard includes
#include <iostream>
#include <string>
#include <unistd.h>
#include <thread>
#include <map>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

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
	int data_port = 6001; // ports for data communication: in the range 6001-6010;
	std::map<int, zmq::socket_t*> connectedClients; // client ID, client data socket
	server();
	server(const server& copy);
	~server();

	void init();

	zmq::socket_t* getSocket();

	zmq::context_t* getContext();

	int newClientHandler();

	std::string getAPIKey();

	std::string getCityWeatherData(std::string cityName, HTTPResponse &response);

	///\cite https://github.com/pocoproject/poco/blob/develop/Net/samples/httpget/src/httpget.cpp
	std::string http_doHTTPRequest(Poco::Net::HTTPClientSession &session, Poco::Net::HTTPRequest &request, Poco::Net::HTTPResponse &response);

	///\cite https://stackoverflow.com/a/18776035
	std::string json_getValue(Object::Ptr aoJsonObject, std::string key);
};