#include "ofxTwitterSearchConnector.h"

ofxTwitterSearchConnector::ofxTwitterSearchConnector()
{
    maxTweetID = "0";
}

void ofxTwitterSearchConnector::setup(
                   string           _searchTerms
                   ,list<string>    _searchParameters
                   ,int             _pollInterval
                   )
{
    searchTerms = _searchTerms;
    searchParameters = _searchParameters;
    pollInterval = _pollInterval;
    
    ofAddListener(httpUtils.newResponseEvent, this, &ofxTwitterSearchConnector::receiveResponse);
}

void ofxTwitterSearchConnector::run()
{
    httpUtils.start();
    
    startQuery();
}

void ofxTwitterSearchConnector::stop()
{
    httpUtils.stop();
}

void ofxTwitterSearchConnector::startQuery()
{
    string query = ofxTwitterSearchConnector_BaseURL;
    string encodedSearchTerms;
    Poco::URI::encode(searchTerms, "#", encodedSearchTerms);
    
    query += "?q=" + encodedSearchTerms;
    
    list<string>::iterator parameter;
    for (parameter = searchParameters.begin(); parameter != searchParameters.end(); ++parameter)
    {
        string encodedParameter;
        Poco::URI::encode(*parameter, "#", encodedParameter);
        
        query += "&" + encodedParameter;
    }
    
    if (maxTweetID.compare("0")) query += "&since_id=" + maxTweetID;
    query += "&result_type=recent";

    httpUtils.addUrl(query);
}


void ofxTwitterSearchConnector::receiveResponse(ofxHttpResponse &response)
{
    cout << "ofxTwitterSearchConnector: HTTP RESPONSE RECIVED." << endl;
    
    string line = response.responseBody.getText();
    
    if(line[0] == '{' && line[line.size()-1] == '}')
    {
        // Load JSON
        json_t* node;
        json_error_t error;
        json_t* searchJSON = json_loads(line.c_str(), 0, &error);
        if(!searchJSON) {
            cout <<  "error: on line:" << error.line << ", " << error.text << endl;
            return;
        }
        
        // Extract and process tweets
        node = json_object_get(searchJSON, "results");
        if(!json_is_array(node)) {
            cout << "error: cannot get tweets from twitter search results" << endl;
            return;
        }
        size_t tweetCount = json_array_size(node);
        for (int i = 0; i < tweetCount; i++)
        {
            parseTweetJSON(json_array_get(node, i));
        }
        
        // If this is one page of results of many, issue the query to obtain the next page
        node = json_object_get(searchJSON, "next_page");
        if(json_is_string(node)) {
            cout << "There is a next page of results. Issuing next page query." << endl;
            
            string nextPageQuery = ofxTwitterSearchConnector_BaseURL;
            nextPageQuery += json_string_value(node);
            httpUtils.addUrl(nextPageQuery);
            
            return;
        }
        // If there are no more pages, we can start polling having updated maxTweetID
        else
        {
            // Extract tweet ID we've searched up to, so we can start here next time
            node = json_object_get(searchJSON, "max_id_str");
            if(!json_is_string(node)) {
                cout << "error: cannot get max_id from twitter search results" << endl;
                return;
            }
            maxTweetID = json_string_value(node);
        }

    }

    if (pollInterval != ofxTwitterSearchConnector_NoPoll)
    {
        if (pollInterval > 0)
        {
            Poco::Thread::sleep(pollInterval * 1000);
        }
        
        startQuery();
    }
}