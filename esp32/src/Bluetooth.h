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

const String CONTROLER_VERSION = "1.0.0";
const String PROTOCOL_VERSION = "1.0.0";

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
        void onMessage(String& message);

        void sendMessage(const String& command);
        void sendMessage(const String& command, String& param);
        void _sendMessage(const String& message);

        void version(const String& command, const String& messageId);
        void protocolVersion(const String& command, const String& messageId, const String& param);
        void setGreenTime(const String& command, const String& messageId, const String& param);
        void setRedTime(const String& command, const String& messageId, const String& param);
        void setGreenDelayTime(const String& command, const String& messageId, const String& param);
        void getGreenTime(const String& command, const String& messageId);
        void getRedTime(const String& command, const String& messageId);
        void getGreenDelayTime(const String& command, const String& messageId);
        void setButtonMode(const String& command, const String& messageId, const String& param);
        void setAutomaticMode(const String& command, const String& messageId, const String& param);
        void getButtonMode(const String& command, const String& messageId);
        void getAutomaticMode(const String& command, const String& messageId);
        void getState(const String& command, const String& messageId);
        void setState(const String& command, const String& messageId, const String& param);
        void toggle(const String& command, const String& messageId);

};





#endif //BLUETOOTH_H__