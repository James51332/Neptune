#pragma once

namespace Neptune
{

enum EventCategory
{
  EventCategoryInput = 1 << 0,
  EventCategoryKeyboard = 1 << 1,
  EventCategoryMouse = 1 << 2
};

enum class EventType
{
	MousePressed, MouseReleased, MouseMoved, MouseScrolled,
  KeyPressed, KeyReleased, KeyTyped
};

#define EVENT_CATEGORY(x) \
virtual int GetCategoryFlags() const override { return x; }

#define EVENT_TYPE(x) \
virtual EventType GetType() const override { return EventType::x; } \
static EventType GetStaticType() { return EventType::x; }
  
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
