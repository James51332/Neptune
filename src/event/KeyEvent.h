#pragma once

#include "core/KeyCode.h"

#include "event/Event.h"

namespace Neptune
{

// represents the translated scancode for physical key
// locations. Neptune KeyCode's are named by QWERTY keys
// at these locations. These are useful for WASD configurations
// regardless of keyboard layout such as DVORAK or AZERTY.
class KeyPressedEvent : public Event
{
public:
  KeyPressedEvent(KeyCode keyCode) noexcept
  	: m_KeyCode(keyCode) {}
  
  KeyCode GetKeyCode() const noexcept
  {
    return m_KeyCode;
  }
  
  String ToString() const override
  {
    return ""; // TODO: Formmatted Event Logging (We'll need our own stream to save a copy)
  }

  EVENT_TYPE(KeyPressed);
  EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard);
  
private:
  KeyCode m_KeyCode;
};

class KeyReleasedEvent : public Event
{
public:
  KeyReleasedEvent(KeyCode keyCode) noexcept
  	: m_KeyCode(keyCode) {}
  
  KeyCode GetKeyCode() const noexcept
  {
    return m_KeyCode;
  }
  
  String ToString() const override
  {
    return "";
  }
  
  EVENT_TYPE(KeyReleased);
  EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard);
  
private:
  KeyCode m_KeyCode;
};

// represents the unicode key code for the characters typed
class KeyTypedEvent : public Event
{
public:
  KeyTypedEvent(UInt32 keyCode) noexcept
  	: m_KeyCode(keyCode) {}
  
  UInt32 GetKeyCode() const noexcept
  {
    return m_KeyCode;
  }
  
  String ToString() const override
  {
    return "";
  }
  
  EVENT_TYPE(KeyTyped);
  EVENT_CATEGORY(EventCategoryInput | EventCategoryKeyboard);
  
private:
  UInt32 m_KeyCode;
};

} // namespace Neptune
