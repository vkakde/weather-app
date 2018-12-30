#export environmental vars to external libs for use by cmake
export POCO_HOME=$PWD/ext/poco-1.9.0
export ZMQ_HOME=$PWD/ext/libzmq
export CPPZMQ_HOME=$PWD/ext/cppzmq

#export library paths
# POCO libs
export LIBRARY_PATH=$POCO_HOME/lib/Linux/x86_64
export LD_LIBRARY_PATH=$POCO_HOME/lib/Linux/x86_64
# ZMQ libs
export LIBRARY_PATH=$LIBRARY_PATH:$ZMQ_HOME/builds/lib
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$ZMQ_HOME/builds/lib
