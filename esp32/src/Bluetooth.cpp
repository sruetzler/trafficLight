#include "Arduino.h"
#include "Bluetooth.h"

const int MAX_COMMAND_LENGTH = 100;
const String CONTROLER_VERSION = "1.0.0";
const String PROTOCOL_VERSION = "1.0.0";

const char* ACK = "ACK";
const char* NACK = "NACK";



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
            m_nextMessage[m_nextMessageOffset] = 0;
            m_nextMessageOffset=0;
            String message = String(m_nextMessage);
            onMessage(message);
        }
        if (m_nextMessageOffset > MAX_COMMAND_LENGTH) m_nextMessageOffset = 0;
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

void Bluetooth::sendMessageWithMsgId(const String& command, const String& msgId){
    String message = command + "|" + msgId + "\n";
    _sendMessage(message);
}
void Bluetooth::sendMessageWithMsgId(const String& command, const String& msgId, const String& param){
    String message = command + "|" + msgId + "|" + param + "\n";
    _sendMessage(message);
}

void Bluetooth::_sendMessage(const String& message){
    Serial.println("send: " + message);
    m_serialBT.write((const uint8_t *)message.c_str(), message.length());
}

void Bluetooth::onMessage(const String& message){
    Serial.println("receive: " + message);
    int index = message.indexOf("|");
    if (index == -1 ) Serial.println("no command found");
    else {
        String command = message.substring(0,index);
        String rest = message.substring(index+1);
        int index = rest.indexOf("|");
        if (index == -1 ) index =rest.length();
        String msgId = rest.substring(0,index);
        if (msgId.length() == 0 ) Serial.println("no messageId found");
        else{
            String param = rest.substring(index+1);
            if (command == "version") version(msgId);
            else if (command == "protocolVersion") protocolVersion(msgId, param);
            else if (command == "setGreenTime") setGreenTime(msgId, param);
            else if (command == "setRedTime") setRedTime(msgId, param);
            else if (command == "setGreenDelayTime") setGreenDelayTime(msgId, param);
            else if (command == "getGreenTime") getGreenTime(msgId);
            else if (command == "getRedTime") getRedTime(msgId);
            else if (command == "getGreenDelayTime") getGreenDelayTime(msgId);
            else if (command == "setButtonMode") setButtonMode(msgId, param);
            else if (command == "setAutomaticMode") setAutomaticMode(msgId, param);
            else if (command == "getButtonMode") getButtonMode(msgId);
            else if (command == "getAutomaticMode") getAutomaticMode(msgId);
            else if (command == "getState") getState(msgId);
            else if (command == "setState") setState(msgId, param);
            else if (command == "toggle") toggle(msgId);
            else sendMessageWithMsgId(NACK, msgId, "unknown comand:" + command);

        }
    }
}

void Bluetooth::version(const String& msgId){
    sendMessageWithMsgId(ACK, msgId, CONTROLER_VERSION);
}
void Bluetooth::protocolVersion(const String& msgId, const String& param){
    sendMessageWithMsgId(ACK, msgId, PROTOCOL_VERSION);
}
void Bluetooth::setGreenTime(const String& msgId, const String& param){
    m_config->setGreenTime(param.toInt());
    m_trafficLight->configChanged();
    sendMessageWithMsgId(ACK, msgId);
}
void Bluetooth::setRedTime(const String& msgId, const String& param){
    m_config->setRedTime(param.toInt());
    m_trafficLight->configChanged();
    sendMessageWithMsgId(ACK, msgId);
}
void Bluetooth::setGreenDelayTime(const String& msgId, const String& param){
    m_config->setGreenDelayTime(param.toInt());
    m_trafficLight->configChanged();
    sendMessageWithMsgId(ACK, msgId);
}
void Bluetooth::getGreenTime(const String& msgId){
    sendMessageWithMsgId(ACK, msgId, String(m_config->getGreenTime()));
}
void Bluetooth::getRedTime(const String& msgId){
    sendMessageWithMsgId(ACK, msgId, String(m_config->getRedTime()));
}
void Bluetooth::getGreenDelayTime(const String& msgId){
    sendMessageWithMsgId(ACK, msgId, String(m_config->getGreenDelayTime()));
}
void Bluetooth::setButtonMode(const String& msgId, const String& param){
    if (param == "requestGreen") m_config->setEnableRequestGreen(true);
    else if (param == "toggle") m_config->setEnableToggle(true);
    else if (param == "off"){
        m_config->setEnableRequestGreen(false);
        m_config->setEnableToggle(false);
    }else{
        sendMessageWithMsgId(NACK, msgId, "unknown mode:" + param);
        return;
    }
    sendMessageWithMsgId(ACK, msgId);

}
void Bluetooth::setAutomaticMode(const String& msgId, const String& param){
    m_config->setEnableAutomatic(param == "true");
    m_trafficLight->configChanged();
    sendMessageWithMsgId(ACK, msgId);
}
void Bluetooth::getButtonMode(const String& msgId){
    String mode;
    if (m_config->getEnableRequestGreen()) mode = "requestGreen";
    else if (m_config->getEnableToggle()) mode = "toggle";
    else mode = "off";
    sendMessageWithMsgId(ACK, msgId, mode);
}
void Bluetooth::getAutomaticMode(const String& msgId){
    sendMessageWithMsgId(ACK, msgId, m_config->getEnableAutomatic()?"true":"false");
}
void Bluetooth::getState(const String& msgId){
    sendMessageWithMsgId(ACK, msgId, m_trafficLight->isGreen()?"green":"red");
}
void Bluetooth::setState(const String& msgId, const String& param){
    if (param == "green") m_trafficLight->setGreen();
    else if (param == "ret") m_trafficLight->setRed();
    else{
        sendMessageWithMsgId(NACK, msgId, "unknown state:" + param);
        return;
    }
    sendMessageWithMsgId(ACK, msgId);
}
void Bluetooth::toggle(const String& msgId){
    sendMessageWithMsgId(ACK, msgId, m_trafficLight->toggle());
}
