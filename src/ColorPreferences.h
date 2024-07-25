#ifndef COLOR_PREFERENCES_H
#define COLOR_PREFERENCES_H

class ColorPreferences {
  public:
    ColorPreferences();
    void saveColor(int r, int g, int b);
    void getColor(int &r, int &g, int &b);
    void saveSpeed(float speed);
    float getSpeed();
    void saveBrightnessCorrection(float redCorrection, float greenCorrection,
                                  float blueCorrection);
    void getBrightnessCorrection(float &redCorrection, float &greenCorrection,
                                 float &blueCorrection);

  private:
    void begin(bool readOnly);
    void end();
};

#endif // COLOR_PREFERENCES_H
