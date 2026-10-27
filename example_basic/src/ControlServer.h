#pragma once

#include <atomic>
#include <cstdint>
#include <mutex>
#include <set>
#include <string>
#include <thread>

#include <websocketpp/config/asio_no_tls.hpp>
#include <websocketpp/server.hpp>

class ControlServer {
  public:
    static constexpr uint16_t port = 9002;

    bool start();
    void broadcast(const std::string &payload);
    void stop();

  private:
    using Server = websocketpp::server<websocketpp::config::asio>;
    using ConnectionHandle = websocketpp::connection_hdl;
    using ConnectionSet = std::set<ConnectionHandle, std::owner_less<ConnectionHandle>>;

    Server server;
    ConnectionSet connections;
    std::mutex connectionsMutex;
    std::thread serverThread;
    std::atomic<bool> running{false};
};
