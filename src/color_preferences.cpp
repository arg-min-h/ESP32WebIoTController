#include "color_preferences.h"

Preferences preferences;

void saveColorPreferences(int r, int g, int b) {
    preferences.begin("color", false);
    preferences.putInt("r", r);
    preferences.putInt("g", g);
    preferences.putInt("b", b);
    preferences.end();
}

void getColorPreferences(int &r, int &g, int &b) {
    preferences.begin("color", true);
    r = preferences.getInt("r", 0);
    g = preferences.getInt("g", 0);
    b = preferences.getInt("b", 0);
    preferences.end();
}
