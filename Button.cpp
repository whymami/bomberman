#include "Button.hpp"

Button::Button(float x, float y, float width, float height, const std::string& text)
    : x(x), y(y), width(width), height(height), text(text),
      isHovered(false), isPressed(false), pressedTime(0.0) {}

Button::~Button() {}

float Button::getX() const { return x; }
float Button::getY() const { return y; }
float Button::getWidth() const { return width; }
float Button::getHeight() const { return height; }
const std::string& Button::getText() const { return text; }
bool Button::getIsHovered() const { return isHovered; }
bool Button::getIsPressed() const { return isPressed; }
double Button::getPressedTime() const { return pressedTime; }

void Button::setHovered(bool hovered) { isHovered = hovered; }
void Button::setPressed(bool pressed) { isPressed = pressed; }
void Button::setPressedTime(double time) { pressedTime = time; }
void Button::setText(const std::string& newText) { text = newText; }