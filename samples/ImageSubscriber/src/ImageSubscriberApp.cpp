#include "cinder/Log.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "ofxZmq.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImageSubscriberApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

private:
	ofxZmqSubscriber subscriber;
	gl::TextureRef mTestImage;
};

void ImageSubscriberApp::setup() {
	// start client

	// If we want to send from one publisher to multiple subscribers, we bind the publisher and connect the subscriber
	// subscriber.connect("tcp://localhost:9999");

	// If we want to send from mulitple publishers to one subscriber, we connect the publisher and bind the subscriber
	subscriber.bind("tcp://*:9999");
}

void ImageSubscriberApp::mouseDown(MouseEvent event) {
}

void ImageSubscriberApp::update() {
	while (subscriber.hasWaitingMessage()) {
		Buffer data;
		subscriber.getNextMessage(data);
		CI_LOG_I("Received data: " << data.getSize() / 1000 << "kB");
		const BufferRef bufferRef = ci::Buffer::create(data.getData(), data.getSize());
		mTestImage = gl::Texture::create(loadImage(DataSourceBuffer::create(bufferRef)));
	}
}

void ImageSubscriberApp::draw() {
	gl::clear(Color(0, 0, 0));
	gl::draw(mTestImage);
}

CINDER_APP(ImageSubscriberApp, RendererGl, [](App::Settings *settings) { //
	settings->setTitle("Cinder-Zmq Image Subscriber Sample");									//
})																																					//
