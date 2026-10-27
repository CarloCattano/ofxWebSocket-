meta:
	ADDON_NAME = ofxWebSocket
	ADDON_DESCRIPTION = WebSocket client addon for openFrameworks using WebSocket++ and Boost.Asio
	ADDON_AUTHOR = Carlo Cattano
	ADDON_TAGS = "websocket" "networking" "asio"
	ADDON_URL = https://github.com/CarloCattano/ofxWebSocket-

common:
	# WebSocket++ is header-only; the submodule is included with this addon.
	ADDON_INCLUDES = src libs/websocketpp
	ADDON_SOURCES = src/ofxWebSocket.cpp

linux64:
	# Boost.Asio uses the system threading primitives on Linux.
	ADDON_LDFLAGS += -pthread
