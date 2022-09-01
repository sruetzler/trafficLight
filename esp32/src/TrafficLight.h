
#ifndef TRAFFIC_LIGHT_H__
#define TRAFFIC_LIGHT_H__

#include "Config.h"
#include "Arduino.h"
///#include "Timer.h"

class TrafficLightChanged {
    public:
        virtual void stateChange(String state) = 0;
};

class TrafficLight {
    private:
        unsigned long m_event1Time;
        unsigned long m_event2Time;
        Config* m_config;
        bool m_greenOn;
        int m_lastButtonValue;
        TrafficLightChanged* m_trafficLightChanged;
        int m_timerId;
        int m_requestTimerId;
    public:
        TrafficLight(Config* config);

        void setGreen();
        void setRed();
        bool isGreen();
        String toggle();
        void configChanged();

        void registerTrafficLightChanged(TrafficLightChanged* trafficLightChanged);

        void loop();

        void onTimer1();
        void onTimer2();

        // static void onTimer1(void* id);
        // static void onTimer2(void* id);
    private:
        void start();
        void _setGreen();
        void _setRed();

        void buttonPressed();

        void startTimer1(int millis);
        void stopTimer1();
        void startTimer2(int millis);
        void stopTimer2();

};

#endif //TRAFFIC_LIGHT_H__