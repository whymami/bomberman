#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>
#include <vector>
#include "Button.hpp"

class Options;

class Window {
private:
    static const int WINDOW_WIDTH = 1000;
    static const int WINDOW_HEIGHT = 1000;
    
    GLFWwindow* window;
    std::vector<Button> menuButtons;
    Options* options;
    bool isInOptions;
    bool isFullscreen;
    int currentWidth;
    int currentHeight;
    int windowedPosX;
    int windowedPosY;
    int windowedWidth;
    int windowedHeight;

    void initGLFW();
    void initOpenGL();
    void setupCallbacks();
    void createMenuButtons();
    void updateViewport();
    void updateProjectionMatrix();

public:
    Window();
    ~Window();

    void run();
    void drawBackground();
    void drawText(float x, float y, const char* text, float r, float g, float b, float scale = 1.0f);
    void drawButton(const Button& button);
    void checkMousePosition(double x, double y);
    void checkButtonClick(double x, double y);
    void setResolution(int width, int height);
    void centerWindow();
    void toggleFullscreen();
    bool getIsFullscreen() const { return isFullscreen; }
    
    int getCurrentWidth() const { return currentWidth; }
    int getCurrentHeight() const { return currentHeight; }
};

#endif 