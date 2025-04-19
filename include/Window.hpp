#pragma once

#include <GLFW/glfw3.h>
#include <vector>
#include "Button.hpp"

class Options;

enum class Tab {
    MAIN_MENU,
    OPTIONS,
    SINGLE_PLAY,
    PLAY_ARENA,
    LOAD_GAME
};

class Window {
private:
    static const int WINDOW_WIDTH = 1000;
    static const int WINDOW_HEIGHT = 1000;
    
    GLFWwindow* window;
    std::vector<Button> menuButtons;
    Options* options;
    bool isFullscreen;
    int currentWidth;
    int currentHeight;
    int windowedPosX;
    int windowedPosY;
    int windowedWidth;
    int windowedHeight;
    Tab activeTab;

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
    void transformMouseCoordinates(double& x, double& y);
    
    int getCurrentWidth() const { return currentWidth; }
    int getCurrentHeight() const { return currentHeight; }
}; 