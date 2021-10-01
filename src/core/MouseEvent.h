#pragma once

#include "core/Event.h"
#include "core/MouseCode.h"

namespace Neptune
{

class MouseButtonEvent : public Event
{
public:
  MouseCode GetMouseCode() const noexcept
  {
    return m_MouseCode;
  }
  
protected:
  MouseButtonEvent(MouseCode mouseCode)
  	: m_MouseCode(mouseCode) {}
  
private:
  MouseCode m_MouseCode;
};

class MousePressedEvent : public MouseButtonEvent
{
public:
  MousePressedEvent(MouseCode mouseCode) noexcept
  	: MouseButtonEvent(mouseCode) {}
  
  virtual String ToString() const override
  {
    return "";
  }
  
  EVENT_TYPE(MousePressed);
  EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse);
};

class MouseReleasedEvent : public MouseButtonEvent
{
public:
  MouseReleasedEvent(MouseCode mouseCode) noexcept
  : MouseButtonEvent(mouseCode) {}
  
  virtual String ToString() const override
  {
    return "";
  }
  
  EVENT_TYPE(MouseReleased);
  EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse);
};

class MouseMovedEvent : public Event
{
public:
  MouseMovedEvent(UInt32 xPos, UInt32 yPos) noexcept
  : m_XPos(xPos), m_YPos(yPos) {}
  
  UInt32 GetXPosition() const noexcept
  {
    return m_XPos;
  }
  
  UInt32 GetYPosition() const noexcept
  {
    return m_YPos;
  }
  
  virtual String ToString() const override
  {
    return "";
  }
  
  EVENT_TYPE(MouseMoved);
  EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse);
  
private:
  UInt32 m_XPos, m_YPos;
};

class MouseScrolledEvent : public Event
{
public:
  MouseScrolledEvent(UInt32 xScroll, UInt32 yScroll)
  	: m_XScroll(xScroll), m_YScroll(yScroll) {}
  
  UInt32 GetXScroll() const noexcept
  {
    return m_XScroll;
  }
  
  UInt32 GetYScroll() const noexcept
  {
    return m_YScroll;
  }
  
  virtual String ToString() const override
  {
    return "";
  }
  
  EVENT_TYPE(MouseScrolled);
  EVENT_CATEGORY(EventCategoryInput | EventCategoryMouse);
  
private:
  UInt32 m_XScroll, m_YScroll;
};

} // namespace Neptune
