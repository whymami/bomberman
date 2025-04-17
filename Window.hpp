#ifndef WINDOW_HPP
#define WINDOW_HPP

#include <GLFW/glfw3.h>
#include <vector>
#include "Button.hpp"
#include "Options.hpp"

class Window {
private:
    static const int WINDOW_WIDTH = 1000;
    static const int WINDOW_HEIGHT = 1000;
    
    GLFWwindow* window;
    std::vector<Button> menuButtons;
    Options options;
    bool isInOptions;

    void initGLFW();
    void initOpenGL();
    void setupCallbacks();
    void createMenuButtons();

public:
    Window();
    ~Window();

    void run();
    void drawBackground();
    void drawText(float x, float y, const char* text, float r, float g, float b, float scale = 1.0f);
    void drawButton(const Button& button);
    void checkMousePosition(double x, double y);
    void checkButtonClick(double x, double y);
};

#endif 