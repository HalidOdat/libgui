#include <Gui.hpp>
#include <eval.hpp>

using namespace Gui;

// Define some complement colors
static constexpr auto COLOR1 = rgba(0x00688BFF);
static constexpr auto COLOR2 = rgba(0x93B1A6FF);
static constexpr auto COLOR3 = rgba(0xCD3700FF);
static constexpr auto COLOR4 = rgba(0xFF6103FF);

auto string = R"(
row:
  padding: 80
  color: 0x040D12
  children:
    input:
      id: result
      font-size: 40
    column:
      children:
        button:
          id: button7
          text: 7
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: button8
          text: 8
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: button9
          text: 9
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: submit
          text: Calc
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
    column:
      children:
        button:
          id: button4
          text: 4
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: button5
          text: 5
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: button6
          text: 6
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: plus
          text: '+'
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
    column:
      children:
        button:
          id: button1
          text: 1
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: button2
          text: 2
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: button3
          text: 3
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: minus
          text: '-'
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
    column:
      children:
        button:
          id: button0
          text: 0
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: dot
          text: '.'
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: mul
          text: '*'
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
        button:
          id: div
          text: '/'
          font-size: 40
          color: 0x040D12
          background: 0x93B1A6
          margin: 20
)";

class MyApplication : public Application {
public:
  MyApplication() : Application("LibGUI - Calculator Example", 800, 800)
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
      if (button->getId() == "submit") {
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
  }
};

int main() {
  MyApplication().run();
}
