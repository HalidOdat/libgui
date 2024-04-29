#include <Gui.hpp>

class MyApplication : public Gui::Application {
public:
  MyApplication() : Application("Hello There", 800, 800)
  {
    mContainer = Gui::Container::create();
    auto child1 = Gui::SizedBox::create(100, 100);
    child1->setColor(Gui::Color::RED);
    auto child2 = Gui::SizedBox::create(100, 100);
    child2->setColor(Gui::Color::BLUE);
    mContainer->addChild(child1);
    mContainer->addChild(child2);

    mContainer->setPosition({200, 200});
    mContainer->setPadding({20, 20, 20, 20});

    mContainer->layout({0, 0, 1000, 1000});
    mContainer->setColor(Gui::Color::GREEN);
    mContainer->reportSize();
  }

  void onUpdate() override {
    mContainer->draw(renderer);
  }

private:
  Gui::Container::Handle mContainer;
};

int main() {
  MyApplication().run();
}
