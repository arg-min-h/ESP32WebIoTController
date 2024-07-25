#include "ColorPreferences.h"
#include "PreferencesHandler.h"

PreferencesHandler preferences;

ColorPreferences::ColorPreferences() {}

void ColorPreferences::saveColor(int r, int g, int b) {
    begin(false);
    preferences.putInt("r", r);
    preferences.putInt("g", g);
    preferences.putInt("b", b);
    end();
}

void ColorPreferences::getColor(int &r, int &g, int &b) {
    begin(true);
    r = preferences.getInt("r", 0);
    g = preferences.getInt("g", 0);
    b = preferences.getInt("b", 0);
    end();
}

void ColorPreferences::saveSpeed(float speed) {
    preferences.begin("settings", false);
    preferences.putInt("speed",
                       static_cast<int>(speed)); // float を int に変換して保存
    preferences.end();
}

float ColorPreferences::getSpeed() {
    preferences.begin("settings", true);
    float speed = static_cast<float>(
        preferences.getInt("speed", 50)); // デフォルト速度を50.0とする
    preferences.end();
    return speed;
}

void ColorPreferences::saveBrightnessCorrection(float redCorrection,
                                                float greenCorrection,
                                                float blueCorrection) {
    preferences.begin("settings", false);
    preferences.putInt("redCorrection", static_cast<int>(redCorrection * 100));
    preferences.putInt("greenCorrection",
                       static_cast<int>(greenCorrection * 100));
    preferences.putInt("blueCorrection",
                       static_cast<int>(blueCorrection * 100));
    preferences.end();
}

void ColorPreferences::getBrightnessCorrection(float &redCorrection,
                                               float &greenCorrection,
                                               float &blueCorrection) {
    preferences.begin("settings", true);
    redCorrection =
        static_cast<float>(preferences.getInt("redCorrection", 100)) / 100;
    greenCorrection =
        static_cast<float>(preferences.getInt("greenCorrection", 100)) / 100;
    blueCorrection =
        static_cast<float>(preferences.getInt("blueCorrection", 100)) / 100;
    preferences.end();
}

void ColorPreferences::begin(bool readOnly) {
    preferences.begin("color", readOnly);
}

void ColorPreferences::end() { preferences.end(); }
