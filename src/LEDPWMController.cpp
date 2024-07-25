#include "LEDPWMController.h"
#include "LEDConfig.h"

LEDPWMController::LEDPWMController()
    : _redPin(RED_PIN), _greenPin(GREEN_PIN), _bluePin(BLUE_PIN), _currentR(0),
      _currentG(0), _currentB(0), _targetR(0), _targetG(0), _targetB(0),
      _transitioning(false), _lastUpdateTime(0), _transitionSpeed(50.0) {}

void LEDPWMController::begin() {
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
}

void LEDPWMController::setColor(float r, float g, float b, float speed) {
    _targetR = (r / 255.0) * _redMax;
    _targetG = (g / 255.0) * _greenMax;
    _targetB = (b / 255.0) * _blueMax;
    _transitionSpeed = speed;
    _transitioning = true;
}

void LEDPWMController::update() {
    unsigned long currentTime = millis();
    float deltaTime = (float)(currentTime - _lastUpdateTime) / 1000.0; // 秒単位
    _lastUpdateTime = currentTime;

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

void LEDPWMController::setBrightnessCorrection(float redMax, float greenMax,
                                               float blueMax) {
    _redMax = redMax;
    _greenMax = greenMax;
    _blueMax = blueMax;
}
