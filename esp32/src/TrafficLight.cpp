
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
    m_event1Time = 0;
    m_event2Time = 0;
    pinMode(REQUEST_BUTTON1, INPUT);  
    pinMode(REQUEST_BUTTON2, INPUT);  
    pinMode(RED_LAMP, OUTPUT);  
    pinMode(GREEN_LAMP, OUTPUT);  
    if (!m_config->getEnableAutomatic())_setRed();
    else {
        m_greenOn = true;
        start();
    }
}

void TrafficLight::start(){
    if (!m_config->getEnableAutomatic()) return;
    startTimer1(m_config->getRedTime());
}

void TrafficLight::registerTrafficLightChanged(TrafficLightChanged* trafficLightChanged){
    m_trafficLightChanged = trafficLightChanged;
}


void TrafficLight::loop(){
    bool button1 = !!digitalRead(REQUEST_BUTTON1);
    bool button2 = !!digitalRead(REQUEST_BUTTON2);
    bool actButtonValue = (button1) || (button2);     
    if (m_lastButtonValue != actButtonValue){
        m_lastButtonValue = actButtonValue;
        if (m_lastButtonValue) buttonPressed();
    }
    unsigned long now = millis();
    if (m_event1Time && m_event1Time <= now){
        m_event1Time=0;
        onTimer1();
    }
    if (m_event2Time && m_event2Time <= now) {
        m_event2Time=0;
        onTimer2();
    }
}

// void TrafficLight::onTimer1(void* id){
//     ((TrafficLight*)id)->_onTimer1();
// }
// void TrafficLight::onTimer2(void* id){
//     ((TrafficLight*)id)->_onTimer1();
// }

void TrafficLight::onTimer1(){
    stopTimer1();
    if (isGreen()){
        _setRed();
        startTimer1(m_config->getRedTime() * 1000);
    }else if(m_config->getEnableAutomatic()){
        _setGreen();
        startTimer1(m_config->getGreenTime() * 1000);
    }
}
void TrafficLight::onTimer2(){
    _setGreen();
    startTimer1(m_config->getGreenTime() * 1000);
}

void TrafficLight::buttonPressed(){
    Serial.println("buttonPressed");
    if (m_config->getEnableToggle()){
        if (m_config->getEnableAutomatic()) onTimer1();
        else{
            if (isGreen()) _setRed();
            else _setGreen();
        }
    }
    if (m_config->getEnableRequestGreen() && !isGreen()){
       startTimer2(m_config->getGreenDelayTime() * 1000);
    }
}

String TrafficLight::toggle(){
    if (m_config->getEnableAutomatic()) onTimer1();
    else{
        if (isGreen()) _setRed();
        else{
            setGreen();
            if (m_config->getEnableRequestGreen()) startTimer1(m_config->getGreenTime() * 1000);
        } 
    }
    return isGreen()?"green":"red";
}
void TrafficLight::setGreen(){
    stopTimer2();
    if (m_config->getEnableAutomatic()){
        m_greenOn = false;
        onTimer1();
    } 
    else _setGreen();
}
void TrafficLight::setRed(){
    if (m_config->getEnableAutomatic()){
        m_greenOn = true;
        onTimer1();
    } 
    else _setRed();
}

bool TrafficLight::isGreen(){
    return m_greenOn;
}

void TrafficLight::configChanged(){
    stopTimer1();
    stopTimer2();
    setRed();
    start();
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

void TrafficLight::startTimer1(int _millis){
    Serial.println("start timer 1 " + _millis);
    m_event1Time = millis() + (unsigned long) _millis;
}
void TrafficLight::startTimer2(int _millis){
    Serial.println("start timer 2 " + _millis);
    m_event2Time = millis() + (unsigned long) _millis;
}
void TrafficLight::stopTimer1(){
    m_event1Time = 0;
}
void TrafficLight::stopTimer2(){
    m_event2Time = 0;
}

