#include "ofxZmqPair.h"

ofxZmqPair::ofxZmqPair()
		: ofxZmqSocket(ZMQ_PAIR) {
}

void ofxZmqPair::connect(std::string addr) {
	ofxZmqSocket::connect(addr);
}

void ofxZmqPair::bind(std::string addr) {
	ofxZmqSocket::bind(addr);
}

bool ofxZmqPair::send(const void *data, size_t len, bool nonblocking, bool more) {
	return ofxZmqSocket::send(data, len, nonblocking, more);
}

bool ofxZmqPair::send(void *data, size_t len, bool nonblocking, bool more) {
	return ofxZmqSocket::send(data, len, nonblocking, more);
}

bool ofxZmqPair::send(const std::string &data, bool nonblocking, bool more) {
	return ofxZmqSocket::send((const void *)data.data(), data.size(), nonblocking, more);
}

bool ofxZmqPair::send(const ci::Buffer &data, bool nonblocking, bool more) {
	return ofxZmqSocket::send(data, nonblocking, more);
}

bool ofxZmqPair::receive(std::string &data) {
	return ofxZmqSocket::receive(data);
}

bool ofxZmqPair::receive(ci::Buffer &data) {
	return ofxZmqSocket::receive(data);
}

bool ofxZmqPair::hasWaitingMessage(long timeout_millis) {
	return ofxZmqSocket::hasWaitingMessage(timeout_millis);
}

bool ofxZmqPair::getNextMessage(std::string &data) {
	return ofxZmqSocket::getNextMessage(data);
}

bool ofxZmqPair::getNextMessage(ci::Buffer &data) {
	return ofxZmqSocket::getNextMessage(data);
}
