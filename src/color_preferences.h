#ifndef COLOR_PREFERENCES_H
#define COLOR_PREFERENCES_H

#include <Preferences.h>

extern Preferences preferences;

void saveColorPreferences(int r, int g, int b);
void getColorPreferences(int &r, int &g, int &b);

#endif // COLOR_PREFERENCES_H
