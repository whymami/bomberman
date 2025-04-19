#include <iostream>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <string>
#include <map>
#include <vector>
#include <Game.hpp>


int main()
{
    try
    {
        Game game;
        
        // Initialize GLFW
        if (!glfwInit())
        {
            std::cerr << "Failed to initialize GLFW" << std::endl;
            return -1;
        }

        // Create a windowed mode window and its OpenGL context
        GLFWwindow *window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        if (!window)
        {
            glfwTerminate();
            std::cerr << "Failed to create GLFW window" << std::endl;
            return -1;
        }

        // Make the window's context current
        glfwMakeContextCurrent(window);

        // Main loop
        while (!glfwWindowShouldClose(window))
        {
            // Render here
            glClear(GL_COLOR_BUFFER_BIT);

            // Swap front and back buffers
            glfwSwapBuffers(window);
            game.run(1);

            // Poll for and process events
            glfwPollEvents();
        }

        glfwDestroyWindow(window);
        glfwTerminate();

    }
    catch (const std::exception &e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return -1;
    }
    return 0;
}
