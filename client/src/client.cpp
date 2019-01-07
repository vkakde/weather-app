///\author Vishwajeet Kakde
///\cite https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/hwclient.cpp

#include "../include/client.hpp"

client::client(){
    ctlContext = NULL;
    ctlSocket = NULL;
    dataContext = NULL;
    dataSocket = NULL;
}

client::~client(){
    // close connection to control socket
    ctlSocket->close();
    delete(ctlContext);
    delete(ctlSocket);
    // close connection to control socket
    dataSocket->close();
    delete(dataContext);
    delete(dataSocket);
}

void client::init(){
    // bind to server's control socket
    ctlContext = new zmq::context_t(1);
    ctlSocket = new zmq::socket_t(*ctlContext, ZMQ_REQ);
}

void client::teardown(){
    // send teardown request to server to close data socket
    std::string req = serverAssignedId + " teardown";
    zmq::message_t request (req.length());
    memcpy (request.data (), req.c_str(), req.length());
    ctlSocket->send (request);
    // get server response (should simply be an ACK)
    zmq::message_t res;
    ctlSocket->recv (&res);
}

zmq::socket_t* client::getCtlSocket(){
	return ctlSocket;
}

zmq::context_t* client::getCtlContext(){
	return ctlContext;
}

zmq::socket_t* client::getDataSocket(){
    if(dataSocket==NULL){
        // request server to create/bind to a new data socket
        // std::cout<<"\nsending new request\n";
        std::string req = "new";
        zmq::message_t request (req.length());
        memcpy (request.data (), req.c_str(), req.length());
        ctlSocket->send (request);
        // server reply will contain connection string to data socket it has allocated
        // for data exchange with this client
        zmq::message_t res;
        ctlSocket->recv (&res);
	    std::string s_res = std::string(static_cast<char*>(res.data()), res.size());
        std::stringstream ss(s_res);
        std::string id, port;
        ss>>id;
        ss>>port;
        serverAssignedId = id;
        // std::cout<<"\nserver assigned id: "<<serverAssignedId;
        // connect to this data socket
        dataContext = new zmq::context_t(1);
        dataSocket = new zmq::socket_t(*dataContext, ZMQ_PAIR);
        dataSocket->connect("tcp://localhost:"+port);
    }
    return dataSocket;
}

void client::closeDataConnectionToServer(){
}

int main ()
{
    // create and initialize client
    client cl;
    cl.init();

    // connect to server (control socket)
    cl.getCtlSocket()->connect ("tcp://localhost:5554");
    std::cout << "Connected to server...";

    // connect to server (data socket)
    cl.getDataSocket();

    string input = "";

    while(input!="q"){
        cout<<"\n\nEnter city name (\'q\' to quit): ";
        getline(cin, input);

        if(input=="q")
            continue;

        // client request must be prefixed with its serverAssignedId
        input = cl.serverAssignedId + " " + input;    

        // create request and send message 
        zmq::message_t request (input.length());
        memcpy (request.data (), input.c_str(), input.length());
        cl.getCtlSocket()->send (request);

        // get data reply
        zmq::message_t reply;
        cl.getDataSocket()->recv (&reply);
        ///\cite Convert message_t data from void* to string: https://stackoverflow.com/a/10967058
    	std::string s_reply = std::string(static_cast<char*>(reply.data()), reply.size());

        cout<<"\nReply from server: \n" + s_reply;

        // get ctl reply
        cl.getCtlSocket()->recv (&reply);
    }

    // cl.teardown();

    return 0;
}