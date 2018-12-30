# weather-app
C++ client-server application for serving current weather reports from around the globe using OpenWeather API

# note
This project is a WIP

# External libraries and build steps for Linux
## POCO C++ Library (provides REST, JSON and MongoDB utilities for C++)
1. This project uses Poco basic edition (current version: 1.9.0)

2. With GNU Make installed, to extract the sources and build all libraries, testsuites and samples, simply

> gunzip poco-X.Y.tar.gz  
> tar -xf poco-X.Y.tar  
> cd poco-X.Y  
> ./configure  
> gmake -s  

(https://raw.githubusercontent.com/pocoproject/poco/poco-1.9.0-release/README)

## Zero MQ (provides messaging services) + CPP Zero MQ (C++ implementation of generic ZMQ)
1. Get ZMQ core from https://github.com/zeromq/libzmq

2. Get CPPZMQ from https://github.com/zeromq/cppzmq

3. Follow build steps here: https://github.com/zeromq/cppzmq/blob/master/README.md

## Exporting library paths
Once all ext libs have been successfully built, export their lib/.so paths for cmake/make to find. 
startup.sh can do the above steps for you. Simply "source startup.sh" from the root folder.

## Client and Server builds
In root folder:
> mkdir build && cd build
> cmake ..
> make

This should build the server project in folder "server/" and client project in folder "client/". These are executable projects. Run them.

