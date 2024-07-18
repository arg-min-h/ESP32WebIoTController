#include "LEDPWMController.h"
#include "LEDConfig.h" // LEDConfig.hをインクルード

LEDPWMController::LEDPWMController()
    : _redPin(RED_PIN), _greenPin(GREEN_PIN), _bluePin(BLUE_PIN), _currentR(0),
      _currentG(0), _currentB(0), _targetR(0), _targetG(0), _targetB(0),
      _transitioning(false) {}

void LEDPWMController::begin() {
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
}

void LEDPWMController::setColor(int r, int g, int b) {
    _targetR = r;
    _targetG = g;
    _targetB = b;
    _startTime = millis();
    _transitioning = true;
}

void LEDPWMController::update() {
    if (_transitioning) {
        unsigned long now = millis();
        unsigned long elapsed = now - _startTime;

        if (elapsed >= TRANSITION_TIME) {
            // 遷移が完了した場合、目標の色に設定
            _currentR = _targetR;
            _currentG = _targetG;
            _currentB = _targetB;
            _transitioning = false;
        } else {
            // 線形補間
            float progress = (float)elapsed / TRANSITION_TIME;
            int newR = _currentR + progress * (_targetR - _currentR);
            int newG = _currentG + progress * (_targetG - _currentG);
            int newB = _currentB + progress * (_targetB - _currentB);

            analogWrite(_redPin, newR);
            analogWrite(_greenPin, newG);
            analogWrite(_bluePin, newB);
        }
    }
}
