#pragma once
#include "ofxTwitterConnector.h"
#include "Poco/Net/SocketAddress.h"
#include "Poco/Net/StreamSocket.h"
#include "Poco/Net/SocketStream.h"
#include "Poco/StreamCopier.h"

using namespace std;

class ofxTwitterStreamConnector : public ofxTwitterConnector {
public:
	ofxTwitterStreamConnector();
	void setup(
               string sUsername
               ,string sPassword
               ,string sStreamServer
               ,string sFilterURL
               );
	
	void run();
	void stop();
private:
	Poco::Net::StreamSocket* socket_ptr;
	string username;
	string password;
	string server;
	string filter;
};