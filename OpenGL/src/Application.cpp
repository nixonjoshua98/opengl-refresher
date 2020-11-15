#include "Application.h"

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"
#include "Texture.h"
#include "Renderer.h"

#include "Camera.h"

#include "IndexBuffer.h"
#include "VertexArray.h"
#include "VertexBuffer.h"

Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

float lastX = 800 / 2.0f;
float lastY = 600 / 2.0f;
bool firstMouse = true;

class GameObject
{
private:
    glm::mat4 m_Model;

public:
    Texture* texture;

    GameObject(const std::string& textureFile) : m_Model(0)
    {
        texture = new Texture(textureFile);

        m_Model = glm::mat4(1.0f);
    }

    glm::mat4 GetModelMatrix() { return m_Model; }
};


void FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(yoffset);
}


Application::Application() : window(0)
{
    Initialize();
}

Application::~Application()
{

}

int Application::Run()
{
    float vertices[] = {
        -0.5f,  0.5f,  0.5f,    0.0f, 1.0f,
         0.5f,  0.5f,  0.5f,    1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,    1.0f, 0.0f,
        -0.5f,  -0.5f, 0.5f,    0.0f, 0.0f,
        
        -0.5f,  0.5f,  -0.5f,    0.0f, 1.0f,
         0.5f,  0.5f,  -0.5f,    1.0f, 1.0f,
         0.5f, -0.5f,  -0.5f,    1.0f, 0.0f,
        -0.5f,  -0.5f, -0.5f,    0.0f, 0.0f,
    };

    unsigned int indices[] = {
        0, 1, 2,    0, 2, 3,

        4, 5, 6,    4, 6, 7,
    };    

    // Create shader
    Shader coreShader("res/shaders/Shader.shader");

    m_Shaders.push_back(coreShader);

    coreShader.Bind();

    // Create object
    GameObject object("res/textures/image.png");

    m_Objects.push_back(object);

    object.texture->Bind();
    // --

    VertexBuffer VBO(vertices, sizeof(vertices));

    m_IBO = new IndexBuffer(indices, sizeof(indices) / sizeof(unsigned int));
    
    coreShader.SetUniform1i("u_Texture", 0);

    VertexBufferLayout layout;

    layout.Push<float>(3);
    layout.Push<float>(2);

    m_VAO = new VertexArray();

    m_VAO->AddBuffer(VBO, layout);

    glfwSetFramebufferSizeCallback(window, FrameBufferSizeCallback);

    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    /* Loop until the user closes the window */
    while (!glfwWindowShouldClose(window))
    {
        Input();

        OnRender();

        glfwPollEvents();
    }

    glfwTerminate();

    return 0;
}

void Application::Input()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::FORWARD, 0.1f);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::BACKWARD, 0.1f);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::LEFT, 0.1f);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.ProcessKeyboard(CameraMovement::RIGHT, 0.1f);

}

void Application::OnRender()
{
    m_Renderer.Clear();

    Shader shader = m_Shaders[0];

    shader.Bind();

    glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)800 / (float)600, 0.1f, 100.0f);

    shader.SetUniformMat4f("u_View", camera.GetViewMatrix());

    for (unsigned int i = 0; i < m_Objects.size(); ++i)
    {
        GameObject current = m_Objects[i];

        shader.SetUniformMat4f("u_Projection", projection);

        shader.SetUniformMat4f("u_Model", current.GetModelMatrix());
        
        m_Renderer.Draw(*m_VAO, *m_IBO, shader);
    }

    glfwSwapBuffers(window);
}

int Application::Initialize()
{
    if (!Init_OpenGl())
        return -1;

    return 0;
}

int Application::Init_OpenGl()
{
    if (!glfwInit())
    {
        std::cout << "Error: GLFW Init" << std::endl;

        return -1;
    }

    CreateWindow();

    if (glewInit() != GLEW_OK)
    {
        std::cout << "Error: GLEW Init" << std::endl;

        return -1;
    }

    glEnable(GL_BLEND);
    glEnable(GL_DEPTH_TEST);

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;

    return 0;
}

int Application::CreateWindow()
{
    if (!glfwInit())
        return -1;

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    /* Create a windowed mode window and its OpenGL context */
    window = glfwCreateWindow(800, 600, "Hello World", NULL, NULL);

    if (!window)
    {
        glfwTerminate();

        std::cout << "Failed to create window" << std::endl;
    }

    else
    {
        glViewport(0, 0, 800, 600);

        glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        glfwMakeContextCurrent(window);
    }

    return window ? 0 : -1;
}
