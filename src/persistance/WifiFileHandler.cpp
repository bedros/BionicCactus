#include "persistance/WifiFileHandler.hpp"

using namespace Persistance;

WifiFileHandler::WifiFileHandler() : _configured(false),
    _hostname{'b', 'i', 'o', 'n', 'i', 'c', '_', 'c', 'a', 'c', 't', 'u', 's'} { 
    _password[0] = 0;
    _ssid[0] = 0; 
}

void WifiFileHandler::load() { 
    auto file = SPIFFS.open(_filePath, "r"); 
    if (!file) {
        _configured = false;
        return; 
    }

    StaticJsonBuffer<CONST_FILE_LENGTH> jsonBuffer; 
    auto &wifiRoot = jsonBuffer.parseObject(file); file.close();

    strncpy(_ssid, wifiRoot[CONST_SSID], 100); 
    strncpy(_password, wifiRoot[CONST_PASSWORD], 100); 
    const char *savedHostname = wifiRoot[CONST_HOSTNAME];
    if (savedHostname) {
        strncpy(_hostname, wifiRoot[CONST_HOSTNAME], 100);
    }
    _configured = true; 
}

void WifiFileHandler::save() {
    StaticJsonBuffer<CONST_FILE_LENGTH> jsonBuffer;
    auto &wifiRoot = jsonBuffer.createObject();
    wifiRoot[CONST_SSID] = _ssid;
    wifiRoot[CONST_PASSWORD] = _password;
    wifiRoot[CONST_HOSTNAME] = _hostname;
    auto file = SPIFFS.open(_filePath, "w");
    wifiRoot.printTo(file);
    file.close();
    _configured = true;
}

bool WifiFileHandler::isConfigured() {
    return _configured;
}

void WifiFileHandler::setConfigured(bool configured) {
    _configured = configured;
}

const char *WifiFileHandler::getSSID() {
    return _ssid;
}

void WifiFileHandler::setSSID(const char *ssid) {
    strncpy(_ssid, ssid, 100);
}

const char *WifiFileHandler::getPassword() {
    return _password;
}

void WifiFileHandler::setPassword(const char *password) {
    strncpy(_password, password, 100);
}

const char *WifiFileHandler::getHostname() {
    return _hostname;
}

void WifiFileHandler::setHostname(const char *hostname) {
    strncpy(_hostname, hostname, 100);
}
