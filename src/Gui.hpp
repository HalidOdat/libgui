#pragma once

#include <functional>

#include "Core/Type.hpp"
#include "Events/KeyEvent.hpp"
#include <Renderer/CameraController.hpp>
#include <Renderer/Renderer2D.hpp>

#include <Widget/Container.hpp>
#include <Widget/Row.hpp>
#include <Widget/Column.hpp>
#include <Widget/SizedBox.hpp>
#include <Widget/Input.hpp>
#include <Widget/Label.hpp>
#include <Widget/Button.hpp>

// Forward declare
struct GLFWwindow;

namespace Gui {

  // TODO: put events in a queue
  class Window final {
  public:
    using Handle = std::shared_ptr<Window>;
    using EventCallback = std::function<void(const Event&)>;

  public:
    static Window::Handle create(const char* title, const u32 width, const u32 height);
    DISALLOW_MOVE_AND_COPY(Window);
    ~Window();

    bool shouldClose();
    void setShouldClose();
    void update();

    void setVSync(bool enable);
    void setEventCallback(EventCallback callback);

    void setTitle(const String& title);

    void setSize(u32 width, u32 height);
    inline u32 getWidth()  const { return this->data.width; }
    inline u32 getHeight() const { return this->data.height; }
    inline f32 getAspectRatio() const { return (f32)this->data.width / (f32)this->data.height; }

    bool isKeyPressed(Key key) const;

    void enableCursor(bool yes = true);

  private:
    struct Data {
      GLFWwindow* window;
      u32 width;
      u32 height;
      EventCallback eventCallback;
    };

  private:
    Window(Data data)
      : data{ data }
    {}

    static void initializeWindowSystem();
    static void deinitializeWindowSystem();

  private:
    Data data;
  };

  class Application {
  public:
    Application(std::string title, u32 widget, u32 height);
    virtual ~Application() {}

    void run();
    u32 getWidth() { return mWidth; }
    u32 getHeight() { return mHeight; }
    float getTime() { return mTime; }
    Vec2 getMousePosition() { return mMousePosition; }
    Window::Handle getWindow() { return mWindow; }

    void resize(u32 width, u32 height);

    virtual void onUpdate() {}

    Widget::Handle getById(std::string_view id);

    template<typename T>
    std::vector<Widget::Handle> getByType() {
      std::vector<Widget::Handle> results;
      Widget::Visitor visitor = [&](Widget::Handle current){
        if (current->as<T>()) {
          results.push_back(current);
        }
        return true;
      };
      root->visit(root, visitor);
      return results;
    }

  public: // Don't use directly!
    void logicLoop();

  private:
    u32 mWidth  = 620;
    u32 mHeight = 480;
    float mTime;

    Window::Handle mWindow = nullptr;
    OrthographicCameraController mCamera;

  protected:
    Renderer2D renderer;
    float dt;
    float mLastFrameTime;

    Vec2 mMousePosition;

    Widget::Handle root;
  };

} // namespace Gui
