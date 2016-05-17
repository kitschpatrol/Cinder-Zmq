# Cinder-Zmq
---

**A wrapper for using the [ZeroMQ](http://zeromq.org) distributed messaging system with [Cinder](https://libcinder.org).**


This is a minimally-invasive Cinder port of [Satoru Higa's ofxZmq](https://github.com/satoruhiga/ofxZmq) openFrameworks addon. See also [Bruce Lane's Cinder-ZeroMQ](https://github.com/brucelane/Cinder-ZeroMQ) Cinder block.

Dependencies are included as static libraries.

Due to some issues with building on Windows via the 4.1.4 release, the libraries bundled in this block are from the [4.1.x stable release branch](https://github.com/zeromq/zeromq4-1/commit/56b71af22db3232eb57fd72e954e22eeac497b46).

###TODO
- Testing and reevaluate functions using ci::Buffer.
- Test on Windows.
- More samples.
- Expand readme.