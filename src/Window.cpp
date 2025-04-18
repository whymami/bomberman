#include "Window.hpp"
#include "Options.hpp"
#include "stb_easy_font.h"
#include <iostream>

Window::Window() : window(nullptr), options(nullptr), isInOptions(false), 
                  isFullscreen(false), currentWidth(WINDOW_WIDTH), currentHeight(WINDOW_HEIGHT),
                  windowedPosX(0), windowedPosY(0), windowedWidth(WINDOW_WIDTH), windowedHeight(WINDOW_HEIGHT) {
    initGLFW();
    initOpenGL();
    setupCallbacks();
    createMenuButtons();
    options = new Options(this);
}

Window::~Window() {
    delete options;

    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void Window::createMenuButtons() {
    menuButtons = {
        Button(250, 280, 500, 50, "Single Play"),
        Button(250, 360, 500, 50, "Play Arena"),
        Button(250, 440, 500, 50, "Load Game"),
        Button(250, 520, 500, 50, "Options"),
        Button(250, 600, 500, 50, "Exit")
    };
}

void Window::initGLFW() {
    if (!glfwInit()) {
        throw std::runtime_error("Failed to initialize GLFW");
    }

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    window = glfwCreateWindow(currentWidth, currentHeight, "Bomberman", NULL, NULL);
    if (!window) {
        glfwTerminate();
        throw std::runtime_error("Failed to create GLFW window");
    }

    glfwMakeContextCurrent(window);
    centerWindow();
}

void Window::centerWindow() {
    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) return;

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode) return;

    int monitorX, monitorY;
    glfwGetMonitorPos(monitor, &monitorX, &monitorY);

    int windowX = monitorX + (mode->width - currentWidth) / 2;
    int windowY = monitorY + (mode->height - currentHeight) / 2;

    glfwSetWindowPos(window, windowX, windowY);
}

void Window::updateViewport() {
    // Set viewport to cover the entire window
    glViewport(0, 0, currentWidth, currentHeight);
    
    // Calculate the aspect ratio
    float targetAspectRatio = static_cast<float>(WINDOW_WIDTH) / WINDOW_HEIGHT;
    float currentAspectRatio = static_cast<float>(currentWidth) / currentHeight;
    
    int viewportX = 0;
    int viewportY = 0;
    int viewportWidth = currentWidth;
    int viewportHeight = currentHeight;
    
    // Adjust viewport to maintain aspect ratio
    if (currentAspectRatio > targetAspectRatio) {
        viewportWidth = static_cast<int>(currentHeight * targetAspectRatio);
        viewportX = (currentWidth - viewportWidth) / 2;
    } else if (currentAspectRatio < targetAspectRatio) {
        viewportHeight = static_cast<int>(currentWidth / targetAspectRatio);
        viewportY = (currentHeight - viewportHeight) / 2;
    }
    
    glViewport(viewportX, viewportY, viewportWidth, viewportHeight);
}

void Window::setResolution(int width, int height) {
    currentWidth = width;
    currentHeight = height;
    

    int xpos, ypos;
    glfwGetWindowPos(window, &xpos, &ypos);
    // set window size
    glfwSetWindowSize(window, width, height);
    if (!window) {
        std::cerr << "Failed to create window with new resolution" << std::endl;
        return;
    }
    
    updateViewport();
    centerWindow();
    
    setupCallbacks();

}

void Window::initOpenGL() {
    updateProjectionMatrix();
}

void Window::updateProjectionMatrix() {
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Window::setupCallbacks() {
    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
        Window* w = static_cast<Window*>(glfwGetWindowUserPointer(win));
        // Scale mouse coordinates to match the original window size
        double scaleX = static_cast<double>(WINDOW_WIDTH) / w->currentWidth;
        double scaleY = static_cast<double>(WINDOW_HEIGHT) / w->currentHeight;
        double scaledX = x * scaleX;
        double scaledY = y * scaleY;
        
        if (w->isInOptions) {
            w->options->checkMousePosition(scaledX, scaledY);
        } else {
            w->checkMousePosition(scaledX, scaledY);
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int) {
        Window* w = static_cast<Window*>(glfwGetWindowUserPointer(win));
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                double x, y;
                glfwGetCursorPos(win, &x, &y);
                // Scale mouse coordinates to match the original window size
                double scaleX = static_cast<double>(WINDOW_WIDTH) / w->currentWidth;
                double scaleY = static_cast<double>(WINDOW_HEIGHT) / w->currentHeight;
                double scaledX = x * scaleX;
                double scaledY = y * scaleY;
                
                if (w->isInOptions) {
                    if (w->options->checkButtonClick(scaledX, scaledY)) {
                        w->isInOptions = false;
                    }
                } else {
                    w->checkButtonClick(scaledX, scaledY);
                }
            } else if (action == GLFW_RELEASE && w->isInOptions) {
                w->options->handleMouseRelease();
            }
        }
    });

    glfwSetWindowUserPointer(window, this);
}

void Window::run() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        drawBackground();

        if (isInOptions) {
            options->draw();
        } else {
            double currentTime = glfwGetTime();
            for (auto& button : menuButtons) {
                if (button.getIsPressed() && currentTime - button.getPressedTime() > 0.15) {
                    button.setPressed(false);
                }
                drawButton(button);
            }
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

void Window::drawBackground() {
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.1f, 0.3f);
    glVertex2f(0, 0);
    glVertex2f(WINDOW_WIDTH, 0);
    glColor3f(0.0f, 0.05f, 0.15f);
    glVertex2f(WINDOW_WIDTH, WINDOW_HEIGHT);
    glVertex2f(0, WINDOW_HEIGHT);
    glEnd();

    glColor3f(0.0f, 0.2f, 0.4f);
    glLineWidth(1.0f);
    for (int i = 0; i <= WINDOW_WIDTH; i += 100) {
        glBegin(GL_LINES);
        glVertex2f(i, 0);
        glVertex2f(i, WINDOW_HEIGHT);
        glEnd();
    }
    for (int i = 0; i <= WINDOW_HEIGHT; i += 100) {
        glBegin(GL_LINES);
        glVertex2f(0, i);
        glVertex2f(WINDOW_WIDTH, i);
        glEnd();
    }
}

void Window::drawText(float x, float y, const char* text, float r, float g, float b, float scale) {
    char buffer[99999];
    int num_quads;

    glPushMatrix();
    glTranslatef(x, y, 0.0f);
    glScalef(scale, scale, 1.0f);

    glColor3f(r, g, b);
    glEnableClientState(GL_VERTEX_ARRAY);
    num_quads = stb_easy_font_print(0, 0, (char*)text, NULL, buffer, sizeof(buffer));
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();
}

void Window::drawButton(const Button& button) {
    float x = button.getX();
    float y = button.getY();
    float w = button.getWidth();
    float h = button.getHeight();

    glBegin(GL_QUADS);
    if (button.getIsPressed()) {
        glColor3f(0.168f, 0.168f, 0.168f);
    } else if (button.getIsHovered()) {
        glColor3f(0.6f, 0.6f, 0.6f);
    } else {
        glColor3f(0.4f, 0.4f, 0.4f);
    }
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    float scale = 2.0f;
    float textWidth = button.getText().length() * 10 * scale;
    float textX = x + (w) / 2.5f;
    float textY = y + (h - scale * 12) / 1.5f;

    drawText(textX, textY, button.getText().c_str(), 1.0f, 1.0f, 1.0f, scale);
}

void Window::checkMousePosition(double x, double y) {
    for (auto& button : menuButtons) {
        button.setHovered(false);
        if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
            y >= button.getY() && y <= button.getY() + button.getHeight()) {
            button.setHovered(true);
            break;
        }
    }
}

void Window::checkButtonClick(double x, double y) {
    for (auto& button : menuButtons) {
        if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
            y >= button.getY() && y <= button.getY() + button.getHeight()) {
            std::cout << "Buton tıklandı: " << button.getText() << std::endl;
            button.setPressed(true);
            button.setPressedTime(glfwGetTime());
            
            if (button.getText() == "Single Play") {
                // Start single play
            }  else if (button.getText() == "Play Arena") {
                // Load game
            } else if (button.getText() == "Load Game") {
                // Load game
            } else if (button.getText() == "Options") {
                isInOptions = true;
            } else if (button.getText() == "Exit") {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
            break;
        }
    }
}

void Window::toggleFullscreen() {
    isFullscreen = !isFullscreen;
    
    if (isFullscreen) {
        // Store windowed mode position and size
        glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
        windowedWidth = currentWidth;
        windowedHeight = currentHeight;

        // Get primary monitor
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        // Switch to fullscreen with monitor's refresh rate
        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);
        currentWidth = mode->width;
        currentHeight = mode->height;
    } else {
        // Switch back to windowed mode
        glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, 
                           windowedWidth, windowedHeight, GLFW_DONT_CARE);
        currentWidth = windowedWidth;
        currentHeight = windowedHeight;
    }

    // Update OpenGL state
    glClear(GL_COLOR_BUFFER_BIT);
    updateViewport();
    updateProjectionMatrix();
    setupCallbacks();
    
    // Force window system to update
    glfwPollEvents();
    glfwSwapBuffers(window);
}