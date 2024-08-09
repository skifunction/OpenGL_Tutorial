// #include "glad/glad.h"
// #include "GLFW/glfw3.h"
// #include "glm/glm.hpp"
// #include "glm/gtc/matrix_transform.hpp"
// #include <iostream>
// #include <vector>
// #include <map>
// #include <ctime>
// #include <string>
// #include <sstream>
// #include <fstream>
// #include <unordered_map>

// void GLClearError() { while (glGetError() != GL_NO_ERROR); }
// void GLDetectError(const char* file, int line)
// {
//     GLenum errorCode = glGetError();
//     if (errorCode != 0)
//     {
//         std::cerr << "GLerror at " << file << ":" << line << ": "
//             << "Error Code " << errorCode << std::endl;
//         exit(EXIT_FAILURE);
//     }

// }
// #define GLCall(x) GLClearError();x;GLDetectError(__FILE__, __LINE__);

// class VertexBuffer
// {
// private:
//     unsigned int ID;
// public:
//     VertexBuffer(const void* data, unsigned int size)
//     {
//         // Initialize Vextex Buffer
//         GLCall(glGenBuffers(1, &ID));
//         GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
//         GLCall(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW));
//     }
//     ~VertexBuffer()
//     {
//         // Delete VBO
//         GLCall(glDeleteBuffers(1, &ID));
//     }
//     void Bind()
//     {
//         // Bind/Use VBO
//         GLCall(glBindBuffer(GL_ARRAY_BUFFER, ID));
//     }
//     void Unbind()
//     {
//         // Unbind VBO
//         GLCall(glBindBuffer(GL_ARRAY_BUFFER, 0));
//     }
//     void SubData(float* data, unsigned int size)
//     {
//         // Substitute VBO data
//         Bind();
//         GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, size, (const void*)data));
//         Unbind();
//     }
//     unsigned int GetID()
//     {
//         return ID;
//     }
// };
// class IndexBuffer
// {
// private:
//     unsigned int ID;
//     unsigned int Count;
// public:
//     IndexBuffer(const unsigned int* data, unsigned int count)
//     {
//         // No. of indices to draw
//         Count = count;

//         // Initialize IBO
//         GLCall(glGenBuffers(1, &ID));
//         GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
//         GLCall(glBufferData(GL_ELEMENT_ARRAY_BUFFER, Count * sizeof(unsigned int), data, GL_STATIC_DRAW));
//     }
//     ~IndexBuffer()
//     {
//         // Delete IBO
//         GLCall(glDeleteBuffers(1, &ID));
//     }
//     void Bind()
//     {
//         // Bind/Use IBO
//         GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ID));
//     }
//     void Unbind()
//     {
//         // Unbind IBO
//         GLCall(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
//     }
//     unsigned int GetID()
//     {
//         return ID;
//     }
//     unsigned int GetCount()
//     {
//         return Count;
//     }
//     void SubData(unsigned int* data, unsigned int count)
//     {
//         // Substitute IBO data
//         Bind();
//         GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, count * sizeof(unsigned int), data));
//         Unbind();
//     }

// };
// typedef struct
// {
//     unsigned int type;
//     unsigned int count;
//     unsigned char normalized;
//     unsigned int typeSize;
// }LayoutElement;
// class VertexBufferLayout
// {
// private:
//     std::vector<LayoutElement> Elements;
//     unsigned int layoutSize;
// public:
//     VertexBufferLayout()
//     {
//         layoutSize = 0;
//     }

//     template<typename T>
//     void Push(unsigned int count)
//     {
//         /*static_assert(false);*/
//     }

//     template <>
//     void Push<float>(unsigned int count)
//     {
//         Elements.push_back({ GL_FLOAT, count, GL_FALSE, sizeof(float) });
//         layoutSize += count * sizeof(float);
//     }

//     template <>
//     void Push<int>(unsigned int count)
//     {
//         Elements.push_back({ GL_INT, count, GL_FALSE, sizeof(int) });
//         layoutSize += count * sizeof(unsigned int);
//     }

//     template <>
//     void Push<unsigned int>(unsigned int count)
//     {
//         Elements.push_back({ GL_UNSIGNED_INT, count, GL_FALSE, sizeof(unsigned int) });
//         layoutSize += count * sizeof(int);
//     }

//     std::vector<LayoutElement> GetElements()
//     {
//         return Elements;
//     }

//     unsigned int GetStride()
//     {
//         return layoutSize;
//     }
// };
// class VertexArray
// {
// private:
//     unsigned int ID;
// public:
//     VertexArray()
//     {
//         // Initialize VAO
//         GLCall(glGenVertexArrays(1, &ID));
//     }
//     ~VertexArray()
//     {
//         // Delete VAO
//         GLCall(glDeleteVertexArrays(1, &ID));
//     }
//     void AddBuffer(VertexBuffer* VBO, VertexBufferLayout* VBL)
//     {
//         // Use VAO and VBO
//         Bind();
//         VBO->Bind();

//         // Assign Layout to VAO
//         std::vector<LayoutElement> elements = VBL->GetElements();
//         unsigned int offSet = 0;
//         for (unsigned int i = 0; i < elements.size(); i++)
//         {
//             LayoutElement element = elements[i];
//             GLCall(glEnableVertexAttribArray(i));
//             GLCall(glVertexAttribPointer(i, element.count, element.type, element.normalized, VBL->GetStride(), (const void*)offSet));
//             offSet += element.count * element.typeSize;
//         }


//     }
//     void Bind()
//     {
//         // Bind/Use VAO
//         GLCall(glBindVertexArray(ID));
//     }
//     void Unbind()
//     {
//         // Unbind VAO
//         GLCall(glBindVertexArray(0));
//     }
//     unsigned int GetID()
//     {
//         return ID;
//     }
// };
// class Shader
// {
// private:
//     unsigned int ID;
//     std::unordered_map<std::string, int> uniformLocationCache;

//     static std::pair<std::string, std::string> ParseShader(const std::string& filepath)
//     {
//         std::ifstream stream(filepath);
//         std::string line;
//         std::stringstream ss[2];

//         int NONE = -1, VERTEX = 0, FRAGMENT = 1;

//         int shaderType = NONE;

//         if (stream.is_open())
//         {
//             while (getline(stream, line))
//             {
//                 if (line.find("#shader") != std::string::npos)
//                 {
//                     if (line.find("vertex") != std::string::npos) shaderType = VERTEX;
//                     else if (line.find("fragment") != std::string::npos) shaderType = FRAGMENT;

//                 }
//                 else
//                 {
//                     if (shaderType != NONE) ss[shaderType] << line << "\n";
//                 }
//             }
//         }
//         else
//         {
//             std::cout << "Failed to open File: " << filepath << std::endl;
//         }


//         std::pair<std::string, std::string> shaders = { ss[0].str(), ss[1].str() };
//         return shaders;
//     }
//     static unsigned int CompileShader(unsigned int type, const std::string& source)
//     {
//         unsigned int id = glCreateShader(type);
//         const char* src = source.c_str();
//         GLCall(glShaderSource(id, 1, &src, nullptr));
//         GLCall(glCompileShader(id));

//         // Error Handling for Shader Compilation
//         int result;
//         GLCall(glGetShaderiv(id, GL_COMPILE_STATUS, &result)); // i: int, v: vector (array)
//         if (result == GL_FALSE)
//         {
//             int length;
//             GLCall(glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length));
//             char* message = (char*)malloc(length * sizeof(char));
//             GLCall(glGetShaderInfoLog(id, length, &length, message));
//             std::cout << "Failed to Compile " << (type == GL_VERTEX_SHADER ? "vertex" : "fragment") << "Shader" << std::endl;
//             std::cout << message << std::endl;
//             GLCall(glDeleteShader(id));
//             free(message);
//             return 0;
//         }

//         return id;
//     }
//     static unsigned int CreateShader(const std::string& vertexShader, const std::string& fragmentShader)
//     {
//         unsigned int program = glCreateProgram();
//         unsigned int vs = CompileShader(GL_VERTEX_SHADER, vertexShader);
//         unsigned int fs = CompileShader(GL_FRAGMENT_SHADER, fragmentShader);

//         glAttachShader(program, vs);
//         glAttachShader(program, fs);
//         glLinkProgram(program);
//         glValidateProgram(program);

//         glDeleteShader(vs);
//         glDeleteShader(fs);

//         return program;
//     }
//     int GetUniformLocation(const std::string& name)
//     {
//         if (uniformLocationCache.find(name) != uniformLocationCache.end())
//             return uniformLocationCache[name];

//         GLCall(int loc = glGetUniformLocation(ID, name.c_str()));
//         if (loc == -1) std::cerr << "Warning: uniform  " << name << "doesn't exist" << std::endl;

//         uniformLocationCache[name] = loc;
//         return loc;
//     }

// public:
//     Shader(const std::string& filepath)
//     {
//         std::pair<std::string, std::string> shaders = ParseShader(filepath);
//         std::string vertexShader = shaders.first;
//         std::string fragmentShader = shaders.second;

//         ID = CreateShader(vertexShader, fragmentShader);
//     }
//     ~Shader()
//     {
//         GLCall(glDeleteProgram(ID)); // Delete the shader
//     }

//     void Bind()
//     {
//         GLCall(glUseProgram(ID)); // Bind the shader
//     }
//     void Unbind()
//     {
//         GLCall(glUseProgram(0)); // Unbind the shader
//     }

//     void SetUniform4f(const std::string& name, float v0, float v1, float v2, float v3)
//     {
//         GLCall(glUniform4f(GetUniformLocation(name), v0, v1, v2, v3));
//     }
//     void SetUniform3f(const std::string& name, float v0, float v1, float v2)
//     {
//         GLCall(glUniform3f(GetUniformLocation(name), v0, v1, v2));
//     }
//     void SetUniformMatrix4fv(const std::string& name, const GLfloat* v)
//     {
//         /*glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, );*/
//         GLCall(glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, v));
//     }

// };
// typedef struct {
//     unsigned int TextureID;  // ID handle of the glyph texture
//     glm::ivec2   Size;       // Size of glyph
//     glm::ivec2   Bearing;    // Offset from baseline to left/top of glyph
//     unsigned int Advance;    // Offset to advance to next glyph
// }TextChar;
// class Text
// {
// private:
//     std::map<char, TextChar> textchars;
//     /*VertexBuffer* VBO;
//     VertexBufferLayout* VBL;
//     VertexArray* VAO;
//     IndexBuffer* IBO;*/
//     Shader* textShader;

// public:
//     Text(Shader* textshader)
//     {
//         textShader = textshader;
//         /*VAO = new VertexArray();*/

//         /*float vertices[] = {1.0f ,2.0f ,3.0f ,4.0f};
//         unsigned int indices[] = {1,2,3};

//         VBO = new VertexBuffer(vertices, sizeof(vertices));
//         IBO = new IndexBuffer(indices, sizeof(indices));

//         VBL = new VertexBufferLayout();
//         VBL->Push<float>(4);

//         VAO->AddBuffer(VBO, VBL);*/

//         FT_Library ft;
//         if (FT_Init_FreeType(&ft))
//         {
//             std::cout << "ERROR::FREETYPE: Could not init FreeType Library" << std::endl;
//         }

//         FT_Face face;
//         if (FT_New_Face(ft, "resources/fonts/arial1.ttf", 0, &face))
//         {
//             std::cout << "ERROR::FREETYPE: Failed to load font" << std::endl;
//             return;
//         }

//         FT_Set_Pixel_Sizes(face, 0, 48);
//         GLCall(glPixelStorei(GL_UNPACK_ALIGNMENT, 1)); // disable byte-alignment restriction

//         for (unsigned char c = 0; c < 128; c++)
//         {
//             // load character glyph 
//             if (FT_Load_Char(face, c, FT_LOAD_RENDER))
//             {
//                 std::cout << "ERROR::FREETYTPE: Failed to load Glyph" << std::endl;
//                 continue;
//             }
//             // generate texture
//             unsigned int texture;
//             glGenTextures(1, &texture);
//             glBindTexture(GL_TEXTURE_2D, texture);
//             glTexImage2D(
//                 GL_TEXTURE_2D,
//                 0,
//                 GL_RED,
//                 face->glyph->bitmap.width,
//                 face->glyph->bitmap.rows,
//                 0,
//                 GL_RED,
//                 GL_UNSIGNED_BYTE,
//                 face->glyph->bitmap.buffer
//             );
//             // set texture options
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//             // now store character for later use
//             TextChar textchar = {
//                 texture,
//                 glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
//                 glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
//                 face->glyph->advance.x
//             };
//             textchars.insert(std::pair<char, TextChar>(c, textchar));
//         }
//         FT_Done_Face(face);
//         FT_Done_FreeType(ft);
//     }
//     ~Text()
//     {
//         /*delete(VAO);
//         delete(VBO);
//         delete(VBL);
//         delete(IBO);*/
//     }
//     void RenderText(std::string text, float x, float y, float scale, glm::vec3 color)
//     {
//         unsigned int VAO, VBO;
//         glGenVertexArrays(1, &VAO);
//         glGenBuffers(1, &VBO);
//         glBindVertexArray(VAO);
//         glBindBuffer(GL_ARRAY_BUFFER, VBO);
//         glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
//         glEnableVertexAttribArray(0);
//         glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), 0);
//         glBindBuffer(GL_ARRAY_BUFFER, 0);
//         glBindVertexArray(0);

//         textShader->Bind();
//         textShader->SetUniform3f("textColor", color.x, color.y, color.z);
//         glBindVertexArray(VAO);

//         // iterate through all characters
//         std::string::const_iterator c;
//         for (c = text.begin(); c != text.end(); c++)
//         {
//             TextChar ch = textchars[*c];

//             float xpos = x + ch.Bearing.x * scale;
//             float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

//             float w = ch.Size.x * scale;
//             float h = ch.Size.y * scale;
//             // update VBO for each character
//             float vertices[6][4] = {
//                 { xpos,     ypos + h,   0.0f, 0.0f },
//                 { xpos,     ypos,       0.0f, 1.0f },
//                 { xpos + w, ypos,       1.0f, 1.0f },

//                 { xpos,     ypos + h,   0.0f, 0.0f },
//                 { xpos + w, ypos,       1.0f, 1.0f },
//                 { xpos + w, ypos + h,   1.0f, 0.0f }
//             };
//             // render glyph texture over quad
//             glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//             // update content of VBO memory
//             glBindBuffer(GL_ARRAY_BUFFER, VBO);
//             glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
//             glBindBuffer(GL_ARRAY_BUFFER, 0);
//             // render quad
//             glDrawArrays(GL_TRIANGLES, 0, 6);
//             // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//             x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
//         }
//         glBindVertexArray(0);
//         glBindTexture(GL_TEXTURE_2D, 0);

//         glDeleteVertexArrays(1, &VAO);
//         glDeleteBuffers(1, &VBO);
//         //glActiveTexture(GL_TEXTURE0);
//         //VAO->Bind();

//         //// iterate through all characters
//         //std::string::const_iterator c;
//         //for (c = text.begin(); c != text.end(); c++)
//         //{
//         //    TextChar ch = textchars[*c];

//         //    float xpos = x + ch.Bearing.x * scale;
//         //    float ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

//         //    float w = ch.Size.x * scale;
//         //    float h = ch.Size.y * scale;
//         //    // update VBO for each character

//         //    float vertices[] = {
//         //        xpos, ypos + h, 0.0f, 0.0f,
//         //        xpos, ypos, 0.0f, 1.0f,
//         //        xpos + w, ypos, 1.0f, 1.0f,
//         //        xpos + w, ypos + h, 1.0f, 0.0f
//         //    };

//         //    unsigned int indices[] = {
//         //        0,1,2,
//         //        0,3,2
//         //    };

//         //    // render glyph texture over quad
//         //    glBindTexture(GL_TEXTURE_2D, ch.TextureID);
//         //    // update content of memory
//         //    VBO->Bind();
//         //    GLCall(glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices));
//         //    VBO->Unbind();

//         //    IBO->Bind();
//         //    GLCall(glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, 0, sizeof(indices), indices));
//         //    IBO->Unbind();

//         //    // render quad
//         //    VAO->Bind();
//         //    IBO->Bind();
//         //    GLCall(glDrawElements(GL_TRIANGLES, IBO->GetCount(), GL_UNSIGNED_INT, nullptr));

//         //    // now advance cursors for next glyph (note that advance is number of 1/64 pixels)
//         //    x += (ch.Advance >> 6) * scale; // bitshift by 6 to get value in pixels (2^6 = 64)
//         //}
//         //VAO->Unbind();
//         //glBindTexture(GL_TEXTURE_2D, 0);
//     }
// };
// class Renderer
// {
// private:
// public:
//     void Draw(VertexArray* vao, IndexBuffer* ibo, Shader* shader, const std::string& primitivetype)
//     {
//         shader->Bind();
//         vao->Bind();
//         ibo->Bind();

//         if (primitivetype == "triangles")
//         {
//             GLCall(glDrawElements(GL_TRIANGLES, ibo->GetCount(), GL_UNSIGNED_INT, nullptr));
//         }
//         else if (primitivetype == "points")
//         {
//             GLCall(glDrawElements(GL_POINTS, ibo->GetCount(), GL_UNSIGNED_INT, nullptr));
//         }

//     }
//     void Clear(float v1, float v2, float v3, float v4)
//     {
//         glClearColor(v1, v2, v3, v4);
//         glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();
//     }
// };

// class Camera
// {
// private:
//     int height;
//     int width;
//     glm::vec3 up;
//     glm::mat4 viewMatrix;
//     glm::mat4 projectionMatrix;
//     glm::mat4 camMatrix;

//     std::vector<Shader*> shaders;

// public:
//     glm::vec3 camPosition;
//     glm::vec3 targetPoint;

//     Camera(int height0, int width0, std::vector<Shader*> shaders0)
//     {
//         // Set aspect ratio
//         height = height0;
//         width = width0;
//         shaders = shaders0;

//         // Set initial cam parameters
//         camPosition = glm::vec3(0.0f, 0.0f, 5.0f);
//         targetPoint = glm::vec3(0.0f, 0.0f, 0.0f);
//         up = glm::vec3(0.0f, 1.0f, 0.0f);

//         // Setup Matrices
//         viewMatrix = glm::mat4(1.0f);
//         projectionMatrix = glm::mat4(1.0f);
//         camMatrix = glm::mat4(1.0f);

//     }
//     void Update()
//     {
//         // Setup Matrices
//         viewMatrix = glm::lookAt(camPosition, targetPoint, up);
//         projectionMatrix = glm::ortho(-100.0f, 100.0f, -100.0f, 100.0f, 0.1f, 100.0f);
//         camMatrix = projectionMatrix * viewMatrix;

//         // Set uniforms
//         for (int i = 0; i < shaders.size(); i++)
//         {
//             shaders[i]->Bind();
//             shaders[i]->SetUniformMatrix4fv("cameraMatrix", &camMatrix[0][0]);
//         }

//     }
// };
// class App 
// {
// private:
//     GLFWwindow* window;
//     Renderer* renderer;
//     Camera* camera;
//     Text* text;
//     std::vector<Shader*> shaders;
//     ImGuiIO io;

//     int height;
//     int width;

// public:
//     App()
//     {
//         // Seed Random Generator
//         std::srand(static_cast<unsigned int>(std::time(nullptr)));

//         // Setup OpenGL and Imgui
//         height = 900, width = 900;
//         window = OpenGLInit(width, height);
//         ImGUIInit();

//         // Create renderer
//         renderer = new Renderer;

//         // Create Shaders
//         shaders.push_back(new Shader("resources/shaders/shader_1.txt"));
//         shaders.push_back(new Shader("resources/shaders/textshader.txt"));

//         // Create Camera;
//         camera = new Camera(height, width, shaders);

//         // Create Text Renderer
//         text = new Text(shaders[1]);

        
//         MainLoop();
//     }
//     ~App()
//     {
//         delete(camera);  
//         delete(text);
//         delete(renderer);
//         for (int i = 0; i < shaders.size(); i++) delete(shaders[i]);

//         ImGui_ImplOpenGL3_Shutdown();
//         ImGui_ImplGlfw_Shutdown();
//         ImGui::DestroyContext();

//         glfwDestroyWindow(window);
//         glfwTerminate();
//     }
//     GLFWwindow* OpenGLInit(unsigned int width, unsigned int height)
//     {
//         // Set up GLFW
//         glfwInit();
//         glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
//         glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
//         glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

//         // Create window
//         GLFWwindow* window0 = glfwCreateWindow(width, height, "Game", NULL, NULL);
//         if (!window0)
//         {
//             glfwTerminate();
//             return nullptr;
//         }
//         glfwSetWindowPos(window0, 500, 100);

//         // Create context
//         glfwMakeContextCurrent(window0);

//         // Turn on VSync (limit FPS to Monitor Refresh Rate)
//         glfwSwapInterval(1);


//         // Set up GLAD
//         if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//         {
//             std::cout << "Failed to initialize GLAD" << std::endl;
//             return nullptr;
//         }

//         // Enable depth buffer
//         glEnable(GL_DEPTH_TEST);
//         glDepthFunc(GL_LESS);

//         glEnable(GL_BLEND);
//         glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

//         // Define the viewport dimensions
//         glViewport(0, 0, width, height);

//         return window0;
//     }
//     void ImGUIInit()
//     {
//         IMGUI_CHECKVERSION();
//         ImGui::CreateContext();
//         io = ImGui::GetIO(); (void)io;
//         ImGui::StyleColorsDark();
//         ImGui_ImplGlfw_InitForOpenGL(window, true);
//         ImGui_ImplOpenGL3_Init("#version 330");
//     }
//     void DrawUI()
//     {
//         ImGui::Begin("My name is window, ImGui window");
//         ImGui::Text("Hello World!");
//         ImGui::End();

//         ImGui::Render();
//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//     }
//     void MainLoop() 
//     {
//         float prevTime = glfwGetTime();
//         while (!glfwWindowShouldClose(window))
//         {
//             renderer->Clear(0.0f, 0.0f, 0.0f, 1.0f);
//             DrawUI();

//             glfwSwapBuffers(window);
//             glfwPollEvents();
//         }
//     }
    
// };


// int main(void)
// {
//     App app;
//     return 0;
// }

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Callback to adjust the viewport when the window is resized
void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

// Process input (close the window when the ESC key is pressed)
void processInput(GLFWwindow* window) {
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// Vertex Shader source code
const char* vertexShaderSource = R"(
#version 330 core
layout(location = 0) in vec3 aPos;
void main() {
    gl_Position = vec4(aPos, 1.0);
}
)";

// Fragment Shader source code
const char* fragmentShaderSource = R"(
#version 330 core
out vec4 FragColor;
void main() {
    FragColor = vec4(1.0, 1.0, 1.0, 1.0); // Set color to white
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
    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL Triangle", NULL, NULL);
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
    glViewport(0, 0, 800, 600);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);

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

    // Set up vertex data and buffers and configure vertex attributes
    float vertices[] = {
        -0.5f, -0.5f, 0.0f,
         0.5f, -0.5f, 0.0f,
         0.0f,  0.5f, 0.0f
    };

    unsigned int VAO, VBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    // Render loop
    while (!glfwWindowShouldClose(window)) {
        // Input
        processInput(window);

        // Render
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        // Draw the triangle
        glUseProgram(shaderProgram);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        // Swap buffers and poll IO events
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // Cleanup
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteProgram(shaderProgram);

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
