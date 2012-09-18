#include "ofxTwitter.h"

// OFX TWITTER STREAM

ofxTwitterStream::ofxTwitterStream() {
}

ofxTwitterStream::~ofxTwitterStream() {
	connector.stop();
	thread.join();
}

void ofxTwitterStream::connect(
	string sUser
	,string sPass
	,string sServer
	,string sFilter
)
{
	connector.setup(sUser, sPass, sServer, sFilter);
	thread.start(connector);
}

bool ofxTwitterStream::hasNewTweets() {
	return connector.hasNewTweets();
}

ofxTweet ofxTwitterStream::getNextTweet() {
	return connector.getNextTweet();
}


// OFX TWITTER SEARCH

ofxTwitterSearch::ofxTwitterSearch() {
}

ofxTwitterSearch::~ofxTwitterSearch() {
	connector.stop();
	thread.join();
}

void ofxTwitterSearch::connect(
                               string           searchTerms
                               ,list<string>    searchParameters
                               ,int             pollInterval
                              )
{
	connector.setup(searchTerms, searchParameters, pollInterval);
	thread.start(connector);
}

bool ofxTwitterSearch::hasNewTweets() {
	return connector.hasNewTweets();
}

ofxTweet ofxTwitterSearch::getNextTweet() {
	return connector.getNextTweet();
}