#include "ofxWebSocket.h"

using websocketpp::lib::bind;
using websocketpp::lib::placeholders::_1;
using websocketpp::lib::placeholders::_2;

ofxWebSocket::ofxWebSocket() {
    wsClient.clear_access_channels(websocketpp::log::alevel::all);
    wsClient.init_asio();

    wsClient.set_message_handler(bind(&ofxWebSocket::onMessageInternal, this, _1, _2));
    wsClient.set_open_handler(bind(&ofxWebSocket::onOpen, this, _1));
    wsClient.set_fail_handler(bind(&ofxWebSocket::onFail, this, _1));
    wsClient.set_close_handler(bind(&ofxWebSocket::onClose, this, _1));
}

ofxWebSocket::~ofxWebSocket() { close(); }

void ofxWebSocket::connect(const std::string &uri) {
    if (clientThread.joinable()) {
        close();
    }

    wsClient.reset();

    websocketpp::lib::error_code ec;
    auto con = wsClient.get_connection(uri, ec);
    if (ec) {
        ofLogError("ofxWebSocket") << "Connection error: " << ec.message();
        return;
    }

    connection = con->get_handle();
    wsClient.connect(con);

    clientThread = std::thread(&ofxWebSocket::run, this);
}

void ofxWebSocket::run() {
    try {
        wsClient.run();
    } catch (const std::exception &e) {
        ofLogError("ofxWebSocket") << "Run exception: " << e.what();
    }
}

void ofxWebSocket::parsePayload(const std::string &payload) {
    if (payload.empty()) {
        ofLogError("ofxWebSocket") << "Empty payload received.";
        return;
    }
    if (payload[0] != '{') {
        ofLogError("ofxWebSocket") << "Invalid payload format: " << payload;
        return;
    }

    ofJson json;

    try {
        json = ofJson::parse(payload);
    } catch (const std::exception &e) {
        ofLogError("ofxWebSocket") << "JSON parse error: " << e.what();
        return;
    }

    parsedData.id = json.value("id", std::string{});
    parsedData.param = json.value("param", 0);
    parsedData.total_parameters = json.value("total_parameters", 0);
}

void ofxWebSocket::onMessageInternal(websocketpp::connection_hdl, message_ptr msg) {
    const std::string payload = msg->get_payload();
    ofLogVerbose("ofxWebSocket") << "Message received: " << payload;

    parsePayload(payload);
    if (onMessage) {
        onMessage(payload);
    }
}

void ofxWebSocket::onOpen(websocketpp::connection_hdl) {
    isConnected = true;
    ofLogNotice("ofxWebSocket") << "Connection opened.";
}

void ofxWebSocket::onFail(websocketpp::connection_hdl) {
    isConnected = false;
    ofLogError("ofxWebSocket") << "Connection failed.";
}

void ofxWebSocket::onClose(websocketpp::connection_hdl) {
    isConnected = false;
    ofLogNotice("ofxWebSocket") << "Connection closed.";
}

void ofxWebSocket::send(const std::string &message) {
    if (!isConnected) {
        ofLogWarning("ofxWebSocket") << "Cannot send message: Not connected.";
        return;
    }

    websocketpp::lib::error_code ec;
    wsClient.send(connection, message, websocketpp::frame::opcode::text, ec);
    if (ec) {
        ofLogError("ofxWebSocket") << "Send failed: " << ec.message();
    }
}

void ofxWebSocket::close() {
    if (isConnected) {
        websocketpp::lib::error_code ec;
        wsClient.close(connection, websocketpp::close::status::normal, "Closing", ec);
        if (ec) {
            ofLogError("ofxWebSocket") << "Close failed: " << ec.message();
        }
    } else if (clientThread.joinable()) {
        wsClient.stop();
    }

    if (clientThread.joinable()) {
        clientThread.join();
    }

    isConnected = false;
}
