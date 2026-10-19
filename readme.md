<!-- title({ prefix: "Cinder-" }) -->

# Cinder-Zmq

<!-- /title -->

<!-- badges -->

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/license/mit)

<!-- /badges -->

<!-- short-description -->

**CinderBlock for the ZeroMQ distributed messaging library. This is a Cinder port of Satoru Higa's openFrameworks addon ofxZmq.**

<!-- /short-description -->

## Overview

A wrapper for using the [ZeroMQ](https://zeromq.org) distributed messaging system with [Cinder](https://libcinder.org), packaged as a [CinderBlock](https://libcinder.org/docs/guides/cinder-blocks/index.html).

This is a minimally invasive Cinder port of [Satoru Higa's ofxZmq](https://github.com/satoruhiga/ofxZmq) openFrameworks addon. It keeps ofxZmq's class names and methods, with Cinder types such as `ci::Buffer` in place of openFrameworks types. It provides publisher, subscriber, request, reply, and pair sockets.

## Getting started

### Dependencies

Requires Cinder on macOS or Windows.

Dependencies are included as static libraries: libzmq for macOS (x86_64) and Windows (Win32 and x64). The libzmq C headers and the [cppzmq](https://github.com/zeromq/cppzmq) C++ headers are in [`include`](include). See [Updating bundled libraries](#updating-bundled-libraries) for where they came from.

The block was written in May 2016 against Cinder's master branch (0.9.1dev, between the 0.9.0 and 0.9.1 releases) on macOS, and on Windows with Visual Studio 2015. It hasn't been tested with later Cinder releases.

### Installation

From your Cinder `blocks` directory:

```sh
git clone https://github.com/kitschpatrol/Cinder-Zmq.git
```

Then add **Zmq** to a project with [TinderBox](https://libcinder.org/docs/guides/tinderbox/index.html). The block manifest adds the headers and links the matching static library for each platform and build configuration.

## Usage

Include `ofxZmq.h`, bind or connect a socket, and poll for messages in `update()`:

```cpp
#include "cinder/Log.h"
#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"

#include "ofxZmq.h"

class MyApp : public ci::app::App {
public:
	void setup() override {
		mPublisher.bind("tcp://*:9999");
		mSubscriber.connect("tcp://localhost:9999");
	}

	void mouseDown(ci::app::MouseEvent event) override {
		std::string message = "hello";
		mPublisher.send(message);
	}

	void update() override {
		while (mSubscriber.hasWaitingMessage()) {
			std::string message;
			mSubscriber.getNextMessage(message);
			CI_LOG_I("Received: " << message);
		}
	}

private:
	ofxZmqPublisher mPublisher;
	ofxZmqSubscriber mSubscriber;
};

CINDER_APP(MyApp, ci::app::RendererGl)
```

A bound subscriber can receive from several connected publishers; the image samples work this way.

`hasWaitingMessage(timeout)` polls the socket, waiting up to `timeout` milliseconds (default `0`). `getNextMessage()` and `receive()` return whether more parts of a multipart message follow, not whether a message was read.

The socket classes, from [`src`](src):

| Class              | ZeroMQ socket | Methods                                                                                                                  |
| ------------------ | ------------- | ------------------------------------------------------------------------------------------------------------------------ |
| `ofxZmqPublisher`  | `ZMQ_PUB`     | `bind()`, `connect()`, `unbind()`, `disconnect()`, `send()`                                                              |
| `ofxZmqSubscriber` | `ZMQ_SUB`     | `bind()`, `connect()`, `unbind()`, `disconnect()`, `setFilter()`, `receive()`, `hasWaitingMessage()`, `getNextMessage()` |
| `ofxZmqRequest`    | `ZMQ_REQ`     | `connect()`, `send()`, `receive()`, `hasWaitingMessage()`, `getNextMessage()`                                            |
| `ofxZmqReply`      | `ZMQ_REP`     | `bind()`, `send()`, `receive()`, `hasWaitingMessage()`, `getNextMessage()`                                               |
| `ofxZmqPair`       | `ZMQ_PAIR`    | `bind()`, `connect()`, `send()`, `receive()`, `hasWaitingMessage()`, `getNextMessage()`                                  |

`send()` takes a `std::string`, a `ci::Buffer`, or a pointer and length, followed by optional `nonblocking` and `more` flags. `receive()` and `getNextMessage()` fill a `std::string` or a `ci::Buffer`. Every socket also has `setIdentity()` / `getIdentity()`, `isConnected()`, and high-water mark setters and getters, declared in [ofxZmqSocket.h](src/ofxZmqSocket.h).

`setFilter(prefix)` subscribes to messages that start with `prefix`. Call it before `connect()` or `bind()`; otherwise the subscriber also keeps the default empty filter, which matches every message.

## Examples

The sample projects expect the block to be in Cinder's `blocks` directory. Each sample logs what it sends and receives to the console.

### ZmqBasic

The [ZmqBasic sample](samples/ZmqBasic/src/ZmqBasicApp.cpp) binds a publisher to `tcp://*:9999` and connects a subscriber to it in the same app. Click the window to publish the string "this is a test".

Open the [Xcode project](samples/ZmqBasic/xcode/ZmqBasic.xcodeproj) on macOS, or [ZmqBasic.sln](samples/ZmqBasic/vc2015/ZmqBasic.sln) (Visual Studio 2015) on Windows.

### ImagePublisher

The [ImagePublisher sample](samples/ImagePublisher/src/ImagePublisherApp.cpp) connects a publisher to `tcp://localhost:9999`. Click the window to download a random cat picture from [Cat as a service](https://cataas.com), compress it to JPEG, publish it, and draw it in the window. Run it alongside ImageSubscriber; you can run several publishers at once.

Open the [Xcode project](samples/ImagePublisher/xcode/ImagePublisher.xcodeproj) on macOS, or [ImagePublisher.sln](samples/ImagePublisher/vc2013/ImagePublisher.sln) on Windows.

### ImageSubscriber

The [ImageSubscriber sample](samples/ImageSubscriber/src/ImageSubscriberApp.cpp) binds a subscriber to `tcp://*:9999`, decodes each image it receives from ImagePublisher, and draws the latest one. Comments in both image samples show the reverse setup, where one bound publisher sends to several connected subscribers.

Open the [Xcode project](samples/ImageSubscriber/xcode/ImageSubscriber.xcodeproj) on macOS, or [ImageSubscriber.sln](samples/ImageSubscriber/vc2013/ImageSubscriber.sln) on Windows.

## Updating bundled libraries

Due to some issues with building on Windows via the 4.1.4 release, the libraries bundled in this block are from the [4.1.x stable release branch](https://github.com/zeromq/zeromq4-1/commit/56b71af22db3232eb57fd72e954e22eeac497b46). That commit is between the 4.1.4 and 4.1.5 releases, and its `zmq.h` reports version 4.1.5.

The libraries in [`lib`](lib) are debug and release builds for macOS (x86_64) and Windows (Win32 and x64, built with Visual Studio 2015). The block's copy of [`include/zmq.h`](include/zmq.h) adds a `ZMQ_STATIC` definition, marked "Added for Cinder-Zmq block.", so that Windows builds link against the static library instead of a DLL. Keep that definition if you replace the headers.

## Background

### Implementation notes

All sockets share one ZeroMQ context, created on first use with `OFXZMQ_NUM_THREAD` I/O threads (4 by default, set in [ofxZmqConfig.h](src/ofxZmqConfig.h)). To change it, add an `OFXZMQ_NUM_THREAD` preprocessor definition to your project. The context is never destroyed.

In ofxZmq, publishers only bind and subscribers only connect. In this block, both can do either.

### Other projects

- See also [Bruce Lane's Cinder-ZeroMQ](https://github.com/brucelane/Cinder-ZeroMQ) CinderBlock.

### Known issues

- Functions using `ci::Buffer` still need testing and reevaluation.
- There are no samples for the request, reply, or pair sockets.
- The ImagePublisher and ImageSubscriber Visual Studio projects use the Visual Studio 2013 toolset (v120), but the bundled Windows libraries were built with Visual Studio 2015. Retarget those projects to the v140 toolset before building them.
- The macOS libraries are x86_64 only, so they won't link in a native Apple silicon (arm64) build.
- Only `send()` catches `zmq::error_t` (it logs the error and returns `false`). Every other method lets it propagate, so an error such as binding an address that's already in use throws.
- `isConnected()` only reports whether the socket is open, not whether a peer is connected.
- `setFilter()` adds a subscription and doesn't remove earlier ones.

## Maintainers

[kitschpatrol](https://github.com/kitschpatrol)

<!-- contributing -->

## Contributing

[Issues](https://github.com/kitschpatrol/Cinder-Zmq/issues) are welcome and appreciated.

Please open an issue to discuss changes before submitting a pull request. Unsolicited PRs (especially AI-generated ones) are unlikely to be merged.

This repository uses [MDAT](https://github.com/kitschpatrol/mdat) for readme placeholder expansion.

<!-- /contributing -->

<!-- license -->

## License

[MIT](license.txt) © Eric Mika

<!-- /license -->

The MIT license covers the wrapper code in [`src`](src) and the samples. The wrapper is derived from [Satoru Higa's ofxZmq](https://github.com/satoruhiga/ofxZmq), which doesn't include a license. The libzmq libraries in [`lib`](lib) and the headers [`include/zmq.h`](include/zmq.h) and [`include/zmq_utils.h`](include/zmq_utils.h) are from [ZeroMQ](https://zeromq.org) and are licensed under the [LGPL-3.0](https://github.com/zeromq/zeromq4-1/blob/56b71af22db3232eb57fd72e954e22eeac497b46/COPYING.LESSER) (version 3 or any later version) with a static linking exception. The cppzmq headers [`include/zmq.hpp`](include/zmq.hpp) and [`include/zmq_addon.hpp`](include/zmq_addon.hpp) are MIT licensed by their own authors.
