#pragma once

#include "ofxZmq.h"

class ofxZmqPair : public ofxZmqSocket {
public:
	ofxZmqPair();

	void connect(std::string addr);
	void bind(std::string addr);

	bool send(const void *data, size_t len, bool nonblocking = false, bool more = false);
	bool send(void *data, size_t len, bool nonblocking = false, bool more = false);
	bool send(const std::string &data, bool nonblocking = false, bool more = false);
	bool send(const ci::Buffer &data, bool nonblocking = false, bool more = false);

	bool receive(std::string &data);
	bool receive(ci::Buffer &data);

	bool hasWaitingMessage(long timeout_millis = 0);
	bool getNextMessage(std::string &data);
	bool getNextMessage(ci::Buffer &data);
};