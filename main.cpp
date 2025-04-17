#define STB_IMAGE_IMPLEMENTATION
#include "lib/stb_image.h"
#define STB_EASY_FONT_IMPLEMENTATION
#include "lib/stb_easy_font.h"

#include "Window.hpp"
#include <iostream>

int main() {
    try {
        Window window;
        window.run();
    } catch (const std::exception& e) {
        std::cerr << "Hata: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
