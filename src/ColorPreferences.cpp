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

void ColorPreferences::begin(bool readOnly) {
    preferences.begin("color", readOnly);
}

void ColorPreferences::end() { preferences.end(); }

void ColorPreferences::saveSpeed(float speed) { // 追加
    preferences.begin("settings", false);
    preferences.putInt("speed",
                       static_cast<int>(speed)); // float を int に変換して保存
    preferences.end();
}

float ColorPreferences::getSpeed() { // 追加
    preferences.begin("settings", true);
    float speed = static_cast<float>(
        preferences.getInt("speed", 50)); // デフォルト速度を50.0とする
    preferences.end();
    return speed;
}
