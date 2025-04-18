#include "Slider.hpp"
#include <GLFW/glfw3.h>
#include "stb_easy_font.h"
#include <algorithm>

Slider::Slider(float x, float y, float width, float height, const std::string& label, float initialValue)
    : x(x), y(y), width(width), height(height), label(label), value(initialValue), isDragging(false) {}

Slider::~Slider() {}

void Slider::draw() const {
    // Draw label text first
    char buffer[99999];
    int num_quads;
    float scale = 2.0f;
    
    glColor3f(0.8f, 0.8f, 0.8f); // Light gray for text
    glPushMatrix();
    glTranslatef(x, y - 25, 0);
    glScalef(scale, scale, 1.0f);
    
    std::string displayText = label + ": " + std::to_string(static_cast<int>(value * 100)) + "%";
    
    glEnableClientState(GL_VERTEX_ARRAY);
    num_quads = stb_easy_font_print(0, 0, (char*)displayText.c_str(), NULL, buffer, sizeof(buffer));
    glVertexPointer(2, GL_FLOAT, 16, buffer);
    glDrawArrays(GL_QUADS, 0, num_quads * 4);
    glDisableClientState(GL_VERTEX_ARRAY);
    
    glPopMatrix();

    // Draw slider background (darker part)
    glBegin(GL_QUADS);
    glColor3f(0.15f, 0.15f, 0.15f); // Dark gray
    glVertex2f(x, y + height/4);
    glVertex2f(x + width, y + height/4);
    glVertex2f(x + width, y + 3*height/4);
    glVertex2f(x, y + 3*height/4);
    glEnd();

    // Draw filled part of the slider
    glBegin(GL_QUADS);
    glColor3f(0.4f, 0.4f, 0.4f); // Medium gray for filled part
    glVertex2f(x, y + height/4);
    glVertex2f(x + width * value, y + height/4);
    glVertex2f(x + width * value, y + 3*height/4);
    glVertex2f(x, y + 3*height/4);
    glEnd();

    // Draw slider handle
    float handleX = x + value * width;
    float handleWidth = 8.0f;
    
    // Handle shadow/3D effect
    glBegin(GL_QUADS);
    glColor3f(0.2f, 0.2f, 0.2f); // Darker gray for shadow
    glVertex2f(handleX - handleWidth/2, y);
    glVertex2f(handleX + handleWidth/2, y);
    glVertex2f(handleX + handleWidth/2, y + height);
    glVertex2f(handleX - handleWidth/2, y + height);
    glEnd();

    // Handle main part
    glBegin(GL_QUADS);
    glColor3f(0.7f, 0.7f, 0.7f); // Light gray for handle
    glVertex2f(handleX - handleWidth/2 + 1, y + 1);
    glVertex2f(handleX + handleWidth/2 - 1, y + 1);
    glVertex2f(handleX + handleWidth/2 - 1, y + height - 1);
    glVertex2f(handleX - handleWidth/2 + 1, y + height - 1);
    glEnd();

    // Draw border around the slider
    glBegin(GL_LINE_LOOP);
    glColor3f(0.1f, 0.1f, 0.1f); // Very dark gray for border
    glVertex2f(x, y + height/4);
    glVertex2f(x + width, y + height/4);
    glVertex2f(x + width, y + 3*height/4);
    glVertex2f(x, y + 3*height/4);
    glEnd();
}

bool Slider::checkMousePosition(double mouseX, double mouseY) {
    // Make the hit area a bit larger for better usability
    return (mouseX >= x && mouseX <= x + width &&
            mouseY >= y && mouseY <= y + height);
}

void Slider::updateValue(double mouseX) {
    if (isDragging) {
        float newValue = (mouseX - x) / width;
        value = std::max(0.0f, std::min(1.0f, newValue));
    }
}

void Slider::setDragging(bool dragging) {
    isDragging = dragging;
}

float Slider::getValue() const { return value; }
void Slider::setValue(float newValue) { value = std::max(0.0f, std::min(1.0f, newValue)); }
float Slider::getX() const { return x; }
float Slider::getY() const { return y; }
float Slider::getWidth() const { return width; }
float Slider::getHeight() const { return height; }
const std::string& Slider::getLabel() const { return label; }
bool Slider::getIsDragging() const { return isDragging; } 