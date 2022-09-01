#ifndef TIMER_H__
#define TIMER_H__

class Timer{
    private:
        unsigned long m_event1Time;
        unsigned long m_event2Time;
        void (*m_func1)(void* id);
        void (*m_func2)(void* id);
        void* m_id1;
        void* m_id2;
    public:
        Timer();
        void startTimer1(int millis, void (*pFunc)(void* id), void* id);
        void stopTimer1();
        void startTimer2(int millis, void (*pFunc)(void* id), void* id);
        void stopTimer2();

        void loop();

};

#endif //TIMER_H__