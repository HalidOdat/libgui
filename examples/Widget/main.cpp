#include <Window.hpp>
#include <Events/WindowEvent.hpp>
#include <Renderer/Renderer2D.hpp>
#include <Renderer/CameraController.hpp>
#include <Core/OpenGL.hpp>

#include <Widget/Container.hpp>
#include <Widget/SizedBox.hpp>

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

    auto container = Gui::Container::create();
    auto child1 = Gui::SizedBox::create(100, 100);
    child1->setColor(Gui::Color::RED);
    auto child2 = Gui::SizedBox::create(100, 100);
    child2->setColor(Gui::Color::BLUE);
    container->addChild(child1);
    container->addChild(child2);

    container->setPosition({200, 200});

    container->setPadding({20, 20, 20, 20});

    container->layout({0, 0, 1000, 1000});
    container->setColor(Gui::Color::GREEN);
    container->reportSize();

    float lastFrameTime = (float)glfwGetTime();
    while (!window->shouldClose()) {
        float time = (float)glfwGetTime();
        float dt = time - lastFrameTime;
        lastFrameTime = time;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        renderer.begin(camera.getCamera());
        renderer.clearScreen();
        
        container->draw(renderer);

        renderer.end();
        window->update();
    }
}
