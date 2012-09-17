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

void ofxTwitterStreamConnector::parseBuffer(vector<char> sBuffer) {
	int mode = 0;
	string line(sBuffer.begin(), sBuffer.end());
	Poco::trimInPlace(line);
	if(is_connected) {
		if(line[0] == '{' && line[line.size()-1] == '}') {
            // load json.
            json_error_t error;
            json_t* tweetJSON = json_loads(line.c_str(), 0, &error);
            if(!tweetJSON) {
                cout <<  "error: on line:" << error.line << ", " << error.text << endl;
                return;
            }
			parseTweetJSON(tweetJSON);
		}
	}
	else {
		string code;
		Poco::RegularExpression reg("(HTTP/1.1) ([0-9]+)");
		std::vector<string> vec;
		reg.split(line, 0,vec);
		if(vec.size() == 3 && Poco::icompare(vec[1], "http/1.1") == 0) {
			if(vec[2] == "200") {
				is_connected = true;
			}
			else {
				cout << "unhandled http result: " << vec[2] << endl;
			}
		}
        
	}
}

void ofxTwitterStreamConnector::stop() {
	if(socket_ptr != NULL) {
		socket_ptr->shutdown();
	}
}