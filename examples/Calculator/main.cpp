#include <Gui.hpp>
#include <eval.hpp>

using namespace Gui;

auto string = R"(
row:
  padding: 200
  children:
    input:
      id: result
    column:
      children:
        button:
          id: button7
          text: 7
          color: red
          background: 0x101010
        button:
          id: button8
          text: 8
          color: red
          background: 0x101010
        button:
          id: button9
          text: 9
          color: red
          background: 0x101010
        button:
          id: submit
          text: Calculate
          color: red
          background: 0x101010
    column:
      children:
        button:
          id: button4
          text: 4
          color: red
          background: 0x101010
        button:
          id: button5
          text: 5
          color: red
          background: 0x101010
        button:
          id: button6
          text: 6
          color: red
          background: 0x101010
        button:
          id: clear
          text: Clear
          color: red
          background: 0x101010
    column:
      children:
        button:
          id: button1
          text: 1
          color: red
          background: 0x101010
        button:
          id: button2
          text: 2
          color: red
          background: 0x101010
        button:
          id: button3
          text: 3
          color: red
          background: 0x101010
        button:
          id: plus
          text: '+'
          color: red
          background: 0x101010
    column:
      children:
        button:
          id: button0
          text: 0
          color: red
          background: 0x101010
        button:
          id: dot
          text: '.'
          color: red
          background: 0x101010
        button:
          id: mul
          text: '*'
          color: red
          background: 0x101010
        button:
          id: div
          text: '/'
          color: red
          background: 0x101010
        button:
          id: minus
          text: '-'
          color: red
          background: 0x101010
)";

class MyApplication : public Application {
public:
  MyApplication() : Application("Hello There", 800, 800)
  {
    YAML::Node node = YAML::Load(string);

    std::vector<DeserializationError> errors;
    Widget::Handle widget = Widget::deserialize(node, errors);

    if (!errors.empty()) {
      for (auto& error : errors) {
        Logger::error("YAML:%d:%d: %s", error.line, error.column, error.message.c_str());
      }

      exit(1);
    }

    // Override default root.
    root = widget;

    auto input = getById("result");

    auto buttons = getByType<Button>();
    for (auto button : buttons) {
      if (button->getId() == "submit" || button->getId() == "clear") {
        continue;
      }

      button->addClickEventHandler([input, button](auto event) -> bool {
        input->as<Input>()->setText(input->as<Input>()->getText() + button->as<Button>()->getText());
        return true;
      });
    }

    auto sumbit = getById("submit");
    sumbit->addClickEventHandler([input](auto event) -> bool {
      try {
          double result = evaluate(input->as<Input>()->getText());
          input->as<Input>()->setText(std::to_string(result));
      } catch (const std::exception &ex) {
          // std::cerr << "Error: " << ex.what() << std::endl;
      }
      return true;
    });

    auto clear = getById("clear");
    clear->addClickEventHandler([input](auto event) -> bool {
      input->as<Input>()->setText("");
      return true;
    });
  }
};

int main() {
  MyApplication().run();
}
