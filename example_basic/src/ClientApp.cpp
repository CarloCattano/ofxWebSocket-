#include "ClientApp.h"

#include <algorithm>

void ClientApp::setup() {
    webSocket.onMessage = [this](const std::string &) {
        const ofxWebSocket::ParsedData data = webSocket.parsedData;
        std::lock_guard<std::mutex> lock(dataMutex);
        lastField = data.id;
        if (data.id == "x") {
            objectX = data.param;
        } else if (data.id == "y") {
            objectY = data.param;
        } else if (data.id == "radius") {
            objectRadius = data.param;
        }
    };
    webSocket.connect("ws://localhost:9002");
}

void ClientApp::draw() {
    int x;
    int y;
    int radius;
    std::string field;
    {
        std::lock_guard<std::mutex> lock(dataMutex);
        x = objectX;
        y = objectY;
        radius = objectRadius;
        field = lastField;
    }

    ofBackground(18, 22, 30);
    ofSetColor(235, 240, 250);
    ofDrawBitmapString("CLIENT WINDOW", 20, 28);
    ofDrawBitmapString("Receiving controls through ofxWebSocket", 20, 52);

    x = std::clamp(x, 20 + radius, ofGetWidth() - 20 - radius);
    y = std::clamp(y, 95 + radius, ofGetHeight() - 40 - radius);
    radius = std::clamp(radius, 10, 120);

    ofSetColor(255, 150, 70);
    ofDrawCircle(x, y, radius);
    ofSetColor(235, 240, 250);
    ofDrawBitmapString("Last field: " + field, 20, ofGetHeight() - 44);
    ofDrawBitmapString("Press q in the server window to quit", 20, ofGetHeight() - 26);
}

void ClientApp::exit() { webSocket.close(); }
