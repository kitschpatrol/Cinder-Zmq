#pragma once

#include "cinder/Buffer.h"
#include "zmq.hpp"

class ofxZmqSocket {

public:
	virtual ~ofxZmqSocket();

	void setIdentity(std::string data);
	std::string getIdentity();

	bool isConnected();

	void setHighWaterMark(long maxQueueSize);
	void setSendHighWaterMark(long maxQueueSize);
	void setReceiveHighWaterMark(long maxQueueSize);

	long getHighWaterMark();
	long getSendHighWaterMark();
	long getReceiveHighWaterMark();

protected:
	zmq::socket_t socket;
	zmq::pollitem_t items[1];

	ofxZmqSocket(int type);

	void connect(std::string addr);
	void bind(std::string addr);
	void disconnect(std::string addr);
	void unbind(std::string addr);

	bool send(const void *data, size_t len, bool nonblocking, bool more);
	bool send(void *data, size_t len, bool nonblocking, bool more);
	bool send(const std::string &data, bool nonblocking, bool more);
	bool send(const ci::Buffer &data, bool nonblocking, bool more);

	bool receive(std::string &data);
	bool receive(ci::Buffer &data);

	bool hasWaitingMessage(long timeout_millis = 0);

	// return true if has more flag
	bool getNextMessage(std::string &data);
	bool getNextMessage(ci::Buffer &data);
};