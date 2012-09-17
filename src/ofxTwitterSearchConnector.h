#pragma once
#include "ofxTwitterConnector.h"
#include "ofxHttpUtils.h"

#define ofxTwitterSearchConnector_BaseURL "http://search.twitter.com/search.json"
#define ofxTwitterSearchConnector_NoPoll -1

using namespace std;

class ofxTwitterSearchConnector : public ofxTwitterConnector {
public:
	ofxTwitterSearchConnector();
	void setup(
               string  searchTerms,
               int     pollInterval = ofxTwitterSearchConnector_NoPoll
            );
	
	void run();
	void stop();
private:
    string          searchTerms;
    int             pollInterval;
    ofxHttpUtils    httpUtils;
    string          maxTweetID;
    void            startQuery();
    void            receiveResponse(ofxHttpResponse &response);
};