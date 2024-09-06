#pragma once
#define THRESHOLD_COUNT 100
#define THRESHOLD_DELAY 10

enum MOTOR_STATE {
    STOPPED,
    FORWARD,
    BACKWARD
};

class MotorDriver {
    public: 
        MotorDriver(int forwardPin, int backwardPin, int thresholdPin, int thresholdValue);
        void loop();
    
        void forward();
        void backward();
        void reverse();
        void stop();
        void setThreshold(int threshold);
    
        MOTOR_STATE getState();
        int thresholdAvg();
        bool overrideJamDetection = false;
        bool debug = false;
        

    private:
        int forwardPin;
        int backwardPin;
        int thresholdPin;
        int thresholdValue;
        int lastThreshold;
        long lastStateChange;

        MOTOR_STATE state;

        long lastThresholdMeasurement;

        bool stalled = false;
        int stallRetries = 0;
        long lastStall = 0;

        int thresholds[THRESHOLD_COUNT];
        int thresholdIndex = 0;
        void insertThreshold(int threshold);
        void applyState(MOTOR_STATE state);
        bool shouldStall();
        void setState(MOTOR_STATE state);
    
};


