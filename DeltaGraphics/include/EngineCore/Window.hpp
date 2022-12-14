#pragma once

struct GLFWwindow;

namespace Delta
{

class EventBase;

class Window
{
public:
    using EventCallback = std::function<void(EventBase&)>;

    Window() = default;
    ~Window() = default;

    Window(const Window&) = delete;
    Window(Window&&) = delete;
    Window& operator=(const Window&) = delete;
    Window& operator=(const Window&&) = delete;

    int init(unsigned int aWindowWidth, unsigned int aWindowHeight, const char* aTitle);
    void shutdown();
    void onUpdate();

    unsigned int getWidth() const { return mData.width; }
    unsigned int getHeight() const { return mData.height; }

    void setEventCallback(EventCallback aEventCallbeck) { mData.eventCallback = std::move(aEventCallbeck); }

private:
    struct WindowParameters
    {
        unsigned int width{ 0 };
        unsigned int height{ 0 };
        std::string title;
        EventCallback eventCallback;
    };

    GLFWwindow* mWindow{ nullptr };
    WindowParameters mData;

};

} // namespace Delta
