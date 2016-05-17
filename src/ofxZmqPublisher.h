#pragma once

#include "ofxZmq.h"

class ofxZmqPublisher : public ofxZmqSocket {
public:
	ofxZmqPublisher();

	void connect(std::string addr);
	void bind(std::string addr);
	void disconnect(std::string addr);
	void unbind(std::string addr);

	bool send(const void *data, size_t len, bool nonblocking = false, bool more = false);
	bool send(void *data, size_t len, bool nonblocking = false, bool more = false);
	bool send(const std::string &data, bool nonblocking = false, bool more = false);
	bool send(const ci::Buffer &data, bool nonblocking = false, bool more = false);
};
