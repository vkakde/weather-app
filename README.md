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

## Exporting library path
Once all ext libs have been built, export their paths. For example - if POCO lib folder exists in /home/user1/poco/lib/Linux/x86_64:
> export LIBRARY_PATH=/home/user1/poco/lib/Linux/x86_64  
> export LD_LIBRARY_PATH=/home/user1/poco/lib/Linux/x86_64  

## Server
Server is defined as cmake project "server". With Poco built successfully, navigate to build/ in root. 
> cmake ..  
> make

With successful make, an executable "server" should now exist in build/. Run this to start the server. 

