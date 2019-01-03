///\author Vishwajeet Kakde
///\cite https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/hwclient.cpp

#include "../include/client.hpp"

int main ()
{
    //  Prepare our context and socket
    zmq::context_t context (1);
    zmq::socket_t socket (context, ZMQ_REQ);

    std::cout << "Connecting to server..." << std::endl;
    socket.connect ("tcp://localhost:5554");

    string input = "";

    while(input!="q"){
        cout<<"\n\nEnter city name (\'q\' to quit): ";
        getline(cin, input);

        if(input=="q")
            continue;

        // create request and send message 
        zmq::message_t request (input.length());
        memcpy (request.data (), input.c_str(), input.length());
        std::cout<<"\n"<<request.data()<<"\n";
        socket.send (request);

        // get server reply
        zmq::message_t reply;
        socket.recv (&reply);
        ///\cite Convert message_t data from void* to string: https://stackoverflow.com/a/10967058
    	std::string s_reply = std::string(static_cast<char*>(reply.data()), reply.size());

        cout<<"Reply from server: " + s_reply;
    }

    // close socket and exit
    socket.close();

    return 0;
}