#include "LEDPWMController.h"

LEDPWMController::LEDPWMController(int redPin, int greenPin, int bluePin)
    : _redPin(redPin), _greenPin(greenPin), _bluePin(bluePin) {}

void LEDPWMController::begin() {
    // ピンを出力に設定
    pinMode(_redPin, OUTPUT);
    pinMode(_greenPin, OUTPUT);
    pinMode(_bluePin, OUTPUT);
}

void LEDPWMController::setColor(int r, int g, int b) {
    // PWM信号を送信して色を設定
    analogWrite(_redPin, r);
    analogWrite(_greenPin, g);
    analogWrite(_bluePin, b);
}
