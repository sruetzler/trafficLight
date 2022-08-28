#include <FS.h>                   //this needs to be first, or it all crashes and burns...
#include "SPIFFS.h"
#include "Arduino.h"
#include "Config.h"

#include <ArduinoJson.h>

const String configFileName="/configTL.json";
const String configVersion="1.0.0";

void convertVersion(String version, int aVersion[3]){
    byte index = 0;
    char* ptr = strtok((char*)version.c_str(), ".");
    while (ptr != NULL && index < 3)
    {
        aVersion[index] = String(ptr).toInt();
        index++;
        ptr = strtok(NULL, ",");
    }
} 

Config::Config(){
    m_enableRequestGreen = true;
    m_enableAutomatic = false;
    m_enableToggle = false;
    m_greenTime = 30;
    m_redTime = 60;    
    m_greenDelayTime = 3;
    m_ssid = "";
    m_passphrase = "";

}

void Config::init(){
    bool initok = SPIFFS.begin();
    if (!(initok)) // Format SPIFS, of not formatted. - Try 1
    {
        Serial.println("SPIFFS Dateisystem formatiert.");
        SPIFFS.format();
        initok = SPIFFS.begin();
    }
    if (!(initok)) // Format SPIFS. - Try 2
    {
        SPIFFS.format();
        initok = SPIFFS.begin();
    }
    if (initok) {
        Serial.println("mounted file system");
        if (SPIFFS.exists(configFileName)) {
            //file exists, reading and loading
            Serial.println("reading config file");
            File configFile = SPIFFS.open(configFileName, "r");
            if (configFile) {
                Serial.println("opened config file");
                size_t size = configFile.size();
                // Allocate a buffer to store contents of the file.

                StaticJsonDocument<512> doc;
                DeserializationError error = deserializeJson(doc, configFile);

                if (error){
                    Serial.println(F("Failed to read file, using default configuration"));
                }else{
                    String version = String((const char*) doc["version"]);
                    int aActVersion[3];
                    int aExpectdVersion[3];
                    convertVersion(configVersion, aExpectdVersion);
                    convertVersion(version, aActVersion);
                    if (aActVersion[0] > aExpectdVersion[0]){
                        Serial.println("configuration file to new");
                    }else{
                        if (aActVersion[0] < aExpectdVersion[0]){
                            //conversation needed
                        }else{
                            m_enableRequestGreen = bool(doc["enableRequestGreen"]);
                            m_enableAutomatic = bool(doc["enableAutomatic"]);
                            m_enableToggle = bool(doc["enableToggle"]);
                            m_greenTime = int(doc["greenTime"]);
                            m_redTime = int(doc["redTime"]);    
                            m_greenDelayTime = int(doc["greenDelayTime"]);
                            m_ssid = String((const char*) doc["ssid"]);
                            m_passphrase = String((const char*) doc["passphrase"]);
                        }
                    }
                }
                configFile.close();
            }
        }
    } else {
        Serial.println("failed to mount FS");
    }  
    Serial.println("Config::init ready");
}


void Config::saveConfig(){
  Serial.println("saving config");
    StaticJsonDocument<256> doc;
    doc["version"] = configVersion;
    doc["enableRequestGreen"] = m_enableRequestGreen;
    doc["enableAutomatic"] = m_enableAutomatic;
    doc["enableToggle"] = m_enableToggle;
    doc["greenTime"] = m_greenTime;
    doc["redTime"] = m_redTime;
    doc["greenDelayTime"] = m_greenDelayTime;
    doc["ssid"] = m_ssid;
    doc["passphrase"] = m_passphrase;

    File configFile = SPIFFS.open(configFileName, "w");
    if (!configFile) {
      Serial.println("failed to open config file for writing");
    }
    if (serializeJson(doc, configFile) == 0) {
      Serial.println(F("Failed to write to file"));
    }

    configFile.close();

}

bool Config::getEnableRequestGreen(){
    return m_enableRequestGreen;
}
bool Config::getEnableAutomatic(){
    return m_enableAutomatic;
}
bool Config::getEnableToggle(){
    return m_enableToggle;
}
int Config::getGreenTime(){
    return m_greenTime;
}
int Config::getRedTime(){
    return m_redTime;
}
int Config::getGreenDelayTime(){
    return m_greenDelayTime;
}
String Config::getSsid(){
    return m_ssid;
}
String Config::getPassphrase(){
    return m_passphrase;
}
void Config::setEnableRequestGreen(bool value){
    m_enableRequestGreen = value;
    if (m_enableRequestGreen == true){
        m_enableToggle = false;
    }
    saveConfig();
}
void Config::setEnableAutomatic(bool value){
    m_enableAutomatic = value;
    saveConfig();
}
void Config::setEnableToggle(bool value){
    m_enableToggle = value;
    if (m_enableToggle == true){
        m_enableRequestGreen = false;
    }
    saveConfig();
}
void Config::setGreenTime(int value){
    m_greenTime = value;
    saveConfig();
}
void Config::setRedTime(int value){
    m_redTime = value;
    saveConfig();
}
void Config::setGreenDelayTime(int value){
    m_greenDelayTime = value;
    saveConfig();
}
void Config::setSsid(String value){
    m_ssid = value;
    saveConfig();
}
void Config::setPassphrase(String value){
    m_passphrase = value;
    saveConfig();
}

