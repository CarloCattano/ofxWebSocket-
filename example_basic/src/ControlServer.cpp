#include "ControlServer.h"

#include "ofMain.h"

#include <boost/asio/post.hpp>

bool ControlServer::start() {
    if (running.exchange(true)) {
        return true;
    }

    try {
        server.clear_access_channels(websocketpp::log::alevel::all);
        server.init_asio();
        server.set_open_handler([this](ConnectionHandle handle) {
            std::lock_guard<std::mutex> lock(connectionsMutex);
            connections.insert(handle);
        });
        server.set_close_handler([this](ConnectionHandle handle) {
            std::lock_guard<std::mutex> lock(connectionsMutex);
            connections.erase(handle);
        });
        server.listen(port);
        server.start_accept();
        serverThread = std::thread([this] { server.run(); });
    } catch (const std::exception &exception) {
        running = false;
        ofLogError("ControlServer")
            << "Unable to listen on port " << port << ": " << exception.what();
        return false;
    }

    ofLogNotice("ControlServer") << "Listening on ws://localhost:" << port;
    return true;
}

void ControlServer::broadcast(const std::string &payload) {
    if (!running) {
        return;
    }

    boost::asio::post(server.get_io_service(), [this, payload] {
        std::lock_guard<std::mutex> lock(connectionsMutex);
        for (auto iterator = connections.begin(); iterator != connections.end();) {
            websocketpp::lib::error_code error;
            server.send(*iterator, payload, websocketpp::frame::opcode::text, error);
            if (error) {
                iterator = connections.erase(iterator);
            } else {
                ++iterator;
            }
        }
    });
}

void ControlServer::stop() {
    if (!running.exchange(false)) {
        return;
    }

    server.stop_listening();
    server.stop();
    if (serverThread.joinable()) {
        serverThread.join();
    }

    std::lock_guard<std::mutex> lock(connectionsMutex);
    connections.clear();
}
