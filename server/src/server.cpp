///\author Vishwajeet Kakde
#include "../include/server.hpp"

server::server(){}

server::server(const server& copy){}

server::~server(){
	// delete zmq context and socket
	socket->close();
	delete(context);
	delete(socket);
}

void server::init(){
    //  Prepare context and socket
	context = new zmq::context_t(1);
	socket = new zmq::socket_t(*context, ZMQ_REP);
	socket->bind("tcp://127.0.0.1:5554");
}

zmq::socket_t* server::getSocket(){
	return socket;
}

zmq::context_t* server::getContext(){
	return context;
}

std::string server::getAPIKey(){
	return APIKEY;
}

std::string server::getCityWeatherData(std::string cityName, HTTPResponse &response){
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
std::string server::http_doHTTPRequest(Poco::Net::HTTPClientSession &session, Poco::Net::HTTPRequest &request, Poco::Net::HTTPResponse &response){
	session.sendRequest(request);
	std::istream &is = session.receiveResponse(response);
	std::stringbuf sb;
	std::ostream os(&sb);
	StreamCopier::copyStream(is, os);
	// StreamCopier::copyStream(is, std::cout);
	return sb.str();
}

///\cite https://stackoverflow.com/a/18776035
std::string server::json_getValue(Object::Ptr aoJsonObject, std::string key) {
	Poco::Dynamic::Var loVariable;
	std::string lsReturn;

	// Get the member variable
	loVariable = aoJsonObject->get(key);
	// Return value as string
	return loVariable.convert<std::string>();
}

int main(int argc, char **argv)
{
	try{
		// create server instance
		server svr;

		// initialize server
		svr.init();
		std::cout<<"\n -- Server is ready and awaiting client requests -- \n\n";

		// begin accepting client requests
    	while (true) {
			try{
        		zmq::message_t request;

        		//  Wait for next request from client
        		svr.getSocket()->recv(&request);
		        ///\cite Convert message_t data from void* to string: https://stackoverflow.com/a/10967058
		    	std::string s_request = std::string(static_cast<char*>(request.data()), request.size());

        		std::cout << "\nReceived " + s_request + "\n";

        		// Process request
				HTTPResponse response;
				std::string responseBody = svr.getCityWeatherData(s_request, response);

				// Send response to client
				if(response.getStatus()!=Poco::Net::HTTPResponse::HTTP_OK){
					std::cout<<"\nFailed. HTTP Response Code: "<<response.getStatus();
					svr.getSocket()->send (zmq::message_t());
				}
				else{
	        		//  Send reply back to client
    	    		zmq::message_t reply (responseBody.length());
        			memcpy (reply.data (), responseBody.c_str(), responseBody.length());
        			svr.getSocket()->send (reply);
				}
			}catch(std::exception &e){
				std::cout<<"\nException! Message: "<<e.what();
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