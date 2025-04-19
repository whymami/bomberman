#include "Options.hpp"
#include "stb_easy_font.h"
#include <iostream>

Options::Options(Window* window) : isFullscreen(false), currentResolution(0),
                    isShowingResolution(false), isShowingKeyBindings(false),
                    musicSlider(200, 550, 600, 40, "Music Volume", 0.5f),
                    soundSlider(200, 630, 600, 40, "Sound Volume", 0.5f),
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

void Options::createOptionButtons() {
    const float buttonWidth = 600;  // Increased from 500
    const float buttonHeight = 50;
    const float startX = (1000 - buttonWidth) / 2;  // Center horizontally in 1000x1000 window

    optionButtons = {
        Button(startX, 280, buttonWidth, buttonHeight, "Screen Resolution"),
        Button(startX, 360, buttonWidth, buttonHeight, "Fullscreen Mode: Windowed"),
        Button(startX, 440, buttonWidth, buttonHeight, "Key Bindings"),
        Button(startX, 680, buttonWidth, buttonHeight, "Back")
    };
}

void Options::createResolutionButtons() {
    const float buttonWidth = 600;
    const float buttonHeight = 50;
    const float startX = (1000 - buttonWidth) / 2;
    float y = 280;
    
    resolutionButtons.clear();
    
    for (size_t i = 0; i < availableResolutions.size(); ++i) {
        resolutionButtons.emplace_back(startX, y, buttonWidth, buttonHeight, availableResolutions[i].text);
        // Disable the current resolution button
        if (i == currentResolution) {
            resolutionButtons.back().setDisabled(true);
            resolutionButtons.back().setPressed(true);  // Make it appear selected
        }
        y += 80;
    }
    
    // Add back button at the bottom
    resolutionButtons.emplace_back(startX, y, buttonWidth, buttonHeight, "Back");
}

void Options::createKeyBindingButtons() {
    const float buttonWidth = 600;  // Increased from 500
    const float buttonHeight = 50;
    const float startX = (1000 - buttonWidth) / 2;  // Center horizontally

    keyBindingButtons = {
        Button(startX, 280, buttonWidth, buttonHeight, "Move Up: W"),
        Button(startX, 360, buttonWidth, buttonHeight, "Move Down: S"),
        Button(startX, 440, buttonWidth, buttonHeight, "Move Left: A"),
        Button(startX, 520, buttonWidth, buttonHeight, "Move Right: D"),
        Button(startX, 600, buttonWidth, buttonHeight, "Place Bomb: Space"),
        Button(startX, 680, buttonWidth, buttonHeight, "Back")
    };
}

void Options::draw() {
    if (isShowingResolution) {
        for (auto& button : resolutionButtons) {
            button.draw();
        }
    } else if (isShowingKeyBindings) {
        for (auto& button : keyBindingButtons) {
            button.draw();
        }
    } else {
        for (auto& button : optionButtons) {
            button.draw();
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

void Options::resetButtonStates() {
    // Reset all button states
    for (auto& button : optionButtons) {
        button.setPressed(false);
        button.setHovered(false);
    }
    for (auto& button : resolutionButtons) {
        button.setPressed(false);
        button.setHovered(false);
    }
    for (auto& button : keyBindingButtons) {
        button.setPressed(false);
        button.setHovered(false);
    }
}

bool Options::checkButtonClick(double x, double y) {
    // Reset all buttons' pressed state first
    auto resetButtonPressState = [](auto& buttons) {
        for (auto& btn : buttons) {
            if (!btn.getIsDisabled()) {  // Don't reset disabled buttons
                btn.setPressed(false);
            }
        }
    };

    if (isShowingResolution) {
        resetButtonPressState(resolutionButtons);
        for (size_t i = 0; i < resolutionButtons.size(); ++i) {
            if (x >= resolutionButtons[i].getX() && x <= resolutionButtons[i].getX() + resolutionButtons[i].getWidth() &&
                y >= resolutionButtons[i].getY() && y <= resolutionButtons[i].getY() + resolutionButtons[i].getHeight()) {
                
                // Skip if button is disabled
                if (resolutionButtons[i].getIsDisabled()) {
                    return false;
                }

                resolutionButtons[i].setPressed(true);
                resolutionButtons[i].setPressedTime(glfwGetTime());

                if (resolutionButtons[i].getText() == "Back") {
                    isShowingResolution = false;
                    resetButtonStates();
                } else {
                    // Change resolution
                    if (i < availableResolutions.size()) {
                        currentResolution = i;
                        parentWindow->setResolution(availableResolutions[i].width, availableResolutions[i].height);
                        std::cout << "Resolution changed to: " << availableResolutions[i].text << std::endl;
                        
                        // Recreate resolution buttons to update disabled state
                        createResolutionButtons();
                    }
                }
                return false;
            }
        }
    } else if (isShowingKeyBindings) {
        resetButtonPressState(keyBindingButtons);
        for (auto& button : keyBindingButtons) {
            if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
                y >= button.getY() && y <= button.getY() + button.getHeight()) {
                button.setPressed(true);
                button.setPressedTime(glfwGetTime());

                if (button.getText() == "Back") {
                    isShowingKeyBindings = false;
                    resetButtonStates();  // Reset all button states when going back
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

        // Reset option buttons' pressed state
        resetButtonPressState(optionButtons);

        // Then check buttons
        for (auto& button : optionButtons) {
            if (x >= button.getX() && x <= button.getX() + button.getWidth() &&
                y >= button.getY() && y <= button.getY() + button.getHeight()) {
                button.setPressed(true);
                button.setPressedTime(glfwGetTime());

                if (button.getText().find("Screen Resolution") != std::string::npos) {
                    isShowingResolution = true;
                    resetButtonStates();  // Reset all button states when switching to resolution menu
                } else if (button.getText().find("Fullscreen Mode") != std::string::npos) {
                    parentWindow->toggleFullscreen();
                    // Update button text based on fullscreen state
                    for (auto& btn : optionButtons) {
                        if (btn.getText().find("Fullscreen Mode") != std::string::npos) {
                            btn.setText("Fullscreen Mode: " + std::string(parentWindow->getIsFullscreen() ? "Full Screen" : "Windowed"));
                            break;
                        }
                    }
                } else if (button.getText() == "Key Bindings") {
                    isShowingKeyBindings = true;
                    resetButtonStates();  // Reset all button states when switching to key bindings menu
                } else if (button.getText() == "Back") {
                    resetButtonStates();  // Reset all button states when going back to main menu
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