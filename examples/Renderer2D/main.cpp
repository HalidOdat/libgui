#include <Gui.hpp>

class MyApplication : public Gui::Application {
public:
  MyApplication()
    : Application("LibGUI - Renderer 2D Example", 800, 800)
  {}

  void onUpdate() override {
    renderer.drawCenteredCircle(
        {getWidth()/2 + getWidth()/5 * cosf(getTime() + getTime()/3), getHeight()/2 + getWidth()/5 * sinf(getTime() + getTime()/3) },
        50,
        {1.0f, 0.0f, 0.5f, 1.0f}
    );

    renderer.drawCenteredQuad(
        {getWidth()/2 + getWidth()/4 * cosf(getTime() + getTime()/2), getHeight()/2 + getWidth()/4 * sinf(getTime() + getTime()/2) },
        {60.0f + 10 * sinf(getTime()), 60.0f},
        {1.0f, 0.0f, 0.8f, 1.0f},
        Gui::Effect::Type::Static
    );

    renderer.drawCenteredQuad(
        {getWidth()/2 + getWidth()/3 * cosf(getTime()), getHeight()/2 + getWidth()/3 * sinf(getTime()) },
        {90.0f, 80.0f + 20 * cosf(getTime())},
        {0.2f, 1.0f, 0.8f, 1.0f},
        Gui::Effect::Type::Striped
    );

    renderer.drawCenteredCircle(
        {getWidth()/2 + getWidth()/2.5 * cosf(getTime()), getHeight()/2 + getWidth()/2.5 * sinf(getTime()) },
        60,
        {1.0f, 0.5f, 1.0f, 1.0f}
    );

    renderer.drawCenteredCircle(
        {getWidth()/2, getHeight()/2},
        20 + 30 * (cosf(getTime()) + 1),
        {0.5f, 0.5f, 1.0f, 1.0f}
    );
  }
};

int main() {
    MyApplication().run();
}
