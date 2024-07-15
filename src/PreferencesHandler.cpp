#include "PreferencesHandler.h"

PreferencesHandler::PreferencesHandler() {}

void PreferencesHandler::begin(const char *name, bool readOnly) {
    preferences.begin(name, readOnly);
}

int PreferencesHandler::getInt(const char *key, int defaultValue) {
    return preferences.getInt(key, defaultValue);
}

void PreferencesHandler::putInt(const char *key, int value) {
    preferences.putInt(key, value);
}

void PreferencesHandler::end() { preferences.end(); }
