#include "cinder/Log.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "cinder/ip/Resize.h"

#include "ofxZmq.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class ImagePublisherApp : public App {
public:
	void setup() override;
	void mouseDown(MouseEvent event) override;
	void update() override;
	void draw() override;

private:
	ofxZmqPublisher publisher;
	SurfaceRef mTestImage;
	gl::TextureRef mTestTexture;
};

void ImagePublisherApp::setup() {
	// start server

	// If we want to send from one publisher to multiple subscribers, we bind the publisher and connect the subscriber
	// publisher.bind("tcp://*:9999");

	// If we want to send from mulitple publishers to one subscriber, we connect the publisher and bind the subscriber
	publisher.connect("tcp://localhost:9999");
}

void ImagePublisherApp::mouseDown(MouseEvent event) {
	CI_LOG_V("Sending...");

	mTestImage = Surface::create(loadImage(loadUrl("http://thecatapi.com/api/images/get")));
	mTestTexture = gl::Texture::create(*mTestImage);

	// Resize Image
	// SurfaceRef testImageResized = Surface::create(500, 500, false);
	// ip::resize(*mTestImage, testImageResized.get());
	// ImageSourceRef testImageSource = (ImageSourceRef)*testImageResized;

	// Compress to jpeg
	OStreamMemRef os = OStreamMem::create();
	DataTargetRef target = DataTargetStream::createRef(os);
	writeImage(target, (ImageSourceRef)*mTestImage, ImageTarget::Options().quality(0.5), "jpg");

	// Write to ZeroMQ
	const void *data = os->getBuffer();
	size_t size = os->tell();

	if (publisher.send(data, size)) {
		CI_LOG_V("Sent " << size / 1000 << "kB");
	} else {
		CI_LOG_E("Send failed.");
	}
}

void ImagePublisherApp::update() {
}

void ImagePublisherApp::draw() {
	gl::clear(Color(0, 0, 0));
	gl::draw(mTestTexture);
	gl::drawString("Click anywhere to send a cat to subscribers over ZeroMQ.", vec2(10, 10));
}

CINDER_APP(ImagePublisherApp, RendererGl, [](App::Settings *settings) { //
	settings->setTitle("Cinder-Zmq Image Publisher Sample");								 //
})																																				 //
