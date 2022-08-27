
#ifndef TRAFFIC_LIGHT_H__
#define TRAFFIC_LIGHT_H__

#include "Config.h"
#include "Arduino.h"

class TrafficLightChanged {
    public:
        virtual void stateChange(String state) = 0;
};

class TrafficLight {
    private:
        Config* m_config;
        bool m_greenOn;
        int m_lastButtonValue;
        TrafficLightChanged* m_trafficLightChanged;
    public:
        TrafficLight(Config* config);

        void setGreen();
        void setRed();
        void toggle();
        void configChanged();

        void registerTrafficLightChanged(TrafficLightChanged* trafficLightChanged);

        void loop();
    private:
        void start();
        void _setGreen();
        void _setRed();

        void buttonPressed();
};

#endif //TRAFFIC_LIGHT_H__