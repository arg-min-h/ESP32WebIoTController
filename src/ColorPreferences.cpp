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
