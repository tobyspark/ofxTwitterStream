#include "ofxTwitterStreamConnector.h"

ofxTwitterStreamConnector::ofxTwitterStreamConnector()
//,socket_ptr(NULL)
{
    socket_ptr = NULL;
}

void ofxTwitterStreamConnector::setup(
                                string sUsername
                                ,string sPassword
                                ,string sStreamServer
                                ,string sFilterURL
                                )
{
	username = sUsername;
	password = sPassword;
	server = sStreamServer;
	filter = sFilterURL;
}

void ofxTwitterStreamConnector::run() {
	string encoded = modp::b64_encode(username +":" +password);
	Poco::Net::SocketAddress sa(server,80);
	Poco::Net::StreamSocket socket(sa);
	socket_ptr = &socket;
	Poco::Net::SocketStream str(socket);
	str << "GET " << filter << " HTTP/1.0\r\n";
	str << "Host: " << server << "\r\n";
	str << "User-Agent: ofxTwitterTest\r\n";
	str << "Access:*/*\r\n";
	str << "Authorization: Basic " << encoded << "\r\n";
	str << "\r\n\r\n";
	str.flush();
	char ch;
	vector<char> buffer;
	while(str) {
		str.read(&ch, 1);
		buffer.push_back(ch);
		if(ch == '\r') {
			parseBuffer(buffer);
			buffer.clear();
		}
	}
}

void ofxTwitterStreamConnector::stop() {
	if(socket_ptr != NULL) {
		socket_ptr->shutdown();
	}
}