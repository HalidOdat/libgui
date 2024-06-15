#include <Gui.hpp>

using namespace Gui;

auto string = R"(
column:
  children:
    textarea:
      id: input
      font-size: 20
    column:
      id: output
)";

auto initial = R"(row:
  padding: 80
  color: 0x040D12
  children:
    label:
      text: SignIn Form
      color: 0xfed766
    input:
      hint: Enter your email here...
    input:
      hint: Enter your password here...
    button:
      text: Submit
      font-size: 30
      color: 0x040D12
      background: 0x93B1A6
      margin: 10
      height: 100
)";

class MyApplication : public Application {
public:
  MyApplication() : Application("LibGUI - Playground", 800, 800)
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

    auto output = getById("output");
    auto input = getById("input");
    input->as<TextArea>()->setOnChange([output](const auto& text) {
      YAML::Node node;
      try {
         node = YAML::Load(text);
      } catch (YAML::ParserException& e) {
        auto mark = e.mark;
        Logger::error("YAML:%d:%d: %s", mark.line, mark.column, e.msg.c_str());
        return;
      }

      std::vector<DeserializationError> errors;
      Widget::Handle widget = Widget::deserialize(node, errors);

      if (!errors.empty()) {
        for (auto& error : errors) {
          Logger::error("YAML:%d:%d: %s", error.line, error.column, error.message.c_str());
        }
        return;
      }

      output->as<Container>()->clearChildren();
      output->as<Container>()->addChild(widget);
    });
    focus(input);

    input->as<TextArea>()->setText(initial);
  }
};

int main() {
  MyApplication().run();
}
