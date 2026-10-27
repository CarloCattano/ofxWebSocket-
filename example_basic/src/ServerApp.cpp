#include "ServerApp.h"

#include <algorithm>
#include <sstream>

ServerApp::ServerApp(ControlServer &controlServer) : controlServer(controlServer) {}

void ServerApp::setup() {
    ofBackground(28, 32, 40);
    broadcastControls();
}

void ServerApp::update() {
    if (ofGetElapsedTimef() - lastBroadcast >= 1.0f / 30.0f) {
        broadcastControls();
    }
}

void ServerApp::draw() {
    ofBackground(28, 32, 40);
    ofSetColor(235, 240, 250);
    ofDrawBitmapString("SERVER CONTROLS", 20, 28);
    ofDrawBitmapString("Drag the circle or use the arrow keys", 20, 52);
    ofDrawBitmapString("[ / ] changes radius    q quits", 20, 74);

    ofSetColor(80, 90, 110);
    ofDrawRectangle(20, 95, ofGetWidth() - 40, ofGetHeight() - 120);
    ofSetColor(80, 180, 255);
    ofDrawCircle(x, y, radius);

    ofSetColor(235, 240, 250);
    ofDrawBitmapString("x: " + ofToString(x), 20, ofGetHeight() - 62);
    ofDrawBitmapString("y: " + ofToString(y), 20, ofGetHeight() - 44);
    ofDrawBitmapString("radius: " + ofToString(radius), 20, ofGetHeight() - 26);
}

void ServerApp::keyPressed(int key) {
    switch (key) {
    case OF_KEY_LEFT:
        updatePosition(x - 10, y);
        break;
    case OF_KEY_RIGHT:
        updatePosition(x + 10, y);
        break;
    case OF_KEY_UP:
        updatePosition(x, y - 10);
        break;
    case OF_KEY_DOWN:
        updatePosition(x, y + 10);
        break;
    case '[':
        radius = std::max(10, radius - 5);
        break;
    case ']':
        radius = std::min(120, radius + 5);
        break;
    case 'q':
        ofExit();
        break;
    default:
        break;
    }
}

void ServerApp::mousePressed(int x, int y, int) { updatePosition(x, y); }

void ServerApp::mouseDragged(int x, int y, int) { updatePosition(x, y); }

void ServerApp::updatePosition(int nextX, int nextY) {
    x = std::clamp(nextX, 20 + radius, ofGetWidth() - 20 - radius);
    y = std::clamp(nextY, 95 + radius, ofGetHeight() - 40 - radius);
}

void ServerApp::broadcastControls() {
    lastBroadcast = ofGetElapsedTimef();

    const auto sendValue = [this](const char *id, int value) {
        std::ostringstream payload;
        payload << "{\"id\":\"" << id << "\",\"param\":" << value << ",\"total_parameters\":3}";
        controlServer.broadcast(payload.str());
    };

    sendValue("x", x);
    sendValue("y", y);
    sendValue("radius", radius);
}
