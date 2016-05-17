#include "ofxZmqSubscriber.h"

ofxZmqSubscriber::ofxZmqSubscriber()
		: ofxZmqSocket(ZMQ_SUB) {
}

void ofxZmqSubscriber::connect(std::string addr) {
	setFilter(filter);
	ofxZmqSocket::connect(addr);
}

void ofxZmqSubscriber::disconnect(std::string addr) {
	ofxZmqSocket::disconnect(addr);
}

bool ofxZmqSubscriber::receive(std::string &data) {
	return ofxZmqSocket::receive(data);
}

bool ofxZmqSubscriber::receive(ci::Buffer &data) {
	return ofxZmqSocket::receive(data);
}

bool ofxZmqSubscriber::hasWaitingMessage(long timeout_millis) {
	return ofxZmqSocket::hasWaitingMessage(timeout_millis);
}

bool ofxZmqSubscriber::getNextMessage(std::string &data) {
	return ofxZmqSocket::getNextMessage(data);
}

bool ofxZmqSubscriber::getNextMessage(ci::Buffer &data) {
	return ofxZmqSocket::getNextMessage(data);
}

void ofxZmqSubscriber::setFilter(std::string f) {
	filter = f;
	socket.setsockopt(ZMQ_SUBSCRIBE, filter.data(), filter.size());
}
