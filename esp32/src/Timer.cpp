#include "Arduino.h"
#include "Timer.h"

Timer::Timer(){
    m_event1Time = 0;
    m_event2Time = 0;
}

void Timer::startTimer1(int _millis, void (*pFunc)(void* id), void* id){
    Serial.println("start timer 1 " + _millis);
    m_event1Time = millis() + (unsigned long) _millis;
    m_func1 = pFunc;
    m_id1 = id;
}
void Timer::startTimer2(int _millis, void (*pFunc)(void* id), void* id){
    Serial.println("start timer 2 " + _millis);
    m_event2Time = millis() + (unsigned long) _millis;
    m_func2 = pFunc;
    m_id2 = id;
}
void Timer::stopTimer1(){
    m_event1Time = 0;
}
void Timer::stopTimer2(){
    m_event2Time = 0;
}

void Timer::loop(){
    unsigned long now = millis();
    if (m_event1Time && m_event1Time <= now && m_func1){
        m_event1Time=0;
        m_func1(m_id1);
    }
    if (m_event2Time && m_event2Time <= now && m_func2) {
        m_event2Time=0;
        m_func2(m_id2);
    }
}
