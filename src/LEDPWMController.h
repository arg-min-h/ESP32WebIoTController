#ifndef LED_PWM_CONTROLLER_H
#define LED_PWM_CONTROLLER_H

#include "LEDConfig.h" // LEDConfig.hをインクルード
#include <Arduino.h>

class LEDPWMController {
  public:
    LEDPWMController();
    void begin();
    void update();
    void setColor(float r, float g, float b, float speed);

  private:
    unsigned long _lastUpdateTime;
    float _currentR, _currentG, _currentB;
    float _targetR, _targetG, _targetB;
    bool _transitioning;
    float _startR, _startG, _startB;
    float _transitionDuration;
    float _transitionSpeed;

    int _redPin, _greenPin, _bluePin;
};

#endif // LED_PWM_CONTROLLER_H
