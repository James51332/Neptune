#pragma once

namespace Neptune
{

enum EventCategory
{
  Input = 1 << 0,
  Keyboard = 1 << 1,
  Mouse = 1 << 2
};

enum class EventType
{
	MousePress, MouseRelease, MouseMove, MouseScroll,
  KeyPress, KeyRelease, KeyRepeat
};

#define EVENT_CATEGORY(x) \
virtual EventCategory GetCategory() const override { return x; }

#define EVENT_TYPE(x) \
virtual EventType GetType() const override { return EventType::#x; }
  
class Event
{
public:
  virtual ~Event() noexcept = default;
  
  virtual EventCategory GetCategory() const = 0;
  virtual EventType GetType() const = 0;
  
  virtual String ToString() const
  {
    return "";
  }
  
private:
};

} // namespace Neptune
