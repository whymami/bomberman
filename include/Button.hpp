#pragma once

#include <string>

class Button {
private:
    float x;
    float y;
    float width;
    float height;
    std::string text;
    bool isHovered;
    bool isPressed;
    bool isDisabled;
    double pressedTime;

public:
    Button(float x, float y, float width, float height, const std::string& text);

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    const std::string& getText() const;
    bool getIsHovered() const;
    bool getIsPressed() const;
    bool getIsDisabled() const;
    double getPressedTime() const;

    void setHovered(bool hovered);
    void setPressed(bool pressed);
    void setDisabled(bool disabled);
    void setPressedTime(double time);
    void setText(const std::string& newText);
}; 