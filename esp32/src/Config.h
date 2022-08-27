#ifndef CONFIG_H__
#define CONFIG_H__

class Config {
    private:
        //Mode of the button
        //only one of both is possible, or both false to disable button to completly
        bool m_enableRequestGreen; //request green 
        bool m_enableToggle;       //toggle colour

        bool m_enableAutomatic;    //swith colour automatically, if enable the buttion still works
        int m_greenTime;            //time how long the green lamp is on (for each mode)
        int m_redTime;              //time how long the red lamp is on (only for automatic mode)
        int m_greenDelayTime;       //time how long it takes that the traffic light switch to green after pressing the button (only in mode requestGreen)
        String m_ssid;
        String m_passphrase;

    private:
        void saveConfig();
    public:
        Config();
        void init();

        bool getEnableRequestGreen();
        bool getEnableAutomatic();
        bool getEnableToggle();
        int getGreenTime();
        int getRedTime();
        int getGreenDelayTime();
        String getSsid();
        String getPassphrase();

        void setEnableRequestGreen(bool value);
        void setEnableAutomatic(bool value);
        void setEnableToggle(bool value);
        void setGreenTime(int value);
        void setRedTime(int value);
        void setGreenDelayTime(int value);
        void setSsid(String value);
        void setPassphrase(String value);
};

#endif //CONFIG_H__