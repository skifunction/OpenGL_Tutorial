#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Vertex Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 position;

uniform mat4 modelMatrix;

void main() {
    gl_Position = modelMatrix * vec4(position, 1.0);
}
)";

// Fragment Shader source code
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;

uniform vec4 colour;
void main() {
    FragColor = colour; // Set color
}
)";

int main() {
    // Initialize GLFW
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return -1;
    }

    // Configure GLFW for OpenGL 3.3 Core Profile
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a windowed mode window and its OpenGL context
    int height = 600;
    int width = 800;
    GLFWwindow* window = glfwCreateWindow(width, height, "Exercise", NULL, NULL);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);

    // Load all OpenGL function pointers using GLAD
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // Set the viewport size and callback
    glViewport(0, 0, width, height);

    // Build and compile the shader program
    int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Define vertices and indices of object
    float vertices[] = {
      -1.0f, -1.0f, 0.0f,
      -1.0f, +1.0f, 0.0f,
      +1.0f, +1.0f, 0.0f,
      +1.0f, -1.0f, 0.0f
    };

    unsigned int indices[] = {
        0,1,2,
        0,3,2
    };

    // Define position, rotation and scale of the object
    glm::vec3 position = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 rotation = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 scaling = glm::vec3(0.5f, 0.5f, 1.0f);

    // Create VBO, VAO and EBO using this data
    unsigned int VAO, VBO, EBO;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // Define attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Unbind VAO
    glBindVertexArray(0);

    // Delta time
    float prevTime = glfwGetTime();
    float deltaTime;
    
    float velocity = 0.4f; // Velocity to move object

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Update delta time
        float currentTime = glfwGetTime();
        deltaTime = currentTime - prevTime;
        prevTime = currentTime;

        // Clear colour of screen, set a background colour
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Update the values
        position[0] += velocity * deltaTime;

        // Bind the Shader and set Uniforms
        glUseProgram(shaderProgram);

        std::string colourName = "colour";
        int colourLoc = glGetUniformLocation(shaderProgram, colourName.c_str());
        glUniform4f(colourLoc, 1.0f, 0.0f, 0.0f, 1.0f);

        glm::mat4 translate = glm::translate(glm::mat4(1.0f), position);
        glm::mat4 rotateX = glm::rotate(glm::mat4(1.0f), rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotateY = glm::rotate(glm::mat4(1.0f), rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotateZ = glm::rotate(glm::mat4(1.0f), rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), scaling);

        glm::mat4 rotationMatrix = scale * rotateY * rotateZ * rotateX * glm::mat4(1.0f);
        glm::mat4 modelMatrix = translate * rotationMatrix;

        std::string modelMatName = "modelMatrix";
        int modelMatLoc = glGetUniformLocation(shaderProgram, modelMatName.c_str());
        glUniformMatrix4fv(modelMatLoc, 1, GL_FALSE, &modelMatrix[0][0]);

        // Bind the VAO (automatically binds the assosiated VBO) and EBO
        glBindVertexArray(VAO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // Draw the triangles
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
