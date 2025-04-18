#ifndef OPTIONS_HPP
#define OPTIONS_HPP

#include <vector>
#include <GLFW/glfw3.h>
#include "Button.hpp"
#include "Slider.hpp"

class Options {
private:
    static const int WINDOW_WIDTH = 1000;
    static const int WINDOW_HEIGHT = 1000;

    std::vector<Button> optionButtons;
    std::vector<Button> resolutionButtons;
    std::vector<Button> keyBindingButtons;
    
    Slider musicSlider;
    Slider soundSlider;
    
    bool isFullscreen;
    int currentResolution;
    bool isShowingResolution;
    bool isShowingKeyBindings;

    void createOptionButtons();
    void createResolutionButtons();
    void createKeyBindingButtons();
    void drawButton(const Button& button);
    void drawText(float x, float y, const char* text, float r, float g, float b, float scale = 1.0f);

public:
    Options();
    ~Options();

    void draw();
    void checkMousePosition(double x, double y);
    bool checkButtonClick(double x, double y);
    void handleMouseRelease();
    
    bool getIsFullscreen() const;
    int getCurrentResolution() const;
    float getMusicVolume() const;
    float getSoundVolume() const;
};

#endif 