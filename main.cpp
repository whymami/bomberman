#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>
#define STB_EASY_FONT_IMPLEMENTATION
#include <stb_easy_font.h>

#include <Window.hpp>
#include <iostream>
#include <utils/json_loader.hpp>

int main() {
    try {
        Window window;
        window.run();
        load_json("config.json");
    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
