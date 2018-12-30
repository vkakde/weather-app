///\author Vishwajeet Kakde
///\cite https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/hwclient.cpp

#include<iostream>
#include<string>
#include "zmq.hpp"

using namespace std;

int main ()
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to server..." << std::endl;
    socket.connect ("tcp://localhost:5555");

    string input = "";

    while(input!="q"){
        cout<<"\n\nEnter city name (\'q\' to quit): ";
        getline(cin, input);

        // create request and send message 
        zmq::message_t request (input.length());
        memcpy (request.data (), input.c_str(), input.length());
        socket.send (request);

        // get server reply
        zmq::message_t reply;
        socket.recv (&reply);
		///\cite Convert cstring to string: https://stackoverflow.com/a/8960101
		std::string s_reply((char*)reply.data());

        cout<<"Reply from server: " + s_reply;
    }

    return 0;
}