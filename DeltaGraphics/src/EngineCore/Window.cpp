#include "EngineCore/Window.hpp"

#include <GLFW/glfw3.h>

#include "EngineCore/Event.hpp"
#include "EngineCore/Rendering/OpenGL/Renderer.hpp"
#include "EngineCore/Modules/GUIModule.hpp"

namespace Delta
{

int Window::init(unsigned int aWindowWidth, unsigned int aWindowHeight, const char* aTitle)
{
    mData.width = aWindowWidth;
    mData.height = aWindowHeight;
    mData.title = std::string(aTitle);

    LOG_INFO("Creating window {0} ({1}, {2})", mData.title, mData.width, mData.height);

    glfwSetErrorCallback([](int errorCode, const char* description)
    {
        LOG_ERROR("GLFW error {0}: {1}", errorCode, description);
    });

    if (!glfwInit())
    {
        LOG_ERROR("GLFW initialization failed");
        return -1;
    }
    LOG_INFO("GLFW initialized succesfully");

    mWindow = glfwCreateWindow(aWindowWidth, aWindowHeight, aTitle, nullptr, nullptr);
    if (!mWindow)
    {
        LOG_ERROR("GLFW window creation failed");
        glfwTerminate();
        return -2;
    }
    LOG_INFO("GLFW window created succesfully");

    if (!Renderer::init(mWindow))
        return -3;

    glfwSetWindowUserPointer(mWindow, &mData);

    glfwSetWindowSizeCallback(mWindow, [](GLFWwindow* window, int width, int height)
    {
        const auto params = static_cast<WindowParameters*>(glfwGetWindowUserPointer(window));
        params->width = width;
        params->height = height;

        WindowResizeEvent event(width, height);
        params->eventCallback(event);
    });

    glfwSetCursorPosCallback(mWindow, [](GLFWwindow* window, double xpos, double ypos)
    {
        const auto params = static_cast<WindowParameters*>(glfwGetWindowUserPointer(window));
        MouseMoveEvent event(xpos, ypos);
        params->eventCallback(event);
    });

    glfwSetKeyCallback(mWindow, [](GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        const auto params = static_cast<WindowParameters*>(glfwGetWindowUserPointer(window));

        switch (action)
        {
            case GLFW_PRESS:
            {
                KeyPressedEvent event(static_cast<KeyCode>(key), false);
                params->eventCallback(event);
                break;
            }
            case GLFW_REPEAT:
            {
                KeyPressedEvent event(static_cast<KeyCode>(key), true);
                params->eventCallback(event);
                break;
            }
            case GLFW_RELEASE:
            {
                KeyReleasedEvent event(static_cast<KeyCode>(key));
                params->eventCallback(event);
                break;
            }
            default:
                break;
            }
    });

    glfwSetWindowCloseCallback(mWindow, [](GLFWwindow* window)
    {
        const auto params = static_cast<WindowParameters*>(glfwGetWindowUserPointer(window));
        WindowCloseEvent event;
        params->eventCallback(event);
    });

    glfwSetFramebufferSizeCallback(mWindow, [](GLFWwindow* window, int width, int height)
    {
        Renderer::viewport(width, height);
    });

    Renderer::viewport(mData.width, mData.height);

    GUIModule::onWindowCreate(mWindow);

    return 0;
}

void Window::shutdown()
{
    GUIModule::onWindowClose();

    glfwDestroyWindow(mWindow);
    glfwTerminate();

    LOG_INFO("Window terminated");
}

void Window::onUpdate()
{
    glfwSwapBuffers(mWindow);
    glfwPollEvents();
}

} // namespace Delta
