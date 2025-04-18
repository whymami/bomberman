#ifndef SLIDER_HPP
#define SLIDER_HPP

#include <string>

class Slider {
private:
    float x;
    float y;
    float width;
    float height;
    float value;  // Current value (0.0 to 1.0)
    std::string label;
    bool isDragging;

public:
    Slider(float x, float y, float width, float height, const std::string& label, float initialValue = 0.5f);
    ~Slider();

    void draw() const;
    bool checkMousePosition(double mouseX, double mouseY);
    void updateValue(double mouseX);
    void setDragging(bool dragging);
    
    float getValue() const;
    void setValue(float newValue);
    float getX() const;
    float getY() const;
    float getWidth() const;
    float getHeight() const;
    const std::string& getLabel() const;
    bool getIsDragging() const;
};

#endif 