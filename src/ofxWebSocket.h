#pragma once

#include "ofMain.h"
#include <atomic>
#include <functional>
#include <string>
#include <thread>

#include <websocketpp/client.hpp>
#include <websocketpp/config/asio_no_tls_client.hpp>

/**
 * A small WebSocket client for openFrameworks.
 *
 * Messages are delivered through onMessage from the WebSocket++ client thread.
 * Call close() from the application thread before destroying the client.
 */
class ofxWebSocket {
  public:
    ofxWebSocket();
    ~ofxWebSocket();

    void connect(const std::string &uri);
    void send(const std::string &message);
    void close();

    std::function<void(const std::string &)> onMessage;

    struct ParsedData {
        std::string id;
        int param = 0;
        int total_parameters = 0;
    };

    ParsedData parsedData;
    std::atomic<bool> isConnected{false};

  private:
    using client = websocketpp::client<websocketpp::config::asio_client>;
    using message_ptr = websocketpp::config::asio_client::message_type::ptr;

    client wsClient;
    websocketpp::connection_hdl connection;
    std::thread clientThread;

    void run();
    void onMessageInternal(websocketpp::connection_hdl hdl, message_ptr msg);
    void onOpen(websocketpp::connection_hdl hdl);
    void onFail(websocketpp::connection_hdl hdl);
    void onClose(websocketpp::connection_hdl hdl);

    void parsePayload(const std::string &payload);
};
