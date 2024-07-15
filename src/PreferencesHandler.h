#ifndef PREFERENCES_HANDLER_H
#define PREFERENCES_HANDLER_H

#include <Preferences.h>

class PreferencesHandler {
  public:
    PreferencesHandler();
    void begin(const char *name, bool readOnly);
    int getInt(const char *key, int defaultValue);
    void putInt(const char *key, int value);
    void end();

  private:
    Preferences preferences;
};

#endif // PREFERENCES_HANDLER_H
