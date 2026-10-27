#pragma once

#include "ofMain.h"
#include "ofxWebSocket.h"

#include <mutex>

class ClientApp : public ofBaseApp {
  public:
    void setup() override;
    void draw() override;
    void exit() override;

  private:
    ofxWebSocket webSocket;
    int objectX = 240;
    int objectY = 210;
    int objectRadius = 50;
    std::string lastField;
    std::mutex dataMutex;
};
