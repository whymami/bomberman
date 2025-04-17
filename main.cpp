#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <GLFW/glfw3.h>
#include <iostream>
#include <vector>
#include <string>

const int WINDOW_WIDTH = 1000;
const int WINDOW_HEIGHT = 1000;

GLFWwindow* window;

// Buton yapısı
struct Button {
    float x, y;
    float width, height;
    std::string text;
    bool isHovered;
    bool isPressed;
    double pressedTime;
};

// Menü butonları
std::vector<Button> menuButtons = {
    {250, 280, 500, 50, "Single Play", false, false, 0.0},
    {250, 520, 500, 50, "Play Arena", false, false, 0.0},
    {250, 440, 500, 50, "Load Game", false, false, 0.0},   // Yer değiştirildi
    {250, 360, 500, 50, "Options", false, false, 0.0},     // Yer değiştirildi
    {250, 600, 500, 50, "Exit", false, false, 0.0}
};


// Arka plan çizimi
void drawBackground() {
    glBegin(GL_QUADS);
    glColor3f(0.0f, 0.1f, 0.3f);
    glVertex2f(-1.0f, 1.0f);
    glVertex2f(1.0f, 1.0f);
    glColor3f(0.0f, 0.05f, 0.15f);
    glVertex2f(1.0f, -1.0f);
    glVertex2f(-1.0f, -1.0f);
    glEnd();

    glColor3f(0.0f, 0.2f, 0.4f);
    glLineWidth(1.0f);
    for (int i = -10; i <= 10; i++) {
        float pos = i * 0.2f;
        glBegin(GL_LINES);
        glVertex2f(-1.0f, pos);
        glVertex2f(1.0f, pos);
        glVertex2f(pos, -1.0f);
        glVertex2f(pos, 1.0f);
        glEnd();
    }
}

// Buton çizimi
void drawButton(const Button& button) {
    float x = button.x / (float)WINDOW_WIDTH * 2.0f - 1.0f;
    float y = 1.0f - (button.y / (float)WINDOW_HEIGHT * 2.0f);
    float w = button.width / (float)WINDOW_WIDTH * 2.0f;
    float h = button.height / (float)WINDOW_HEIGHT * 2.0f;

    // Arkaplan
    glBegin(GL_QUADS);
    if (button.isPressed) {
        glColor3f(0.168f, 0.168f, 0.168f);
    } else if (button.isHovered) {
        glColor3f(0.6f, 0.6f, 0.6f);  // Hover
    } else {
        glColor3f(0.4f, 0.4f, 0.4f);  // Normal
    }
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y - h);
    glVertex2f(x, y - h);
    glEnd();

    // Çerçeve
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y - h);
    glVertex2f(x, y - h);
    glEnd();
}

// Fare konumunu kontrol et
void checkMousePosition(double x, double y) {
    for (auto& button : menuButtons) {
        button.isHovered = false;
        if (x >= button.x && x <= button.x + button.width &&
            y >= button.y && y <= button.y + button.height) {
            button.isHovered = true;
            break;
        }
    }
}

// Tıklama kontrolü
void checkButtonClick(double x, double y) {
    for (auto& button : menuButtons) {
        if (x >= button.x && x <= button.x + button.width &&
            y >= button.y && y <= button.y + button.height) {
            std::cout << "Buton tıklandı: " << button.text << std::endl;
            button.isPressed = true;
            button.pressedTime = glfwGetTime();

            if (button.text == "Exit") {
                glfwSetWindowShouldClose(window, GLFW_TRUE);
            }
        }
    }
}

int main() {
    if (!glfwInit()) return -1;

    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    glfwWindowHint(GLFW_DECORATED, GLFW_TRUE);

    window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Bomberman", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);

    glViewport(0, 0, WINDOW_WIDTH, WINDOW_HEIGHT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-1.0, 1.0, -1.0, 1.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glfwSetCursorPosCallback(window, [](GLFWwindow*, double x, double y) {
        checkMousePosition(x, y);
    });

    glfwSetMouseButtonCallback(window, [](GLFWwindow*, int button, int action, int) {
        if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
            double x, y;
            glfwGetCursorPos(window, &x, &y);
            checkButtonClick(x, y);
        }
    });

    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        drawBackground();

        double currentTime = glfwGetTime();
        for (auto& button : menuButtons) {
            // 150 ms sonra basılma durumu sıfırlanıyor
            if (button.isPressed && currentTime - button.pressedTime > 0.15) {
                button.isPressed = false;
            }
            drawButton(button);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}
