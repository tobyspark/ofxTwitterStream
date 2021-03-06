#pragma once 
#include <string>
#include <deque>
#include <iostream>
#include "Poco/Thread.h"
#include "Poco/Runnable.h"
#include "Poco/String.h"
#include "Poco/RegularExpression.h"
#include "Poco/RWLock.h"
#include "modp_b64.h"
#include "jansson.h"
#include "ofxTweet.h"

using namespace std;

// Abstract base class, ie. see ofxTwitterStreamConnector
class ofxTwitterConnector : public Poco::Runnable {
public:
    ofxTwitterConnector();
    
	virtual void run(){};
    virtual void stop(){};
    
	void parseTweetJSON(json_t* tweetJSON);
	bool hasNewTweets();
	ofxTweet getNextTweet();
	
protected:
    bool is_connected;
	deque<ofxTweet> tweets;
	Poco::RWLock lock;
};