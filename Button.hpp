#ifndef BUTTON_HPP
#define BUTTON_HPP

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
    double pressedTime;

public:
    Button(float x, float y, float width, float height, const std::string& text);
    ~Button();

    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    const std::string& getText() const;
    bool getIsHovered() const;
    bool getIsPressed() const;
    double getPressedTime() const;

    void setHovered(bool hovered);
    void setPressed(bool pressed);
    void setPressedTime(double time);
    void setText(const std::string& newText);
};

#endif 