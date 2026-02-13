// Sample from: https://www.glfw.org/documentation.html
#include <glad/gl.h>
// GLFW (include after glad)
#include <GLFW/glfw3.h>
#include <cstdio>

// math library includes
#include "glm/glm.hpp"
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
    auto errorCallback(int error, const char* description) -> void
    {
        static_cast<void>(fprintf(stderr, "Error: %s\n", description));
    }
    
    void inputCallback(GLFWwindow* window, int key, int scancode, int action, int mods) // NOLINT Required function signature
    {
        if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GLFW_TRUE);
        }
    }
    //static int buffer_width, buffer_height = 0;
    void bufferSizeCallback(GLFWwindow* window, int width, int height) // NOLINT required function signature
    {
        if (window != nullptr)
        {
            glViewport(0, 0, width, height);
        }
    }
}

struct vertex
{
    glm::vec2 pos;
    glm::vec3 col;
};
 
static constexpr vertex vertices[3] =
{
    { { -0.6f, -0.4f }, { 1.f, 0.f, 0.f } },
    { {  0.6f, -0.4f }, { 0.f, 1.f, 0.f } },
    { {   0.f,  0.6f }, { 0.f, 0.f, 1.f } }
};

// shaders
namespace
{
    const char* VertexShaderText =
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
 
    const char* FragmentShaderText =
    "#version 330\n"
    "in vec3 color;\n"
    "out vec4 fragment;\n"
    "void main()\n"
    "{\n"
    "    fragment = vec4(color, 1.0);\n"
    "}\n";
}

int main(void)
{
    glfwSetErrorCallback(errorCallback);
    
    // Initialize the library
    if (!glfwInit())
    {
        return -1;
    }

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(static_cast<int>(screen_size::width), static_cast<int>(screen_size::height), "Soul Engine", nullptr, nullptr);
    if (!window)
    {
        glfwTerminate();
        return -1;
    }

    // Set input callback
    glfwSetKeyCallback(window, inputCallback);
    
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
    
    glfwSetFramebufferSizeCallback(window, bufferSizeCallback);
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    GLuint vertexBuffer;
    glGenBuffers(1, &vertexBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, vertexBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
 
    const GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &VertexShaderText, nullptr);
    glCompileShader(vertexShader);
 
    const GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &FragmentShaderText, nullptr);
    glCompileShader(fragmentShader);
 
    const GLuint program = glCreateProgram();
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);
    glLinkProgram(program);
    glDetachShader(program, vertexShader);
    glDetachShader(program, fragmentShader);
 
    const GLint mvpLocation = glGetUniformLocation(program, "MVP");
    const GLint posLocation = glGetAttribLocation(program, "vPos");
    const GLint colLocation = glGetAttribLocation(program, "vCol");
 
    GLuint vertexArray;
    glGenVertexArrays(1, &vertexArray);
    glBindVertexArray(vertexArray);
    glEnableVertexAttribArray(posLocation);
    glVertexAttribPointer(posLocation, 2, GL_FLOAT, GL_FALSE,
                          sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, pos)));  // NOLINT(performance-no-int-to-ptr)
    glEnableVertexAttribArray(colLocation);
    glVertexAttribPointer(colLocation, 3, GL_FLOAT, GL_FALSE,
                          sizeof(vertex), reinterpret_cast<void*>(offsetof(vertex, col)));  // NOLINT(performance-no-int-to-ptr)
    
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CW);
    
    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window))
    {
        glfwGetFramebufferSize(window, &width, &height);
        const float ratio = static_cast<float>(width) / static_cast<float>(height);
 
        glViewport(0, 0, width, height);
        glClear(GL_COLOR_BUFFER_BIT);
 
        glm::mat4 m, p, mvp;
        m = glm::mat4(1.0f);
        m = glm::rotate(m, static_cast<float>(glfwGetTime()), glm::vec3(0.0f, 1.0f, 0.0f));
        p = glm::ortho( -ratio, ratio, -1.f, 1.f, 1.f, -1.f);
        mvp =  p * m;
 
        glUseProgram(program);
        glUniformMatrix4fv(mvpLocation, 1, GL_FALSE, reinterpret_cast<const GLfloat*>(&mvp));
        glBindVertexArray(vertexArray);
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

// sample from https://nicolbolas.github.io/oldtut/Basics/Tut01%20Making%20Shaders.html
// GLuint CreateShader(GLenum eShaderType, const std::string &strShaderFile)
// {
//     GLuint shader = glCreateShader(eShaderType);
//     const char *strFileData = strShaderFile.c_str();
//     glShaderSource(shader, 1, &strFileData, NULL);
//     
//     glCompileShader(shader);
//     
//     GLint status;
//     glGetShaderiv(shader, GL_COMPILE_STATUS, &status);
//     if (status == GL_FALSE)
//     {
//         GLint infoLogLength;
//         glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLogLength);
//         
//         GLchar *strInfoLog = new GLchar[infoLogLength + 1];
//         glGetShaderInfoLog(shader, infoLogLength, NULL, strInfoLog);
//         
//         const char *strShaderType = NULL;
//         switch(eShaderType)
//         {
//         case GL_VERTEX_SHADER: strShaderType = "vertex"; break;
//         case GL_GEOMETRY_SHADER: strShaderType = "geometry"; break;
//         case GL_FRAGMENT_SHADER: strShaderType = "fragment"; break;
//         }
//         
//         fprintf(stderr, "Compile failure in %s shader:\n%s\n", strShaderType, strInfoLog);
//         delete[] strInfoLog;
//     }
//
//     return shader;
// }

// GLuint CreateProgram(const std::vector<GLuint> &shaderList)
// {
//     GLuint program = glCreateProgram();
//     
//     for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
//         glAttachShader(program, shaderList[iLoop]);
//     
//     glLinkProgram(program);
//     
//     GLint status;
//     glGetProgramiv (program, GL_LINK_STATUS, &status);
//     if (status == GL_FALSE)
//     {
//         GLint infoLogLength;
//         glGetProgramiv(program, GL_INFO_LOG_LENGTH, &infoLogLength);
//         
//         GLchar *strInfoLog = new GLchar[infoLogLength + 1];
//         glGetProgramInfoLog(program, infoLogLength, NULL, strInfoLog);
//         fprintf(stderr, "Linker failure: %s\n", strInfoLog);
//         delete[] strInfoLog;
//     }
//     
//     for(size_t iLoop = 0; iLoop < shaderList.size(); iLoop++)
//         glDetachShader(program, shaderList[iLoop]);
//
//     return program;
// }

/*
*Buffer Object Usage Hints. Every time we draw something, we are changing the buffer object's data. 
*OpenGL has a way to tell it that you will be doing something like this, 
*and it is the purpose of the last parameter of glBufferData. 
*This tutorial changed the allocation of the buffer object slightly, replacing:
*
*===========================================================================================
*glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
*===========================================================================================
*with this:
*===========================================================================================
*glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STREAM_DRAW);
*===========================================================================================
*
*GL_STATIC_DRAW tells OpenGL that you intend to only set the data in this buffer object once. 
*GL_STREAM_DRAW tells OpenGL that you intend to set this data constantly, generally once per frame. 
*These parameters do not mean anything with regard to the API; they are simply hints to the OpenGL implementation. 
*Proper use of these hints can be crucial for getting good buffer object performance when making frequent changes. 
 */