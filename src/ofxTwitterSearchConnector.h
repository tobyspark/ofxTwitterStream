#pragma once
#include "ofxTwitterConnector.h"
#include "ofxHttpUtils.h"
#include <list>

#define ofxTwitterSearchConnector_BaseURL "http://search.twitter.com/search.json"
#define ofxTwitterSearchConnector_NoPoll -1

using namespace std;

class ofxTwitterSearchConnector : public ofxTwitterConnector {
public:
	ofxTwitterSearchConnector();
	void setup(
               string           searchTerms
               ,list<string>    searchParameters
               ,int             pollInterval = ofxTwitterSearchConnector_NoPoll
            );
	
	void run();
	void stop();
private:
    string          searchTerms;
    list<string>    searchParameters;
    int             pollInterval;
    ofxHttpUtils    httpUtils;
    string          maxTweetID;
    void            startQuery();
    void            receiveResponse(ofxHttpResponse &response);
};