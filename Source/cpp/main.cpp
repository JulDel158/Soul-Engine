// Sample from: https://www.glfw.org/documentation.html
#include <glad/gl.h>
// GLFW (include after glad)
#include <GLFW/glfw3.h>
#include <cstdio>

enum class screen_size : uint16_t
{
    width = 800,
    height = 600
};

namespace
{
    auto error_callback(int error, const char* description) -> void
    {
        static_cast<void>(fprintf(stderr, "Error: %s\n", description));
    }
}

int main(void)
{
    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }
    
    glfwSetErrorCallback(error_callback);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(static_cast<int>(screen_size::width), static_cast<int>(screen_size::height), "Soul Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    // Loading extension loader library (glad)
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        printf("Failed to initialize OpenGL context\n");
        return -1;
    }
    
    // Successfully loaded OpenGL
    printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        // Render here
        glClear(GL_COLOR_BUFFER_BIT);

        // Swap front and back buffers */
        glfwSwapBuffers(window);

        // Poll for and process events */
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}