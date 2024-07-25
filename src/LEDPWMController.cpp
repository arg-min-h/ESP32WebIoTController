#include "LEDPWMController.h"
#include "LEDConfig.h" // LEDConfig.hをインクルード

LEDPWMController::LEDPWMController()
    : _redPin(RED_PIN), _greenPin(GREEN_PIN), _bluePin(BLUE_PIN), _currentR(0),
      _currentG(0), _currentB(0), _targetR(0), _targetG(0), _targetB(0),
      _transitioning(false), _lastUpdateTime(0) {}

void LEDPWMController::begin() {
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
}

void LEDPWMController::setColor(float r, float g, float b, float speed) {
    _targetR = r;
    _targetG = g;
    _targetB = b;
    _transitioning = true;
    _transitionSpeed = speed; // 速度を設定
}

void LEDPWMController::update() {
    unsigned long currentTime = millis();
    float deltaTime = (float)(currentTime - _lastUpdateTime) / 1000.0; // 秒単位
    _lastUpdateTime = currentTime;

    // 遷移速度 (例: 1秒あたりの変化量)
    // float speed = 1000.0; // ここで速度を設定
    float speed = _transitionSpeed;

    if (_currentR != _targetR) {
        float change = speed * deltaTime;
        if (abs(_targetR - _currentR) <= change) {
            _currentR = _targetR;
        } else {
            _currentR += (_targetR > _currentR ? change : -change);
        }
    }

    if (_currentG != _targetG) {
        float change = speed * deltaTime;
        if (abs(_targetG - _currentG) <= change) {
            _currentG = _targetG;
        } else {
            _currentG += (_targetG > _currentG ? change : -change);
        }
    }

    if (_currentB != _targetB) {
        float change = speed * deltaTime;
        if (abs(_targetB - _currentB) <= change) {
            _currentB = _targetB;
        } else {
            _currentB += (_targetB > _currentB ? change : -change);
        }
    }

    analogWrite(_redPin, _currentR);
    analogWrite(_greenPin, _currentG);
    analogWrite(_bluePin, _currentB);
}
