#pragma once

#include "event/Event.h"

namespace Neptune
{

// WindowFocused, WindowLostFocus

class WindowClosedEvent : public Event
{
public:
  WindowClosedEvent() noexcept = default;

  String ToString() const override
  {
    return ""; // TODO: Formmatted Event Logging (We'll need our own stream to save a copy)
  }
  
  EVENT_TYPE(WindowClosed);
  EVENT_CATEGORY(EventCategoryApplication);
};

class WindowResizedEvent : public Event
{
public:
  WindowResizedEvent(UInt32 width, UInt32 height) noexcept
  	: m_Width(width), m_Height(height)
  {
  }
  
  UInt32 GetWidth() const noexcept
  {
    return m_Width;
  }
  
  UInt32 GetHeight() const noexcept
  {
    return m_Height;
  }
  
  String ToString() const override
  {
    return ""; // TODO: Formmatted Event Logging (We'll need our own stream to save a copy)
  }
  
  EVENT_TYPE(WindowResized);
  EVENT_CATEGORY(EventCategoryApplication);
  
private:
  UInt32 m_Width, m_Height;
};

class WindowMovedEvent : public Event
{
public:
  WindowMovedEvent(UInt32 xPos, UInt32 yPos) noexcept
  : m_XPos(xPos), m_YPos(yPos)
  {
  }
  
  UInt32 GetXPosition() const noexcept
  {
    return m_XPos;
  }
  
  UInt32 GetYPosition() const noexcept
  {
    return m_YPos;
  }
  
  String ToString() const override
  {
    return ""; // TODO: Formmatted Event Logging (We'll need our own stream to save a copy)
  }
  
  EVENT_TYPE(WindowMoved);
  EVENT_CATEGORY(EventCategoryApplication);
  
private:
  UInt32 m_XPos, m_YPos;
};

class WindowFocusedEvent : public Event
{
public:
  WindowFocusedEvent() noexcept = default;
  
  String ToString() const override
  {
    return ""; // TODO: Formmatted Event Logging (We'll need our own stream to save a copy)
  }
  
  EVENT_TYPE(WindowFocused);
  EVENT_CATEGORY(EventCategoryApplication);
};

class WindowLostFocusEvent : public Event
{
public:
  WindowLostFocusEvent() noexcept = default;
  
  String ToString() const override
  {
    return ""; // TODO: Formmatted Event Logging (We'll need our own stream to save a copy)
  }
  
  EVENT_TYPE(WindowLostFocus);
  EVENT_CATEGORY(EventCategoryApplication);
};
  
} // namespace neptune
