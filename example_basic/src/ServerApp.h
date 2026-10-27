#pragma once

#include "ControlServer.h"
#include "ofMain.h"

class ServerApp : public ofBaseApp {
  public:
    explicit ServerApp(ControlServer &controlServer);

    void setup() override;
    void update() override;
    void draw() override;
    void keyPressed(int key) override;
    void mousePressed(int x, int y, int button) override;
    void mouseDragged(int x, int y, int button) override;

  private:
    void updatePosition(int x, int y);
    void broadcastControls();

    ControlServer &controlServer;
    int x = 240;
    int y = 210;
    int radius = 50;
    float lastBroadcast = 0.0f;
};
