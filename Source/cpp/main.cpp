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
    
    void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
    static int buffer_width, buffer_height = 0;
    void buffer_size_callback(GLFWwindow* window, int width, int height)
    {
        buffer_width = width;
        buffer_height = height;
        if (window != nullptr)
        {
            glViewport(0, 0, buffer_width, buffer_height);
        }
    }
}

// typedef struct Vertex
// {
//     vec2 pos;
//     vec3 col;
// } Vertex;
//  
// static const Vertex vertices[3] =
// {
//     { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
//     { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
//     { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
// };
 
static const char* vertex_shader_text =
"#version 330\n"
"uniform mat4 MVP;\n"
"in vec3 vCol;\n"
"in vec2 vPos;\n"
"out vec3 color;\n"
"void main()\n"
"{\n"
"    gl_Position = MVP * vec4(vPos, 0.0, 1.0);\n"
"    color = vCol;\n"
"}\n";
 
static const char* fragment_shader_text =
"#version 330\n"
"in vec3 color;\n"
"out vec4 fragment;\n"
"void main()\n"
"{\n"
"    fragment = vec4(color, 1.0);\n"
"}\n";

int main(void)
{
    glfwSetErrorCallback(error_callback);
    
    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }
    
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(static_cast<int>(screen_size::width), static_cast<int>(screen_size::height), "Soul Engine", NULL, NULL);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Set input callback
    glfwSetKeyCallback(window, key_callback);
    
    // Make the window's context current
    glfwMakeContextCurrent(window);
    
    // Loading extension loader library (glad)
    int version = gladLoadGL(glfwGetProcAddress);
    if (version == 0)
    {
        printf("Failed to initialize OpenGL context\n");
        return -1;
    }
    
    // setting interval time for buffer swaps
    glfwSwapInterval(1);
    
    // Successfully loaded OpenGL
    printf("Loaded OpenGL %d.%d\n", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));
    
    glfwSetFramebufferSizeCallback(window, buffer_size_callback);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    double time = 0;
    
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        time = glfwGetTime();
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