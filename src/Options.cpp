#include "Options.hpp"
#include "stb_easy_font.h"
#include <iostream>

Options::Options(Window* window) : isFullscreen(false), currentResolution(0),
                    isShowingResolution(false), isShowingKeyBindings(false),
                    musicSlider(250, 550, 500, 40, "Music Volume", 0.5f),
                    soundSlider(250, 630, 500, 40, "Sound Volume", 0.5f),
                    parentWindow(window) {
    initResolutions();
    createOptionButtons();
    createResolutionButtons();
    createKeyBindingButtons();
}

Options::~Options() {}

void Options::initResolutions() {
    availableResolutions = {
        {800, 600, "800x600"},
        {1024, 768, "1024x768"},
        {1280, 720, "1280x720 (HD)"},
        {1366, 768, "1366x768"},
        {1600, 900, "1600x900"},
        {1920, 1080, "1920x1080 (Full HD)"}
    };
}

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
        Button(250, 360, 500, 50, "Fullscreen Mode: Off"),
        Button(250, 440, 500, 50, "Key Bindings"),
        Button(250, 680, 500, 50, "Back")
    };
}

void Options::createResolutionButtons() {
    resolutionButtons.clear();
    float y = 280;
    
    for (size_t i = 0; i < availableResolutions.size(); ++i) {
        resolutionButtons.emplace_back(250, y, 500, 50, availableResolutions[i].text);
        y += 80;
    }
    
    // Add back button at the bottom
    resolutionButtons.emplace_back(250, y, 500, 50, "Back");
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
        for (size_t i = 0; i < resolutionButtons.size(); ++i) {
            if (x >= resolutionButtons[i].getX() && x <= resolutionButtons[i].getX() + resolutionButtons[i].getWidth() &&
                y >= resolutionButtons[i].getY() && y <= resolutionButtons[i].getY() + resolutionButtons[i].getHeight()) {
                resolutionButtons[i].setPressed(true);
                resolutionButtons[i].setPressedTime(glfwGetTime());

                if (resolutionButtons[i].getText() == "Back") {
                    isShowingResolution = false;
                } else {
                    // Change resolution
                    if (i < availableResolutions.size()) {
                        currentResolution = i;
                        parentWindow->setResolution(availableResolutions[i].width, availableResolutions[i].height);
                        std::cout << "Resolution changed to: " << availableResolutions[i].text << std::endl;
                    }
                }
                return false;
            }
        }
    } else if (isShowingKeyBindings) {
        for (auto& button : keyBindingButtons) {
            button.setHovered(false);
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

                if (button.getText().find("Screen Resolution") != std::string::npos) {
                    isShowingResolution = true;
                } else if (button.getText().find("Fullscreen Mode") != std::string::npos) {
                    parentWindow->toggleFullscreen();
                    // Update button text based on fullscreen state
                    for (auto& btn : optionButtons) {
                        if (btn.getText().find("Fullscreen Mode") != std::string::npos) {
                            btn.setText("Fullscreen Mode: " + std::string(parentWindow->getIsFullscreen() ? "On" : "Off"));
                            break;
                        }
                    }
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