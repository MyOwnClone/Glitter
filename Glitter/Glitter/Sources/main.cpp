// Local Headers
#include "glitter.hpp"
#include <iostream>

// System Headers
//#include <glad/glad.h>
#include <GLFW/glfw3.h>

// Standard Headers
#include <cstdio>
#include <cstdlib>

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    // When a user presses the escape key, we set the WindowShouldClose property to true,
    // closing the application
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

const GLchar *vertexShaderSource =
"#version 330 core\n"
"layout (location = 0) in vec3 position;\n"
"layout (location = 1) in vec3 color;   \n"
"                                       \n"
"out vec3 ourColor;                     \n"
"                                       \n"
"void main()                            \n"
"{\n"
"    gl_Position = vec4(position.x, position.y, position.z, 1.0);\n"
"    ourColor = color;                  \n"
"}\n";

const GLchar *fragmentShaderSource =
"#version 330 core  \n"
"in vec3 ourColor;  \n"
"out vec4 color;    \n"
"void main()        \n"
"{                  \n"
"    color = vec4(ourColor, 1.0f);\n"
"}                  \n";

int main(int argc, char * argv[]) {

    // Load GLFW and Create a Window
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    auto window = glfwCreateWindow(mWidth, mHeight, "OpenGL", nullptr, nullptr);

    // Check for Valid Context
    if (window == nullptr) {
        fprintf(stderr, "Failed to Create OpenGL Context");
        return EXIT_FAILURE;
    }

    // Create Context and Load OpenGL Functions
    glfwMakeContextCurrent(window);
    //gladLoadGL();
    fprintf(stderr, "OpenGL %s\n", glGetString(GL_VERSION));
    
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK)
    {
        std::cout << "Failed to initialize GLEW" << std::endl;
        return -1;
    }
    
    GLint nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "Maximum nr of vertex attributes supported: " << nrAttributes << std::endl;
    
    int width, height;
    glfwGetFramebufferSize(window, &width, &height);
    glViewport(0, 0, width, height);
    
    glfwSetKeyCallback(window, key_callback);
    
    GLuint vertexShader;
    
    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);
    
    GLint success;
    GLchar infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    GLuint fragmentShader;
    
    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    
    if(!success)
    {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
    }
    
    GLuint shaderProgram;
    shaderProgram = glCreateProgram();
    
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if(!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::LINKING_FAILED\n" << infoLog << std::endl;
    }
    
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    
    GLuint VAO0;
    glGenVertexArrays(1, &VAO0);
    
    GLuint VBO0;
    glGenBuffers(1, &VBO0);
    
    GLuint EBO0;
    glGenBuffers(1, &EBO0);
    
    GLfloat vertices0[] = {
        -0.25, 0.0, 0.0,    1, 0, 0,
         0.00, 0.5, 0.0,    0, 1, 0,
         0.25, 0.0, 0.0,    0, 0, 1
    };
    
    GLuint indices0[] = {  // Note that we start from 0!
        0, 1, 2,   // First Triangle
    };
    
    glBindVertexArray(VAO0);
        glBindBuffer(GL_ARRAY_BUFFER, VBO0);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices0), vertices0, GL_STATIC_DRAW);
    
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO0);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices0), indices0, GL_STATIC_DRAW);
    
        // Position attribute
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)0);
        glEnableVertexAttribArray(0);
        // Color attribute
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), (GLvoid*)(3* sizeof(GLfloat)));
        glEnableVertexAttribArray(1);
    glBindVertexArray(0);
    
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    
    // Rendering Loop
    while (glfwWindowShouldClose(window) == false)
    {
        glfwPollEvents();
        
        // Background Fill Color
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        
        glUseProgram(shaderProgram);
        
        glBindVertexArray(VAO0);
        glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);

        // Flip Buffers and Draw
        glfwSwapBuffers(window);
    }
    
    glfwTerminate();
    
    return EXIT_SUCCESS;
}
