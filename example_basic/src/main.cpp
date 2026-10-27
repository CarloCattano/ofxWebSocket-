#include "ofAppGLFWWindow.h"
#include "ofMain.h"

#include "ClientApp.h"
#include "ControlServer.h"
#include "ServerApp.h"

int main() {
    ControlServer controlServer;
    if (!controlServer.start()) {
        return 1;
    }

    ofGLFWWindowSettings serverSettings;
    serverSettings.setSize(480, 420);
    serverSettings.setPosition(glm::vec2(40, 100));
    serverSettings.title = "ofxWebSocket - Server Controls";
    auto serverWindow = ofCreateWindow(serverSettings);

    ofGLFWWindowSettings clientSettings;
    clientSettings.setSize(480, 420);
    clientSettings.setPosition(glm::vec2(560, 100));
    clientSettings.title = "ofxWebSocket - Client";
    auto clientWindow = ofCreateWindow(clientSettings);

    auto serverApp = std::make_shared<ServerApp>(controlServer);
    auto clientApp = std::make_shared<ClientApp>();
    ofRunApp(serverWindow, serverApp);
    ofRunApp(clientWindow, clientApp);
    ofRunMainLoop();

    controlServer.stop();
    return 0;
}
