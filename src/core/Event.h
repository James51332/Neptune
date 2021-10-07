#pragma once

namespace Neptune
{

enum EventCategory
{
  EventCategoryApplication = 1 << 0,
  EventCategoryInput = 1 << 1,
  EventCategoryKeyboard = 1 << 2,
  EventCategoryMouse = 1 << 3
};

enum class EventType
{
	MousePressed, MouseReleased, MouseMoved, MouseScrolled,
  KeyPressed, KeyReleased, KeyTyped,
  WindowClosed, WindowResized, WindowMoved, WindowFocused, WindowLostFocus
};

#define EVENT_CATEGORY(x) \
virtual int GetCategoryFlags() const noexcept override { return x; }

#define EVENT_TYPE(x) \
virtual EventType GetType() const noexcept override { return EventType::x; } \
static EventType GetStaticType() noexcept { return EventType::x; }
  
class Event
{
public:
  bool Handled = false;
  
public:
  virtual ~Event() noexcept = default;
  
  virtual int GetCategoryFlags() const = 0;
  virtual EventType GetType() const = 0;
  
  virtual String ToString() const
  {
    return "";
  }
};

} // namespace Neptune
