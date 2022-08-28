#ifndef BLUETOOTH_H__
#define BLUETOOTH_H__

/*
Version 1.0.0

Request format:
COMMAND|MSGID[|PARAMETER]\n
Multiple Parameter seprated by #
Answer format:
ACK|MSIG[|DATA]\n      //Success
NACK|MSIG[|DATA]\n     //Failure

MSIG is increase on each message

Common Commands:
--------------------------------------------
version: Read the app or controler version.
no param
Result: Versionnumber X.Y.Z

--------------------------------------------
protocolVersion: Read the version of the protocol
Param: own version
Result: Versionnumber X.Y.Z

X - increase on incompatible changes
Y - increase on compatible changes
Z - increase on bug fixes


############################################
Commands from App
--------------------------------------------
setGreenTime: Set green time   
Param: Time in seconds

Return:
no data

Example: setGreenTime|4711|5\n   --> ACK|4711\n

--------------------------------------------
setRedTime: set red time
Param: Time in seconds

Return:
no data

--------------------------------------------
setGreenDelayTime: Set green delay time
Param: Time in seconds

Return:
no data

--------------------------------------------
getGreenTime: Get green time
no param

Return: Time in seconds

Example:
getGreenTime|42\n   -->   ACK|42|5\n

--------------------------------------------
getRedTime: get red time
no param

Return: Time in seconds

--------------------------------------------
getGreenDelayTime: Get green delay time
no param

Return: Time in seconds

--------------------------------------------
setButtonMode: Set mode of the button
Param: requestGreen, toggle or off

Return:
no data

Example:
setButtonMode|99|requestGreen\n    -->    ACK|99\n

--------------------------------------------
setAutomaticMode: Enable or disable automatic mode
Param: true or false

Return:
no data

--------------------------------------------
getButtonMode: Get the mode of the button
no param

Return:
requestGreen or toggle or off

--------------------------------------------
getAutomaticMode: Get the state automatic mode
no param

Return:
true or false

--------------------------------------------
getState: Get the actual state of the traffic light
no param

Return:
red or green

--------------------------------------------
setState: Set the actual state of the traffic light
Param: red or green

Return:
no data

--------------------------------------------
toggle : Toogle the actual state of the traffic light
no param

Return:
New state: red or green

############################################
Commands from Controler
--------------------------------------------
actState: Send the actual state if the state changes
Param: red or green

Return:
no data

Example:
actState|0815|red\n   -->  ACK|0815\n

*/

#include "Config.h"
#include "TrafficLight.h"
#include "BluetoothSerial.h"

class Bluetooth : public TrafficLightChanged {
    private:
        Config* m_config;
        TrafficLight* m_trafficLight;
        BluetoothSerial m_serialBT;
        char* m_nextMessage;
        int m_nextMessageOffset;
        long int m_nextMessageId;

    public:
        Bluetooth(Config* config, TrafficLight* trafficLight);

        void loop();

        virtual void stateChange(String state);

    private:
        void receive();
        void onMessage(const String& message);

        void sendMessage(const String& command);
        void sendMessage(const String& command, const String& param);
        void sendMessageWithMsgId(const String& command, const String& msgId);
        void sendMessageWithMsgId(const String& command, const String& param, const String& msgId);
        void _sendMessage(const String& message);

        void version(const String& msgId);
        void protocolVersion(const String& msgId, const String& param);
        void setGreenTime(const String& msgId, const String& param);
        void setRedTime(const String& msgId, const String& param);
        void setGreenDelayTime(const String& msgId, const String& param);
        void getGreenTime(const String& msgId);
        void getRedTime(const String& msgId);
        void getGreenDelayTime(const String& msgId);
        void setButtonMode(const String& msgId, const String& param);
        void setAutomaticMode(const String& msgId, const String& param);
        void getButtonMode(const String& msgId);
        void getAutomaticMode(const String& msgId);
        void getState(const String& msgId);
        void setState(const String& msgId, const String& param);
        void toggle(const String& msgId);

};





#endif //BLUETOOTH_H__