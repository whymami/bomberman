#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <vector>
#include <GLFW/glfw3.h>
#include "Button.hpp"

class Options {
private:
    static const int WINDOW_WIDTH = 1000;
    static const int WINDOW_HEIGHT = 1000;

    std::vector<Button> optionButtons;
    std::vector<Button> resolutionButtons;
    std::vector<Button> volumeButtons;
    std::vector<Button> keyBindingButtons;
    
    bool isFullscreen;
    int currentResolution;
    int musicVolume;
    int soundVolume;
    bool isShowingResolution;
    bool isShowingVolume;
    bool isShowingKeyBindings;

    void createOptionButtons();
    void createResolutionButtons();
    void createVolumeButtons();
    void createKeyBindingButtons();
    void drawButton(const Button& button);
    void drawText(float x, float y, const char* text, float r, float g, float b, float scale = 1.0f);

public:
    Options();
    ~Options();

    void draw();
    void checkMousePosition(double x, double y);
    bool checkButtonClick(double x, double y);
    
    bool getIsFullscreen() const;
    int getCurrentResolution() const;
    int getMusicVolume() const;
    int getSoundVolume() const;
};

#endif 