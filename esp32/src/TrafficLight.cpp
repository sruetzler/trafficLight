
#include "Arduino.h"
#include "TrafficLight.h"

const int REQUEST_BUTTON1 = 34;
const int REQUEST_BUTTON2 = 27;
const int RED_LAMP = 32;
const int GREEN_LAMP = 25;


TrafficLight::TrafficLight(Config* config){
    m_config = config;
    m_lastButtonValue = 0;
    m_trafficLightChanged = NULL;
    pinMode(REQUEST_BUTTON1, INPUT);  
    pinMode(REQUEST_BUTTON2, INPUT);  
    pinMode(RED_LAMP, OUTPUT);  
    pinMode(GREEN_LAMP, OUTPUT);  
    _setRed();
    start();
}

void TrafficLight::start(){

}

void TrafficLight::registerTrafficLightChanged(TrafficLightChanged* trafficLightChanged){
    m_trafficLightChanged = trafficLightChanged;
}


void TrafficLight::loop(){
    bool actButtonValue = (!!digitalRead(REQUEST_BUTTON1)) || (!!digitalRead(REQUEST_BUTTON2));     
    if (m_lastButtonValue != actButtonValue){
        m_lastButtonValue = actButtonValue;
        if (m_lastButtonValue) buttonPressed();
    }
}

void TrafficLight::buttonPressed(){
    toggle();
}



void TrafficLight::toggle(){
    if (m_greenOn) setRed();
    else setGreen();
}
void TrafficLight::setGreen(){
    _setGreen();
}
void TrafficLight::setRed(){
    _setRed();
}
void TrafficLight::configChanged(){

}

void TrafficLight::_setGreen(){
    digitalWrite(RED_LAMP, LOW);
    digitalWrite(GREEN_LAMP, HIGH);
    m_greenOn = true;
    if (m_trafficLightChanged) m_trafficLightChanged->stateChange("green");
}
void TrafficLight::_setRed(){
    digitalWrite(GREEN_LAMP, LOW);
    digitalWrite(RED_LAMP, HIGH);
    m_greenOn = false;
    if (m_trafficLightChanged) m_trafficLightChanged->stateChange("red");
}

