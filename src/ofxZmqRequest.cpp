#include "ofxZmqRequest.h"

ofxZmqRequest::ofxZmqRequest() : ofxZmqSocket(ZMQ_REQ)
{
}

void ofxZmqRequest::connect(std::string addr)
{
	ofxZmqSocket::connect(addr);
}

bool ofxZmqRequest::send(const void *data, size_t len, bool nonblocking, bool more)
{
	return ofxZmqSocket::send(data, len, nonblocking, more);
}

bool ofxZmqRequest::send(void *data, size_t len, bool nonblocking, bool more)
{
	return ofxZmqSocket::send(data, len, nonblocking, more);
}

bool ofxZmqRequest::send(const std::string &data, bool nonblocking, bool more)
{
	return ofxZmqSocket::send((const void*)data.data(), data.size(), nonblocking, more);
}

bool ofxZmqRequest::send(const ci::Buffer &data, bool nonblocking, bool more)
{
	return ofxZmqSocket::send(data, nonblocking, more);
}

bool ofxZmqRequest::receive(std::string &data)
{
	return ofxZmqSocket::receive(data);
}

bool ofxZmqRequest::receive(ci::Buffer &data)
{
	return ofxZmqSocket::receive(data);
}

bool ofxZmqRequest::hasWaitingMessage(long timeout_millis)
{
	return ofxZmqSocket::hasWaitingMessage(timeout_millis);
}

bool ofxZmqRequest::getNextMessage(std::string &data)
{
	return ofxZmqSocket::getNextMessage(data);
}

bool ofxZmqRequest::getNextMessage(ci::Buffer &data)
{
	return ofxZmqSocket::getNextMessage(data);
}
