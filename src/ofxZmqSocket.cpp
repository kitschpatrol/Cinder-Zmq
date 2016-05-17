#include "ofxZmqSocket.h"

#include "cinder/Log.h"

#include "ofxZmqConfig.h"

static zmq::context_t *ctx = NULL;

static zmq::context_t &ofxZmqContext() {
	if (ctx == NULL) {
		ctx = new zmq::context_t(OFXZMQ_NUM_THREAD);
	}
	return *ctx;
}

ofxZmqSocket::ofxZmqSocket(int type)
		: socket(ofxZmqContext(), type) {
	zmq::pollitem_t item;
	item.socket = (void *)socket;
	item.fd = 0;
	item.events = ZMQ_POLLIN;
	item.revents = 0;
	items[0] = item;
}

ofxZmqSocket::~ofxZmqSocket() {
}

void ofxZmqSocket::connect(std::string addr) {
	socket.connect(addr.c_str());
}

void ofxZmqSocket::bind(std::string addr) {
	socket.bind(addr.c_str());
}

void ofxZmqSocket::disconnect(std::string addr) {
	socket.disconnect(addr.c_str());
}

void ofxZmqSocket::unbind(std::string addr) {
	socket.unbind(addr.c_str());
}

bool ofxZmqSocket::send(const void *data, size_t len, bool nonblocking, bool more) {
	zmq::message_t m(len);
	memcpy(m.data(), data, len);

	int flags = 0;

	if (more)
		flags |= ZMQ_SNDMORE;
	if (nonblocking)
		flags |= ZMQ_DONTWAIT;

	try {
		return socket.send(m, flags);
	} catch (zmq::error_t &e) {
		CI_LOG_E("ofxZmqSocket::send: " << e.what());
		return false;
	}
}

bool ofxZmqSocket::send(void *data, size_t len, bool nonblocking, bool more) {
	return ofxZmqSocket::send((const void *)data, len, nonblocking, more);
}

bool ofxZmqSocket::send(const std::string &data, bool nonblocking, bool more) {
	return ofxZmqSocket::send((const void *)data.data(), data.size(), nonblocking, more);
}

bool ofxZmqSocket::send(const ci::Buffer &data, bool nonblocking, bool more) {
	return ofxZmqSocket::send(data.getData(), data.getSize(), nonblocking, more);
}

bool ofxZmqSocket::receive(std::string &data) {
	int32_t more;
	size_t more_size = sizeof(more);

	data.clear();

	zmq::message_t m;
	socket.recv(&m);

	socket.getsockopt(ZMQ_RCVMORE, &more, &more_size);

	const int numBytes = m.size();
	const uint8_t *src = (uint8_t *)m.data();

	data.insert(data.end(), src, src + numBytes);

	return more;
}

bool ofxZmqSocket::receive(ci::Buffer &data) {
	int32_t more = 0;
	size_t more_size = sizeof(more);

	zmq::message_t m;
	socket.recv(&m);
	socket.getsockopt(ZMQ_RCVMORE, &more, &more_size);

	// TODO clear existing data?
	// TODO check buffer length?
	// TODO kind of a mess.
	data.resize(m.size());
	data.copyFrom(m.data(), m.size());

	return more;
}

void ofxZmqSocket::setIdentity(std::string data) {
	socket.setsockopt(ZMQ_IDENTITY, data.data(), data.size());
}

std::string ofxZmqSocket::getIdentity() {
	char buf[255];
	size_t size = 0;
	socket.getsockopt(ZMQ_IDENTITY, buf, &size);
	return std::string(buf, buf + size);
}

bool ofxZmqSocket::isConnected() {
	return this->socket.connected();
}

void ofxZmqSocket::setHighWaterMark(long maxQueueSize) {
	setReceiveHighWaterMark(maxQueueSize);
	setSendHighWaterMark(maxQueueSize);
}

void ofxZmqSocket::setSendHighWaterMark(long maxQueueSize) {
	int32_t v = maxQueueSize;
	size_t size = sizeof(v);
	socket.setsockopt(ZMQ_SNDHWM, &v, size);
}

void ofxZmqSocket::setReceiveHighWaterMark(long maxQueueSize) {
	int32_t v = maxQueueSize;
	size_t size = sizeof(v);
	socket.setsockopt(ZMQ_RCVHWM, &v, size);
}

long ofxZmqSocket::getHighWaterMark() {
	return getSendHighWaterMark();
}

long ofxZmqSocket::getSendHighWaterMark() {
	int32_t v;
	size_t size = sizeof(v);
	socket.getsockopt(ZMQ_SNDHWM, &v, &size);
	return v;
}

long ofxZmqSocket::getReceiveHighWaterMark() {
	int32_t v;
	size_t size = sizeof(v);
	socket.getsockopt(ZMQ_RCVHWM, &v, &size);
	return v;
}

bool ofxZmqSocket::hasWaitingMessage(long timeout_millis) {
	return zmq::poll(items, 1, timeout_millis * 1000) > 0;
}

bool ofxZmqSocket::getNextMessage(std::string &data) {
	if ((items[0].revents & ZMQ_POLLIN) == false)
		return false;
	return receive(data);
}

bool ofxZmqSocket::getNextMessage(ci::Buffer &data) {
	if ((items[0].revents & ZMQ_POLLIN) == false)
		return false;
	return receive(data);
}
