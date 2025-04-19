#include "Button.hpp"
#include "stb_easy_font.h"

Button::Button(float x, float y, float width, float height, const std::string& text)
    : x(x), y(y), width(width), height(height), text(text),
      isHovered(false), isPressed(false), isDisabled(false), pressedTime(0.0) {}

float Button::getX() const { return x; }
float Button::getY() const { return y; }
float Button::getWidth() const { return width; }
float Button::getHeight() const { return height; }
const std::string& Button::getText() const { return text; }
bool Button::getIsHovered() const { return isHovered && !isDisabled; }
bool Button::getIsPressed() const { return isPressed; }
bool Button::getIsDisabled() const { return isDisabled; }
double Button::getPressedTime() const { return pressedTime; }

void Button::setHovered(bool hovered) { isHovered = hovered; }
void Button::setPressed(bool pressed) { isPressed = pressed; }
void Button::setDisabled(bool disabled) { isDisabled = disabled; }
void Button::setPressedTime(double time) { pressedTime = time; }
void Button::setText(const std::string& newText) { text = newText; }

void Button::drawText(float x, float y, const char* text, float r, float g, float b, float scale) {
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

void Button::draw() const {
    glBegin(GL_QUADS);
    if (isDisabled) {
        glColor3f(0.168f, 0.168f, 0.168f);  // Dark gray for disabled state
    } else if (isPressed) {
        glColor3f(0.2f, 0.2f, 0.2f);  // Slightly lighter gray for pressed state
    } else if (isHovered) {
        glColor3f(0.6f, 0.6f, 0.6f);
    } else {
        glColor3f(0.4f, 0.4f, 0.4f);
    }
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    glBegin(GL_LINE_LOOP);
    glColor3f(1.0f, 1.0f, 1.0f);
    glVertex2f(x, y);
    glVertex2f(x + width, y);
    glVertex2f(x + width, y + height);
    glVertex2f(x, y + height);
    glEnd();

    float scale = 2.0f;
    float textWidth = text.length() * 10 * scale;
    float textX = x + (width) / 2.5f;
    float textY = y + (height - scale * 12) / 1.5f;

    // Use a different color for disabled text
    if (isDisabled) {
        const_cast<Button*>(this)->drawText(textX, textY, text.c_str(), 0.8f, 0.8f, 0.8f, scale);  // Light gray text
    } else {
        const_cast<Button*>(this)->drawText(textX, textY, text.c_str(), 1.0f, 1.0f, 1.0f, scale);  // White text
    }
}