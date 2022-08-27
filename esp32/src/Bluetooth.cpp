#include "Arduino.h"
#include "Bluetooth.h"

const int MAX_COMMAND_LENGTH = 100;

Bluetooth::Bluetooth(Config* config, TrafficLight* trafficLight){
    m_config = config;
    m_trafficLight = trafficLight;
    m_nextMessage = new char[MAX_COMMAND_LENGTH];
    m_nextMessageOffset = 0;
    m_nextMessageId = 1;

    m_trafficLight->registerTrafficLightChanged(this);

    m_serialBT.begin("Fussgaengerampel"); //Bluetooth device name
}


void Bluetooth::stateChange(String state){
    Serial.println("toogle");
    sendMessage("actState",state);
}

void Bluetooth::loop(){
    if (Serial.available()) {
        m_serialBT.write(Serial.read());
    }
    receive();
}

void Bluetooth::receive(){
    while(m_serialBT.available()){
        int nextChar = m_serialBT.read();
        if (nextChar != '\n') m_nextMessage[m_nextMessageOffset++] = (char)nextChar;
        else {
            m_nextMessage[m_nextMessageOffset++] = 0;
            m_nextMessageOffset=0;
            String message = String(m_nextMessage);
            onMessage(message);
        }
    }
}

void Bluetooth::sendMessage(const String& command){
    String message = command + "|" + m_nextMessageId++ + "\n";
    _sendMessage(message);
}

void Bluetooth::sendMessage(const String& command, const String& param){
    String message = command + "|" + m_nextMessageId++ + "|" + param + "\n";
    _sendMessage(message);
}

void Bluetooth::_sendMessage(const String& message){
    Serial.println("send: " + message);
    m_serialBT.write((const uint8_t *)message.c_str(), message.length());
}

void Bluetooth::onMessage(String& message){
    Serial.println("receive: " + message);
    int index = message.indexOf("|");
    if (index == -1 ) Serial.println("no command found");
    else {
        String command = message.substring(0,index);
        message = message.substring(index+1);
        int index = message.indexOf("|");
        if (index == -1 ) Serial.println("no messageId found");
        else {
            String msgId = message.substring(0,index);
            message = message.substring(index+1);
            if (command === "")
        }
    }
}

void Bluetooth::version(const String& command, const String& messageId){

}
void Bluetooth::protocolVersion(const String& command, const String& messageId, const String& param){

}
void Bluetooth::setGreenTime(const String& command, const String& messageId, const String& param){

}
void Bluetooth::setRedTime(const String& command, const String& messageId, const String& param){

}
void Bluetooth::setGreenDelayTime(const String& command, const String& messageId, const String& param){

}
void Bluetooth::getGreenTime(const String& command, const String& messageId){

}
void Bluetooth::getRedTime(const String& command, const String& messageId){

}
void Bluetooth::getGreenDelayTime(const String& command, const String& messageId){

}
void Bluetooth::setButtonMode(const String& command, const String& messageId, const String& param){

}
void Bluetooth::setAutomaticMode(const String& command, const String& messageId, const String& param){

}
void Bluetooth::getButtonMode(const String& command, const String& messageId){

}
void Bluetooth::getAutomaticMode(const String& command, const String& messageId){

}
void Bluetooth::getState(const String& command, const String& messageId){

}
void Bluetooth::setState(const String& command, const String& messageId, const String& param){

}
void Bluetooth::toggle(const String& command, const String& messageId){

}
