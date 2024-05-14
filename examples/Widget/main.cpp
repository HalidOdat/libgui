#include <Gui.hpp>

using namespace Gui;

// Define some complement colors
static constexpr auto COLOR1 = rgba(0x00688BFF);
static constexpr auto COLOR2 = rgba(0x03A89EFF);
static constexpr auto COLOR3 = rgba(0xCD3700FF);
static constexpr auto COLOR4 = rgba(0xFF6103FF);

/// Specify the padding for each row/column widget.
static constexpr Vec4 PADDING = {2.5, 2.5, 2.5, 2.5};

bool callback(Widget::ClickEvent event) {
  auto container = (Container*)event.target;

  if (event.button == MouseButton::_1) {
    if (container->as<Column>()) {
      auto child1 = Row::create();
      child1->setAlignment(Alignment::Center);
      child1->setColor(COLOR1);
      child1->setPadding(PADDING);
      child1->addClickEventHandler(callback);

      auto child2 = Row::create();
      child2->setAlignment(Alignment::Center);
      child2->setColor(COLOR3);
      child2->setPadding(PADDING);
      child2->addClickEventHandler(callback);

      container->addChild(child1);
      container->addChild(child2);
    } else {
      auto child1 = Column::create();
      child1->setAlignment(Alignment::Center);
      child1->setColor(COLOR2);
      child1->setPadding(PADDING);
      child1->addClickEventHandler(callback);

      auto child2 = Column::create();
      child2->setAlignment(Alignment::Center);
      child2->setColor(COLOR4);
      child2->setPadding(PADDING);
      child2->addClickEventHandler(callback);

      container->addChild(child1);
      container->addChild(child2);
    }
    return false;
  } else if (event.button == MouseButton::_2 && event.target->parent) {
    if (auto container = event.target->parent->as<Container>()) {
      container->clearChildren();
    }
    return false;
  }

  // Propagate the event
  return true;
}

class MyApplication : public Application {
public:
  MyApplication() : Application("Hello There", 800, 800)
  {
    auto newRoot = Column::create();
    newRoot->setAlignment(Alignment::Center);
    newRoot->setColor(Color::WHITE);
    newRoot->setPadding(PADDING);
    newRoot->addClickEventHandler(callback);

    // Override default root.
    root = newRoot;
  }
};

int main() {
  MyApplication().run();
}
