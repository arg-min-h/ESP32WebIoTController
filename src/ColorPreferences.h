#ifndef COLOR_PREFERENCES_H
#define COLOR_PREFERENCES_H

class ColorPreferences {
  public:
    ColorPreferences();
    void saveColor(int r, int g, int b);
    void getColor(int &r, int &g, int &b);

  private:
    void begin(bool readOnly);
    void end();
};

#endif // COLOR_PREFERENCES_H
