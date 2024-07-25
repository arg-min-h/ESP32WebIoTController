#ifndef COLOR_PREFERENCES_H
#define COLOR_PREFERENCES_H

class ColorPreferences {
  public:
    ColorPreferences();
    void saveColor(int r, int g, int b);
    void getColor(int &r, int &g, int &b);
    void saveSpeed(float speed); // 追加
    float getSpeed();            // 追加

  private:
    void begin(bool readOnly);
    void end();
};

#endif // COLOR_PREFERENCES_H
