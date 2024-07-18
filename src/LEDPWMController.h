#ifndef LED_PWM_CONTROLLER_H
#define LED_PWM_CONTROLLER_H

#include "LEDConfig.h" // LEDConfig.hをインクルード
#include <Arduino.h>

class LEDPWMController {
  public:
    LEDPWMController();
    void begin();
    void setColor(int r, int g, int b);
    void update();

  private:
    int _redPin;
    int _greenPin;
    int _bluePin;

    // 現在の色
    int _currentR;
    int _currentG;
    int _currentB;

    // 目標の色
    int _targetR;
    int _targetG;
    int _targetB;

    // 色遷移のための変数
    unsigned long _startTime;
    unsigned long _transitionTime;
    bool _transitioning; // 色遷移中かどうか
};

#endif // LED_PWM_CONTROLLER_H
