#include <Window.hpp>
#include <Settings.hpp>
#include <stb_easy_font.h>
#include <iostream>

Window::Window() : window(nullptr), settings(nullptr), 
                  isFullscreen(false), currentWidth(WINDOW_WIDTH), currentHeight(WINDOW_HEIGHT),
                  windowedPosX(0), windowedPosY(0), windowedWidth(WINDOW_WIDTH), windowedHeight(WINDOW_HEIGHT),
                  activeTab(Tab::MAIN_MENU) {
    initGLFW();
    initOpenGL();
    setupCallbacks();
    createMenuButtons();
    settings = new Settings(this);
}

Window::~Window() {
    delete settings;

    if (window) {
        glfwDestroyWindow(window);
    }
    glfwTerminate();
}

void Window::createMenuButtons() {
    const float buttonWidth = 600;
    const float buttonHeight = 50;
    const float startX = (WINDOW_WIDTH - buttonWidth) / 2;

    menuButtons = {
        Button(startX, 280, buttonWidth, buttonHeight, "Single Play"),
        Button(startX, 360, buttonWidth, buttonHeight, "Play Arena"),
        Button(startX, 440, buttonWidth, buttonHeight, "Load Game"),
        Button(startX, 520, buttonWidth, buttonHeight, "Settings"),
        Button(startX, 600, buttonWidth, buttonHeight, "Exit")
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
    if (isFullscreen) return;  // Don't center if in fullscreen mode

    GLFWmonitor* monitor = glfwGetPrimaryMonitor();
    if (!monitor) return;

    const GLFWvidmode* mode = glfwGetVideoMode(monitor);
    if (!mode) return;

    int monitorX, monitorY, monitorWidth, monitorHeight;
    glfwGetMonitorWorkarea(monitor, &monitorX, &monitorY, &monitorWidth, &monitorHeight);

    // Get actual window size (including decorations)
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);

    // Calculate window position to center it in the monitor's workarea
    int windowX = monitorX + (monitorWidth - windowWidth) / 2;
    int windowY = monitorY + (monitorHeight - windowHeight) / 2;

    // Set window position
    glfwSetWindowPos(window, windowX, windowY);
    
    // Wait for the window system to update
    glfwWaitEvents();
}

void Window::updateViewport() {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    
    // Use full framebuffer size without maintaining aspect ratio
    glViewport(0, 0, fbWidth, fbHeight);
    currentWidth = fbWidth;
    currentHeight = fbHeight;
}

void Window::updateProjectionMatrix() {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    
    // Set up projection to stretch content to fill the window
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void Window::setResolution(int width, int height) {
    windowedWidth = width;
    windowedHeight = height;

    if (isFullscreen) {
        // If in fullscreen, store the new resolution but don't apply it yet
        return;
    }

    // Set window size
    glfwSetWindowSize(window, width, height);
    
    // Wait for the window system to update
    glfwWaitEvents();
    
    // Get the actual framebuffer size
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    currentWidth = fbWidth;
    currentHeight = fbHeight;
    
    // Center window before updating OpenGL state
    centerWindow();
    
    // Update OpenGL state
    updateViewport();
    updateProjectionMatrix();
}

void Window::initOpenGL() {
    updateProjectionMatrix();
}

void Window::transformMouseCoordinates(double& x, double& y) {
    int fbWidth, fbHeight;
    glfwGetFramebufferSize(window, &fbWidth, &fbHeight);
    
    // Get the window size (in screen coordinates)
    int windowWidth, windowHeight;
    glfwGetWindowSize(window, &windowWidth, &windowHeight);
    
    // Scale from window coordinates to framebuffer coordinates
    x = x * (static_cast<double>(fbWidth) / windowWidth);
    y = y * (static_cast<double>(fbHeight) / windowHeight);
    
    // Transform to our logical coordinate system
    x = (x / fbWidth) * WINDOW_WIDTH;
    y = (y / fbHeight) * WINDOW_HEIGHT;
}

void Window::setupCallbacks() {
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* win, int width, int height) {
        Window* w = static_cast<Window*>(glfwGetWindowUserPointer(win));
        w->currentWidth = width;
        w->currentHeight = height;
        w->updateViewport();
        w->updateProjectionMatrix();
    });

    glfwSetCursorPosCallback(window, [](GLFWwindow* win, double x, double y) {
        Window* w = static_cast<Window*>(glfwGetWindowUserPointer(win));
        w->transformMouseCoordinates(x, y);
        
        switch (w->activeTab) {
            case Tab::SETTINGS:
                w->settings->checkMousePosition(x, y);
                break;
            case Tab::MAIN_MENU:
            default:
                w->checkMousePosition(x, y);
                break;
        }
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow* win, int button, int action, int) {
        Window* w = static_cast<Window*>(glfwGetWindowUserPointer(win));
        if (button == GLFW_MOUSE_BUTTON_LEFT) {
            if (action == GLFW_PRESS) {
                double x, y;
                glfwGetCursorPos(win, &x, &y);
                w->transformMouseCoordinates(x, y);
                
                switch (w->activeTab) {
                    case Tab::SETTINGS:
                        if (w->settings->checkButtonClick(x, y)) {
                            w->activeTab = Tab::MAIN_MENU;
                        }
                        break;
                    case Tab::MAIN_MENU:
                    default:
                        w->checkButtonClick(x, y);
                        break;
                }
            } else if (action == GLFW_RELEASE && w->activeTab == Tab::SETTINGS) {
                w->settings->handleMouseRelease();
            }
        }
    });

    glfwSetWindowUserPointer(window, this);
}

void Window::run() {
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);
        drawBackground();

        switch (activeTab) {
            case Tab::SETTINGS:
                settings->draw();
                break;
            case Tab::SINGLE_PLAY:
                //game->init();
                break;
            case Tab::PLAY_ARENA:
                // TODO: Implement arena rendering
                break;
            case Tab::LOAD_GAME:
                // TODO: Implement load game rendering
                break;
            case Tab::MAIN_MENU:
            default:
                double currentTime = glfwGetTime();
                for (auto& button : menuButtons) {
                    if (button.getIsPressed() && currentTime - button.getPressedTime() > 0.15) {
                        button.setPressed(false);
                    }
                    button.draw();
                }
                break;
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
    // Reset all buttons' pressed state first
    for (auto& button : menuButtons) {
        button.setPressed(false);
    }

    for (auto& button : menuButtons) {
        if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
            y >= button.getY() && y <= button.getY() + button.getHeight()) {
            std::cout << "Buton tıklandı: " << button.getText() << std::endl;
            button.setPressed(true);
            button.setPressedTime(glfwGetTime());
            
            if (button.getText() == "Single Play") {
                activeTab = Tab::SINGLE_PLAY;
            } else if (button.getText() == "Play Arena") {
                activeTab = Tab::PLAY_ARENA;
            } else if (button.getText() == "Load Game") {
                activeTab = Tab::LOAD_GAME;
            } else if (button.getText() == "Settings") {
                activeTab = Tab::SETTINGS;
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
        glfwGetWindowPos(window, &windowedPosX, &windowedPosY);
        windowedWidth = currentWidth;
        windowedHeight = currentHeight;

        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* mode = glfwGetVideoMode(monitor);

        glfwSetWindowMonitor(window, monitor, 0, 0, mode->width, mode->height, mode->refreshRate);

        centerWindow();
    } else {
        glfwSetWindowMonitor(window, nullptr, windowedPosX, windowedPosY, 
                           windowedWidth, windowedHeight, GLFW_DONT_CARE);
    }
}