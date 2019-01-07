///\author Vishwajeet Kakde
///\cite https://github.com/booksbyus/zguide/blob/master/examples/C%2B%2B/hwclient.cpp

#include<iostream>
#include<string>
#include "zmq.hpp"

using namespace std;

class client{
    zmq::context_t *ctlContext;
    zmq::socket_t *ctlSocket;
    zmq::context_t *dataContext;
    zmq::socket_t *dataSocket;

public:
    std::string serverAssignedId;

    client();

    ~client();

    void init();

    void teardown();

    zmq::socket_t* getCtlSocket();

    zmq::context_t* getCtlContext();

    zmq::socket_t* getDataSocket();

    void closeDataConnectionToServer();
};