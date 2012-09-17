#include "ofxTwitterConnector.h"

ofxTwitterConnector::ofxTwitterConnector()
:is_connected(false)
{
}

/*
{
    "in_reply_to_status_id_str": "73829792671215616",
    "geo": null,
    "retweet_count": 0,
    "text": "@Cutie4Nkotbsb @DonnieWahlberg I love him to the moon and back!!!...",
    "id_str": "73829941053108224",
    "in_reply_to_screen_name": "Cutie4Nkotbsb",
    "in_reply_to_user_id_str": "21703864",
    "coordinates": null,
    "contributors": null,
    "retweeted": false,
    "created_at": "Thu May 26 19:16:44 +0000 2011",
    "source": "web",
    "truncated": false,
    "entities": {
        "user_mentions": [{
            "indices": [0, 14],
            "screen_name": "Cutie4Nkotbsb",
            "id_str": "21703864",
            "name": "Allison",
            "id": 21703864
        }, {
            "indices": [15, 30],
            "screen_name": "DonnieWahlberg",
            "id_str": "24776235",
            "name": "Donnie Wahlberg",
            "id": 24776235a
        }],
        "urls": [],
        "hashtags": []
    },
    "in_reply_to_user_id": 21703864,
    "place": null,
    "in_reply_to_status_id": 73829792671215616,
    "user": {
        "location": "Louisiana",
        "id_str": "305617428",
        "default_profile": true,
        "profile_text_color": "333333",
        "screen_name": "DdubsbabyBug",
        "is_translator": false,
        "profile_sidebar_fill_color": "DDEEF6",
        "listed_count": 2,
        "notifications": null,
        "profile_background_tile": false,
        "created_at": "Thu May 26 14:32:50 +0000 2011",
        "description": "Donnies baby bug Lola..  from Louisiana figuring out what I want in this world.  Live for tomorrow, hope for today and never miss a chance to say I love you.",
        "show_all_inline_media": false,
        "geo_enabled": false,
        "followers_count": 40,
        "contributors_enabled": false,
        "friends_count": 108,
        "profile_link_color": "0084B4",
        "profile_sidebar_border_color": "C0DEED",
        "default_profile_image": false,
        "lang": "en",
        "statuses_count": 86,
        "time_zone": "Central Time (US & Canada)",
        "protected": false,
        "profile_use_background_image": true,
        "favourites_count": 1,
        "url": null,
        "name": "Lola Venice",
        "verified": false,
        "profile_background_color": "C0DEED",
        "id": 305617428,
        "follow_request_sent": null,
        "following": null,
        "profile_background_image_url": "http:\/\/a0.twimg.com\/images\/themes\/theme1\/bg.png",
        "utc_offset": -21600,
        "profile_image_url": "http:\/\/a3.twimg.com\/profile_images\/1369940852\/2506567-4cda1e4d70e28-full_normal.jpg"
    },
    "id": 73829941053108224,
    "favorited": false
}
*/


void ofxTwitterConnector::parseTweetJSON(json_t* tweetJSON) {

	ofxTweet tweet;

	// text
	json_t* node =	json_object_get(tweetJSON, "text");
	if(!json_is_string(node)) {
		cout << "error: cannot get text from tweet" << endl;
		return;
	}
	string text = json_string_value(node);
	tweet.setText(text);

	// id_str
	node = json_object_get(tweetJSON, "id_str");
	if(!json_is_string(node)) {
		cout <<  "error: cannot get id_str from tweet" << endl;
		return;
	}	
	string id = json_string_value(node);
	tweet.setID(id);
		
	// user - values can be in tweet root or in user object
	json_t* user = json_object_get(tweetJSON, "user");
    if(json_is_object(user))
    {
        // user: screen name
        node = json_object_get(user, "screen_name");
        if(!json_is_string(node)) {
            cout << "error: cannot get user screen_name" << endl;
            return;
        }
        string screen_name = json_string_value(node);
        tweet.setScreenName(screen_name);
        
        // user: avatar
        node = json_object_get(user, "profile_image_url");
        if(!json_is_string(node)) {
            cout << "error: cannot get profile_image_url" << endl;
            return;
        }
        string profile_image = json_string_value(node);
        tweet.setAvatar(profile_image);
        
        // user: id
        node = json_object_get(user, "id_str");
        if(!json_is_string(node)) {
            cout << "error: cannot get user id" << endl;
            return;
        }
        string user_id = json_string_value(node);
        tweet.setUserID(user_id);
    }
	else
    {
        // user: screen name
        node = json_object_get(tweetJSON, "from_user_name"); // Note this is different from key when wrapped in user object
        if(!json_is_string(node)) {
            cout << "error: cannot get user screen_name" << endl;
            return;
        }
        string screen_name = json_string_value(node);
        tweet.setScreenName(screen_name);
        
        // user: avatar
        node = json_object_get(tweetJSON, "profile_image_url");
        if(!json_is_string(node)) {
            cout << "error: cannot get profile_image_url" << endl;
            return;
        }
        string profile_image = json_string_value(node);
        tweet.setAvatar(profile_image);
        
        // user: id
        node = json_object_get(tweetJSON, "id_str");
        if(!json_is_string(node)) {
            cout << "error: cannot get user id" << endl;
            return;
        }
        string user_id = json_string_value(node);
        tweet.setUserID(user_id);
    }


	// store tweet.
	lock.writeLock();
		tweets.push_back(tweet);
	lock.unlock();
}

bool ofxTwitterConnector::hasNewTweets() {
	return tweets.size() > 0;	
}

ofxTweet ofxTwitterConnector::getNextTweet() {
	lock.readLock();
	ofxTweet t = tweets.front();
	tweets.pop_front();
	lock.unlock();
	return t;
}
