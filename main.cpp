#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#define STB_EASY_FONT_IMPLEMENTATION
#include "lib/stb_easy_font.h"

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
    {250, 440, 500, 50, "Load Game", false, false, 0.0},
    {250, 360, 500, 50, "Options", false, false, 0.0},
    {250, 600, 500, 50, "Exit", false, false, 0.0}
};

// Arka plan çizimi
void drawBackground() {
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

void drawText(float x, float y, const char* text, float r, float g, float b, float scale = 1.0f) {
    char buffer[99999]; // Yeterince büyük buffer
    int num_quads;

    glPushMatrix();                      // Mevcut dönüşüm matrisini kaydet
    glTranslatef(x, y, 0.0f);            // Yazının konumuna taşı
    glScalef(scale, scale, 1.0f);        // Ölçek uygula (scale ile büyütme/küçültme)

    glColor3f(r, g, b);
    glEnableClientState(GL_VERTEX_ARRAY);
    num_quads = stb_easy_font_print(0, 0, (char*)text, NULL, buffer, sizeof(buffer)); // Artık (x, y) yerine (0, 0)
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);

    glPopMatrix();                       // Önceki dönüşüm matrisine geri dön
}

// Buton çizimi
void drawButton(const Button& button) {
    float x = button.x;
    float y = button.y;
    float w = button.width;
    float h = button.height;

    // Arkaplan
    glBegin(GL_QUADS);
    if (button.isPressed) {
        glColor3f(0.168f, 0.168f, 0.168f);
    } else if (button.isHovered) {
        glColor3f(0.6f, 0.6f, 0.6f);
    } else {
        glColor3f(0.4f, 0.4f, 0.4f);
    }
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    // Çerçeve
    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(x, y);
    glVertex2f(x + w, y);
    glVertex2f(x + w, y + h);
    glVertex2f(x, y + h);
    glEnd();

    // Yazı (ölçekli)
    float scale = 2.0f; // 2 kat daha büyük yazı
    float textWidth = button.text.length() * 10 * scale; // Yazının genişliği
    float textX = x + (w - textWidth) / 2.0f;  // Yatayda ortalamak için
    float textY = y + (h - scale * 12) / 2.0f;  // Dikeyde ortalamak için

    drawText(textX, textY, button.text.c_str(), 1.0f, 1.0f, 1.0f, scale);
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

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0, WINDOW_WIDTH, WINDOW_HEIGHT, 0, -1, 1); // burayı değiştir
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
