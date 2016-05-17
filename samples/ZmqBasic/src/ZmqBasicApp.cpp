#include "cinder/Log.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "ofxZmq.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ZmqBasicApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

private:
	ofxZmqSubscriber subscriber;
	ofxZmqPublisher publisher;
};

void ZmqBasicApp::setup() {
	// start server
	publisher.bind("tcp://*:9999");

	// start client
	subscriber.connect("tcp://localhost:9999");
}

void ZmqBasicApp::mouseDown(MouseEvent event) {
	std::string data = "this is a test";
	if (publisher.send(data)) {
		CI_LOG_V("Sending Data: " << data);
	} else {
		CI_LOG_E("Send failed.");
	}
}

void ZmqBasicApp::update() {
	while (subscriber.hasWaitingMessage()) {
		std::string data;
		subscriber.getNextMessage(data);
		CI_LOG_V("Received Data: " << data);
	}
}

void ZmqBasicApp::draw() {
	gl::clear(Color(0, 0, 0));
}

CINDER_APP(ZmqBasicApp, RendererGl)
