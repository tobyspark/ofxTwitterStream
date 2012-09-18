#pragma once
#include <string>
#include <Poco/Thread.h>
#include "ofxTwitterStreamConnector.h"
#include "ofxTwitterSearchConnector.h"
#include "ofxTweet.h"
using namespace std;

class ofxTwitterStream {
public:
	ofxTwitterStream();
	~ofxTwitterStream();
	void connect(
		 string sUsername
		,string sPassword
		,string sStreamServer
		,string sFilterURL
	);
	bool hasNewTweets();
	ofxTweet getNextTweet();	
private:
	ofxTwitterStreamConnector connector;
	Poco::Thread thread;
};

class ofxTwitterSearch {
public:
	ofxTwitterSearch();
	~ofxTwitterSearch();
	void connect(
                 string           searchTerms
                 ,list<string>    searchParameters
                 ,int             pollInterval = ofxTwitterSearchConnector_NoPoll
    );
	bool hasNewTweets();
	ofxTweet getNextTweet();
private:
	ofxTwitterSearchConnector connector;
	Poco::Thread thread;
};