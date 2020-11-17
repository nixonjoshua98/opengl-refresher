#include "Application.h"

#include <string>
#include <iostream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Renderer.h"
#include "VertexBuffer.h"
#include "ResourceCache.h"

float deltaTime = 0.0f;

Application::Application() : window(0)
{
    Initialize();

    Setup();
}

Application::~Application()
{

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


void Application::Setup()
{
    ResourceCache::LoadShader("s_Basic", "res/shaders/Shader.shader");

    ResourceCache::LoadTexture("t_Basic", "res/textures/wall.jpg");

    camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));

    m_Entities = std::vector<Block>();

    float x = -5.0f;

    for (int i = 0; i < 10; ++i)
    {
        x = i + (i * 0.25f);

        m_Entities.push_back({ glm::vec3(x, 0.0f, 0.0f) });
    }

    glfwSetWindowUserPointer(window, this);

    glfwSetFramebufferSizeCallback(window, Application::FrameBufferSizeCallback);

    glfwSetCursorPosCallback(window, Application::MouseMovementCallback);
}


int Application::Run()
{
    float vertices[] = {
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
    };

    VertexBuffer VBO(vertices, sizeof(vertices));

    VertexBufferLayout layout;

    layout.Push<float>(3);
    layout.Push<float>(2);

    m_VAO = new VertexArray();

    m_VAO->AddBuffer(VBO, layout);

    float lastFrame = 0.0f;

    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();

        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        glfwPollEvents();

        Input();

        Render();
    }

    glfwTerminate();

    return 0;
}

void Application::Input()
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::FORWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::BACKWARD, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera->ProcessKeyboard(CameraMovement::RIGHT, deltaTime);

}

void Application::Render()
{
    Renderer::Clear();

    Shader* shader = ResourceCache::GetShader("s_Basic");
    Texture2D* texture = ResourceCache::GetTexture("t_Basic");

    shader->Use();

    texture->Bind();

    shader->SetUniform1i("u_Texture", 0);

    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);

    shader->SetUniformMat4f("u_View", camera->GetViewMatrix());

    shader->SetUniformMat4f("u_Projection", projection);

    for (Block& entity : m_Entities)
    {
        shader->SetUniformMat4f("u_Model", entity.transform.GetModelMatrix());

        Renderer::DrawCube(m_VAO, shader);
    }   

    glfwSwapBuffers(window);
}

void Application::MouseMovementCallback(GLFWwindow* window, double xPos, double yPos)
{
    Application* app = static_cast<Application*>(glfwGetWindowUserPointer(window));
    
    app->camera->ProcessMouseMovement(xPos, yPos);
}

void Application::FrameBufferSizeCallback(GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}