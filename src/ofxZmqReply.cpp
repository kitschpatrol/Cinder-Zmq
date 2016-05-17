#include "ofxZmqReply.h"

ofxZmqReply::ofxZmqReply() : ofxZmqSocket(ZMQ_REP)
{
}

void ofxZmqReply::bind(std::string addr)
{
	ofxZmqSocket::bind(addr);
}

bool ofxZmqReply::send(const void *data, size_t len, bool nonblocking, bool more)
{
	return ofxZmqSocket::send(data, len, nonblocking, more);
}

bool ofxZmqReply::send(void *data, size_t len, bool nonblocking, bool more)
{
	return ofxZmqSocket::send(data, len, nonblocking, more);
}

bool ofxZmqReply::send(const std::string &data, bool nonblocking, bool more)
{
	return ofxZmqSocket::send((const void*)data.data(), data.size(), nonblocking, more);
}

bool ofxZmqReply::send(const ci::Buffer &data, bool nonblocking, bool more)
{
	return ofxZmqSocket::send(data, nonblocking, more);
}

bool ofxZmqReply::receive(std::string &data)
{
	return ofxZmqSocket::receive(data);
}

bool ofxZmqReply::receive(ci::Buffer &data)
{
	return ofxZmqSocket::receive(data);
}

bool ofxZmqReply::hasWaitingMessage(long timeout_millis)
{
	return ofxZmqSocket::hasWaitingMessage(timeout_millis);
}

bool ofxZmqReply::getNextMessage(std::string &data)
{
	return ofxZmqSocket::getNextMessage(data);
}

bool ofxZmqReply::getNextMessage(ci::Buffer &data)
{
	return ofxZmqSocket::getNextMessage(data);
}
