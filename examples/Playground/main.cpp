#include <Gui.hpp>

using namespace Gui;

auto string = R"(
column:
  children:
    row:
      children:
        textarea:
          id: input
          font-size: 20
          background: 0x292929
          color: 0xdfdfdf
        row:
          id: log-section
          height: 350
          display: false    # Only display when there is an error/warning
          color: 0x292929
          children:
            label:
              text: "Errors:"
              color: 0xff6347
              margin: 5
            textarea:
              id: errors
              font-size: 18
              background: 0x292929
              color: 0xdfdfdf
              editable: false
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
      margin: 10
    input:
      hint: Enter your email here...
    input:
      hint: Enter your password here...
    column:
      main-axis: start
      padding: 10
      height: 60
      children:
        checkbox:
        label:
          text: "Tearms & Conditions"
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

    auto logSection = getById("log-section");
    auto errorsText = getById("errors");
    input->as<TextArea>()->setOnChange([output, errorsText, logSection](const auto& text) {
      Widget::Handle widget = nullptr;
      try {
        auto node = YAML::Load(text);

        std::vector<DeserializationError> errors;
        widget = Widget::deserialize(node, errors);

        if (!errors.empty()) {
          std::string errorString = "\n";
          for (auto& error : errors) {
            errorString += "  " + std::to_string(error.line) + ":" + std::to_string(error.column) + ": " + error.message + "\n";
            Logger::error("YAML:%d:%d: %s", error.line, error.column, error.message.c_str());
          }
          errorsText->as<TextArea>()->setText(errorString);
          logSection->setDisplay(true);
          return;
        }
        errorsText->as<TextArea>()->setText("");
        logSection->setDisplay(false);
      } catch (YAML::ParserException& e) {
        auto mark = e.mark;
        Logger::error("YAML:%d:%d: %s", mark.line, mark.column, e.msg.c_str());
        errorsText->as<TextArea>()->setText(
          "\n  " + std::to_string(mark.line) + ":" + std::to_string(mark.line) + ": " + e.msg
        );
        logSection->setDisplay(true);
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
