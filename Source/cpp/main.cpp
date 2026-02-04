// Sample from: https://www.glfw.org/documentation.html
#include <glad/gl.h>
// GLFW (include after glad)
#include <GLFW/glfw3.h>
#include <cstdio>

// math library includes
#include "glm/vec2.hpp"
#include "glm/vec3.hpp"
#include "glm/mat4x4.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"

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

struct vertex
{
    glm::vec2 pos;
    glm::vec3 col;
};
 
static const vertex vertices[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};
 
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
    
    GLuint vertex_buffer;
    glGenBuffers(1, &vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    const GLuint vertex_shader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex_shader, 1, &vertex_shader_text, NULL);
    glCompileShader(vertex_shader);
 
    const GLuint fragment_shader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment_shader, 1, &fragment_shader_text, NULL);
    glCompileShader(fragment_shader);
 
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertex_shader);
    glAttachShader(program, fragment_shader);
    glLinkProgram(program);
 
    const GLint mvp_location = glGetUniformLocation(program, "MVP");
    const GLint pos_location = glGetAttribLocation(program, "vPos");
    const GLint col_location = glGetAttribLocation(program, "vCol");
 
    GLuint vertex_array;
    glGenVertexArrays(1, &vertex_array);
    glBindVertexArray(vertex_array);
    glEnableVertexAttribArray(pos_location);
    glVertexAttribPointer(pos_location, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, pos)));  // NOLINT(performance-no-int-to-ptr)
    glEnableVertexAttribArray(col_location);
    glVertexAttribPointer(col_location, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, col)));  // NOLINT(performance-no-int-to-ptr)
    
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = static_cast<float>(width) / static_cast<float>(height);
 
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
 
        glm::mat4 m, p, mvp;
        m = glm::mat4(1.0f);
        m = glm::rotate(m, static_cast<float>(glfwGetTime()), glm::vec3(1.0f, 1.0f, 1.0f));
        p = glm::ortho( -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mvp =  p * m;
 
        glUseProgram(program);
        glUniformMatrix4fv(mvp_location, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&mvp));
        glBindVertexArray(vertex_array);
        glDrawArrays(GL_TRIANGLES, 0, 3);
 
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// sample from https://github.com/g-truc/glm/blob/master/readme.md
// #include <glm/vec3.hpp> // glm::vec3
// #include <glm/vec4.hpp> // glm::vec4
// #include <glm/mat4x4.hpp> // glm::mat4
// #include <glm/ext/matrix_transform.hpp> // glm::translate, glm::rotate, glm::scale
// #include <glm/ext/matrix_clip_space.hpp> // glm::perspective
// #include <glm/ext/scalar_constants.hpp> // glm::pi
//
// glm::mat4 camera(float Translate, glm::vec2 const& Rotate)
// {
//     glm::mat4 Projection = glm::perspective(glm::pi<float>() * 0.25f, 4.0f / 3.0f, 0.1f, 100.f);
//     glm::mat4 View = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, -Translate));
//     View = glm::rotate(View, Rotate.y, glm::vec3(-1.0f, 0.0f, 0.0f));
//     View = glm::rotate(View, Rotate.x, glm::vec3(0.0f, 1.0f, 0.0f));
//     glm::mat4 Model = glm::scale(glm::mat4(1.0f), glm::vec3(0.5f));
//     return Projection * View * Model;
// }