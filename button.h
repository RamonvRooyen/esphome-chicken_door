#pragma once
#include <optional>
#include <Arduino.h>

#if defined(__arm__) || defined(ESP8266) || defined(ESP32)
    #include <functional>
    using fptr = std::function<void()>;
#else
    typedef void (*fptr)();
#endif


class Button{
    public: 
        Button(int pin);
        void onDown(const fptr& callback);
        void onUp(const fptr& callback);
        void loop();
        bool isPressed();
        bool debug = false;

    private:
        bool buttonPressed = false;
        int pin;
        std::optional<fptr> downCallback;
        std::optional<fptr> upCallback;
};