#include "Options.hpp"
#include "lib/stb_easy_font.h"
#include <iostream>

Options::Options() : isFullscreen(false), currentResolution(0), 
                    musicVolume(50), soundVolume(50),
                    isShowingResolution(false), isShowingVolume(false),
                    isShowingKeyBindings(false) {
    createOptionButtons();
    createResolutionButtons();
    createVolumeButtons();
    createKeyBindingButtons();
}

Options::~Options() {}

void Options::drawButton(const Button& button) {
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

void Options::drawText(float x, float y, const char* text, float r, float g, float b, float scale) {
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

void Options::createOptionButtons() {
    optionButtons = {
        Button(250, 280, 500, 50, "Screen Resolution"),
        Button(250, 360, 500, 50, "Fullscreen Mode"),
        Button(250, 440, 500, 50, "Key Bindings"),
        Button(250, 520, 500, 50, "Music Volume"),
        Button(250, 600, 500, 50, "Sound Volume"),
        Button(250, 680, 500, 50, "Back")
    };
}

void Options::createResolutionButtons() {
    resolutionButtons = {
        Button(250, 280, 500, 50, "800x600"),
        Button(250, 360, 500, 50, "1024x768"),
        Button(250, 440, 500, 50, "1280x720"),
        Button(250, 520, 500, 50, "1920x1080"),
        Button(250, 600, 500, 50, "Back")
    };
}

void Options::createVolumeButtons() {
    volumeButtons = {
        Button(250, 280, 500, 50, "Volume: 0%"),
        Button(250, 360, 500, 50, "Volume: 25%"),
        Button(250, 440, 500, 50, "Volume: 50%"),
        Button(250, 520, 500, 50, "Volume: 75%"),
        Button(250, 600, 500, 50, "Volume: 100%"),
        Button(250, 680, 500, 50, "Back")
    };
}

void Options::createKeyBindingButtons() {
    keyBindingButtons = {
        Button(250, 280, 500, 50, "Move Up: W"),
        Button(250, 360, 500, 50, "Move Down: S"),
        Button(250, 440, 500, 50, "Move Left: A"),
        Button(250, 520, 500, 50, "Move Right: D"),
        Button(250, 600, 500, 50, "Place Bomb: Space"),
        Button(250, 680, 500, 50, "Back")
    };
}

void Options::draw() {
    if (isShowingResolution) {
        for (auto& button : resolutionButtons) {
            drawButton(button);
        }
    } else if (isShowingVolume) {
        for (auto& button : volumeButtons) {
            drawButton(button);
        }
    } else if (isShowingKeyBindings) {
        for (auto& button : keyBindingButtons) {
            drawButton(button);
        }
    } else {
        for (auto& button : optionButtons) {
            drawButton(button);
        }
    }
}

void Options::checkMousePosition(double x, double y) {
    if (isShowingResolution) {
        for (auto& button : resolutionButtons) {
            button.setHovered(false);
            if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
                y >= button.getY() && y <= button.getY() + button.getHeight()) {
                button.setHovered(true);
                break;
            }
        }
    } else if (isShowingVolume) {
        for (auto& button : volumeButtons) {
            button.setHovered(false);
            if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
                y >= button.getY() && y <= button.getY() + button.getHeight()) {
                button.setHovered(true);
                break;
            }
        }
    } else if (isShowingKeyBindings) {
        for (auto& button : keyBindingButtons) {
            button.setHovered(false);
            if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
                y >= button.getY() && y <= button.getY() + button.getHeight()) {
                button.setHovered(true);
                break;
            }
        }
    } else {
        for (auto& button : optionButtons) {
            button.setHovered(false);
            if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
                y >= button.getY() && y <= button.getY() + button.getHeight()) {
                button.setHovered(true);
                break;
            }
        }
    }
}

bool Options::checkButtonClick(double x, double y) {
    if (isShowingResolution) {
        for (auto& button : resolutionButtons) {
            if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
                y >= button.getY() && y <= button.getY() + button.getHeight()) {
                button.setPressed(true);
                button.setPressedTime(glfwGetTime());

                if (button.getText() == "Back") {
                    isShowingResolution = false;
                } else {
                    // Çözünürlük ayarını güncelle
                    std::cout << "Çözünürlük değiştirildi: " << button.getText() << std::endl;
                }
                return false;
            }
        }
    } else if (isShowingVolume) {
        for (auto& button : volumeButtons) {
            if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
                y >= button.getY() && y <= button.getY() + button.getHeight()) {
                button.setPressed(true);
                button.setPressedTime(glfwGetTime());

                if (button.getText() == "Back") {
                    isShowingVolume = false;
                } else {
                    // Ses seviyesini güncelle
                    std::cout << "Ses seviyesi değiştirildi: " << button.getText() << std::endl;
                }
                return false;
            }
        }
    } else if (isShowingKeyBindings) {
        for (auto& button : keyBindingButtons) {
            if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
                y >= button.getY() && y <= button.getY() + button.getHeight()) {
                button.setPressed(true);
                button.setPressedTime(glfwGetTime());

                if (button.getText() == "Back") {
                    isShowingKeyBindings = false;
                } else {
                    // Tuş atamasını güncelle
                    std::cout << "Tuş ataması değiştirildi: " << button.getText() << std::endl;
                }
                return false;
            }
        }
    } else {
        for (auto& button : optionButtons) {
            if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
                y >= button.getY() && y <= button.getY() + button.getHeight()) {
                button.setPressed(true);
                button.setPressedTime(glfwGetTime());

                if (button.getText() == "Screen Resolution") {
                    isShowingResolution = true;
                } else if (button.getText() == "Fullscreen Mode") {
                    isFullscreen = !isFullscreen;
                    std::cout << "Tam ekran modu: " << (isFullscreen ? "Açık" : "Kapalı") << std::endl;
                } else if (button.getText() == "Key Bindings") {
                    isShowingKeyBindings = true;
                } else if (button.getText() == "Music Volume") {
                    isShowingVolume = true;
                } else if (button.getText() == "Sound Volume") {
                    isShowingVolume = true;
                } else if (button.getText() == "Back") {
                    // Ana menüye dön
                    return true;
                }
                return false;
            }
        }
    }
    return false;
}

bool Options::getIsFullscreen() const { return isFullscreen; }
int Options::getCurrentResolution() const { return currentResolution; }
int Options::getMusicVolume() const { return musicVolume; }
int Options::getSoundVolume() const { return soundVolume; } 