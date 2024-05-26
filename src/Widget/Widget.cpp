#include "Core/Log.hpp"
#include "Widget/Widget.hpp"
#include "Widget/Container.hpp"
#include "Widget/Column.hpp"
#include "Widget/Row.hpp"
#include "Widget/Input.hpp"
#include "Widget/SizedBox.hpp"
#include "Widget/Label.hpp"
#include "Widget/Button.hpp"

#include <iostream>
#include <regex>

namespace Gui {

void insertDeserializationError(std::vector<DeserializationError>& errors, YAML::Mark mark, std::string message) {
  errors.push_back(DeserializationError {
    mark.line + 1,
    mark.column + 1,
    std::move(message),
  });
}

static std::regex parseHexColorPattern("0x([0-9a-fA-F]{6})");

// https://stackoverflow.com/questions/70143856/how-to-convert-hex-color-string-to-rgb-using-regex-in-c
static bool parseHexColor(const std::string& value, Vec3& result) {
  std::smatch match;
  if (std::regex_match(value, match, parseHexColorPattern))
  {
    int r, g, b;
    sscanf(match.str(1).c_str(), "%2x%2x%2x", &r, &g, &b);
    std::cout << "R: " << r << ", G: " << g << ", B: " << b << "\n";
    result = {r/255.0f, g/255.0f, b/255.0f};
    return true;
  }

  return false;
}

Vec4 deserializeColor(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  Vec3 color = Color::WHITE;
  if (node && node.IsScalar()) {
    auto value = node.as<std::string>();
    if (value == "red") {
      color = Color::RED;
    } else if (value == "blue") {
      color = Color::BLUE;
    } else if (value == "green") {
      color = Color::GREEN;
    } else if (value == "purple") {
      color = Color::PURPLE;
    } else if (parseHexColor(value, color)) {
    } else {
      insertDeserializationError(errors, node.Mark(), "unknown color: " + value);
    }
  }
  return Vec4{color, 1.0f};
}

std::string deserializeId(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  std::string id;
  if (node && node.IsScalar()) {
    id = node.as<std::string>();
  }
  return id;
}

std::vector<Widget::Handle> deserializeChildren(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  std::vector<Widget::Handle> children;
  if(node) {
    if (node.IsSequence()) {
      for (const auto& child : node) {
        if (auto widget = Widget::deserialize(child, errors)) {
          children.push_back(widget);
        }
      }
    } else {
      insertDeserializationError(errors, node.Mark(), "Expected children field to be a sequence!");
    }
  }

  return children;
}

Vec4 deserializePadding(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  float padding = 0.0f;
  if (node && node.IsScalar()) {
    padding = node.as<float>();
  }

  // TODO: Multicomponent padding
  return Vec4{padding};
}

void Widget::reportSize() const {
  std::cout
    << "Size: x="
    << mPosition.x
    << ", y="
    << mPosition.y
    << ", width="
    << mSize.x
    << ", height="
    << mSize.y
    << std::endl;
}

bool compareNodeWithStaticString(const YAML::Node& node, const char* other) {
    if (node.IsScalar()) {
        return node.as<std::string>() == other;
    }
    return false;
}

Widget::Handle Widget::deserialize(const YAML::Node& node, std::vector<DeserializationError>& errors) {
  if (!node.IsMap()) {
    insertDeserializationError(errors, node.Mark(), "Widget name is not a map");
    return nullptr;
  }

  if (node.size() != 1) {
    insertDeserializationError(errors, node.Mark(), "Widget map contains more that one name key");
    return nullptr;
  }

  std::optional<YAML::detail::iterator_value> pair;
  for (const auto& kv : node) {
    pair = kv;
    break;
  }

  if (compareNodeWithStaticString(pair->first, "row")) {
    return Row::deserialize(pair->second, errors);
  } else if (compareNodeWithStaticString(pair->first, "column")) {
    return Column::deserialize(pair->second, errors);
  } else if (compareNodeWithStaticString(pair->first, "container")) {
    return Container::deserialize(pair->second, errors);
  } else if (compareNodeWithStaticString(pair->first, "input")) {
    return Input::deserialize(pair->second, errors);
  } else if (compareNodeWithStaticString(pair->first, "sized-box")) {
    return SizedBox::deserialize(pair->second, errors);
  } else if (compareNodeWithStaticString(pair->first, "label")) {
    return Label::deserialize(pair->second, errors);
  } else if (compareNodeWithStaticString(pair->first, "button")) {
    return Button::deserialize(pair->second, errors);
  } else {
    insertDeserializationError(errors, pair->second.Mark(), "unknown Widget type");
  }

  return nullptr;
}

} // namespace Gui
