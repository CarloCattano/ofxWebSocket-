# ofxWebSocket

An openFrameworks addon that provides a small, callback-based WebSocket client using
[WebSocket++](https://github.com/zaphoyd/websocketpp) and Boost.Asio.

## Features

- Connect to a non-TLS `ws://` endpoint.
- Send text messages.
- Receive text messages through `onMessage`.
- Parse the JSON fields `id`, `param`, and `total_parameters` into `parsedData` when a
  received payload has that shape.
- Use `ofWebSocket` as a compatibility alias for the canonical `ofxWebSocket` class.

## Installation

Clone the addon into the `addons` directory of openFrameworks, including its WebSocket++
submodule:

```sh
git clone --recurse-submodules https://github.com/CarloCattano/ofxWebSocket- \
  openFrameworks/addons/ofxWebSocket
```

For an existing checkout, initialize or refresh the dependency with:

```sh
git submodule update --init --recursive
```

Add `ofxWebSocket` to a project's `addons.make`, or select it with the openFrameworks
Project Generator. The included `example_basic` project shows the expected layout and API.

## Usage

```cpp
#include "ofxWebSocket.h"

ofxWebSocket socket;

void ofApp::setup() {
    socket.onMessage = [](const std::string & message) {
        ofLogNotice("example") << "Received: " << message;
    };
    socket.connect("ws://localhost:9002");
}

void ofApp::keyPressed(int key) {
    if (key == 's') {
        socket.send("hello from openFrameworks");
    }
}

void ofApp::exit() {
    socket.close();
}
```

`onMessage` runs on the WebSocket++ client thread. Keep callbacks short and synchronize
any state shared with the openFrameworks draw/update thread. `close()` should be called
from the application thread. This addon currently uses the non-TLS WebSocket++ client, so
`wss://` endpoints are not supported.

## Dependencies

- openFrameworks.
- Boost.Asio, provided by the Boost installation used by openFrameworks.
- WebSocket++, included as the `libs/websocketpp` Git submodule.

No additional openFrameworks addons are required.

## Compatibility

The addon follows the standard openFrameworks addon layout and requires a C++17-capable
openFrameworks toolchain because the bundled WebSocket++ revision uses C++17 language
features. The repository currently has no platform CI matrix; build `example_basic` with
the openFrameworks release and platform you intend to use before shipping an application.

## License

The addon is available under the [MIT License](license.md). WebSocket++ remains under its
upstream license in the bundled submodule.

## Version history

### 0.1 — unreleased

- First packaged version of the addon.
- Added a standard addon layout, metadata, example, and dependency documentation.
