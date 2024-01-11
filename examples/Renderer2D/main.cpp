#include <Window.hpp>
#include <Events/WindowEvent.hpp>
#include <Renderer/Renderer2D.hpp>
#include <Renderer/CameraController.hpp>
#include <Core/OpenGL.hpp>

int main() {
    Gui::u32 width  = 620;
    Gui::u32 height = 480;
    auto window = Gui::Window::create("Hello There", width, height);
    window->setVSync(true);

    auto renderer = Gui::Renderer2D(width, height);
    auto camera = Gui::OrthographicCameraController(width, height, window->getAspectRatio());
    camera.resize(width, height);

    window->setEventCallback(
      [&](const Gui::Event& event) {
        if (event.getType() == Gui::Event::Type::WindowResize) {
            auto resize = (Gui::WindowResizeEvent&)(event);
            width = resize.getWidth();
            height = resize.getHeight();
            glViewport(0, 0, width, height);
            printf("width = %d, height = %d\n", width, height);
            camera.resize(width, height);
            renderer.invalidate(width, height);
        }
      }
    );

    float lastFrameTime = (float)glfwGetTime();
    while (!window->shouldClose()) {
        float time = (float)glfwGetTime();
        float dt = time - lastFrameTime;
        lastFrameTime = time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.begin(camera.getCamera());
        renderer.clearScreen();
        
        renderer.drawCenteredCircle(
            {width/2 + width/5 * cosf(time + time/3), height/2 + width/5 * sinf(time + time/3) },
            50,
            {1.0f, 0.0f, 0.5f, 1.0f}
        );

        renderer.drawCenteredQuad(
            {width/2 + width/4 * cosf(time + time/2), height/2 + width/4 * sinf(time + time/2) },
            {60.0f + 10 * sinf(time), 60.0f},
            {1.0f, 0.0f, 0.8f, 1.0f},
            Gui::Effect::Type::Static
        );

        renderer.drawCenteredQuad(
            {width/2 + width/3 * cosf(time), height/2 + width/3 * sinf(time) },
            {90.0f, 80.0f + 20 * cosf(time)},
            {0.2f, 1.0f, 0.8f, 1.0f},
            Gui::Effect::Type::Striped
        );

        renderer.drawCenteredCircle(
            {width/2 + width/2.5 * cosf(time), height/2 + width/2.5 * sinf(time) },
            60,
            {1.0f, 0.5f, 1.0f, 1.0f}
        );

        renderer.drawCenteredCircle(
            {width/2, height/2},
            20 + 30 * (cosf(time) + 1),
            {0.5f, 0.5f, 1.0f, 1.0f}
        );

        renderer.end();
        window->update();
    }
}
