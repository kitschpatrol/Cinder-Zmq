#include "ofxZmqPublisher.h"

ofxZmqPublisher::ofxZmqPublisher()
		: ofxZmqSocket(ZMQ_PUB) {
}

void ofxZmqPublisher::connect(std::string addr) {
	ofxZmqSocket::connect(addr);
}

void ofxZmqPublisher::disconnect(std::string addr) {
	ofxZmqSocket::disconnect(addr);
}

void ofxZmqPublisher::bind(std::string addr) {
	ofxZmqSocket::bind(addr);
}

void ofxZmqPublisher::unbind(std::string addr) {
	ofxZmqSocket::unbind(addr);
}

bool ofxZmqPublisher::send(const void *data, size_t len, bool nonblocking, bool more) {
	return ofxZmqSocket::send(data, len, nonblocking, more);
}

bool ofxZmqPublisher::send(void *data, size_t len, bool nonblocking, bool more) {
	return ofxZmqSocket::send(data, len, nonblocking, more);
}

bool ofxZmqPublisher::send(const std::string &data, bool nonblocking, bool more) {
	return ofxZmqSocket::send((const void *)data.data(), data.size(), nonblocking, more);
}

bool ofxZmqPublisher::send(const ci::Buffer &data, bool nonblocking, bool more) {
	return ofxZmqSocket::send(data, nonblocking, more);
}
