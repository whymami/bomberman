#include "Options.hpp"
#include "lib/stb_easy_font.h"
#include <iostream>

Options::Options() : isFullscreen(false), currentResolution(0),
                    isShowingResolution(false), isShowingKeyBindings(false),
                    musicSlider(250, 550, 500, 40, "Music Volume", 0.5f),
                    soundSlider(250, 630, 500, 40, "Sound Volume", 0.5f) {
    createOptionButtons();
    createResolutionButtons();
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
    } else if (isShowingKeyBindings) {
        for (auto& button : keyBindingButtons) {
            drawButton(button);
        }
    } else {
        for (auto& button : optionButtons) {
            drawButton(button);
        }
        // Draw sliders in the main options menu
        musicSlider.draw();
        soundSlider.draw();
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

        // Update slider values if they're being dragged
        if (musicSlider.getIsDragging()) {
            musicSlider.updateValue(x);
        }
        if (soundSlider.getIsDragging()) {
            soundSlider.updateValue(x);
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
                    std::cout << "Resolution changed: " << button.getText() << std::endl;
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
                    std::cout << "Key binding changed: " << button.getText() << std::endl;
                }
                return false;
            }
        }
    } else {
        // Check sliders first
        if (musicSlider.checkMousePosition(x, y)) {
            musicSlider.setDragging(true);
            return false;
        }
        if (soundSlider.checkMousePosition(x, y)) {
            soundSlider.setDragging(true);
            return false;
        }

        // Then check buttons
        for (auto& button : optionButtons) {
            if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
                y >= button.getY() && y <= button.getY() + button.getHeight()) {
                button.setPressed(true);
                button.setPressedTime(glfwGetTime());

                if (button.getText() == "Screen Resolution") {
                    isShowingResolution = true;
                } else if (button.getText() == "Fullscreen Mode") {
                    isFullscreen = !isFullscreen;
                    std::cout << "Fullscreen mode: " << (isFullscreen ? "On" : "Off") << std::endl;
                } else if (button.getText() == "Key Bindings") {
                    isShowingKeyBindings = true;
                } else if (button.getText() == "Back") {
                    return true;
                }
                return false;
            }
        }
    }
    return false;
}

void Options::handleMouseRelease() {
    musicSlider.setDragging(false);
    soundSlider.setDragging(false);
}

bool Options::getIsFullscreen() const { return isFullscreen; }
int Options::getCurrentResolution() const { return currentResolution; }
float Options::getMusicVolume() const { return musicSlider.getValue(); }
float Options::getSoundVolume() const { return soundSlider.getValue(); } 