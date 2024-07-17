#ifndef LED_PWM_CONTROLLER_H
#define LED_PWM_CONTROLLER_H

#include <Arduino.h>

class LEDPWMController {
  public:
    LEDPWMController(int redPin, int greenPin, int bluePin);
    void begin();
    void setColor(int r, int g, int b);

  private:
    int _redPin;
    int _greenPin;
    int _bluePin;
};

#endif // LED_PWM_CONTROLLER_H
