#include "neptunepch.h"
#include "Input.h"

#include "EventQueue.h"

namespace Neptune
{

bool Input::s_Keys[Input::s_NumKeys];
bool Input::s_LastKeys[Input::s_NumKeys];

bool Input::s_MouseButtons[Input::s_NumMouseButtons];
bool Input::s_LastMouseButtons[Input::s_NumMouseButtons];

Float32 Input::s_MouseX, Input::s_LastMouseX;
Float32 Input::s_MouseY, Input::s_LastMouseY;
Int32 Input::s_ScrollDeltaX, Input::s_ScrollDeltaY;

void Input::OnInit()
{
  for (Size i = 0; i < s_NumKeys; ++i)
  {
    s_Keys[i] = false;
    s_LastKeys[i] = false;
  }
  
  for (Size i = 0; i < s_NumMouseButtons; ++i)
  {
    s_MouseButtons[i] = false;
    s_LastMouseButtons[i] = false;
  }
  
  s_MouseX = 0;
  s_LastMouseX = 0;
  s_MouseY = 0;
  s_LastMouseY = 0;
  s_ScrollDeltaX = 0;
  s_ScrollDeltaY = 0;
}

void Input::OnUpdate()
{
  std::memcpy(&s_LastKeys[0], &s_Keys[0], sizeof(s_Keys));
  std::memcpy(&s_LastMouseButtons[0], &s_MouseButtons[0], sizeof(s_MouseButtons));
  
  s_LastMouseX = s_MouseX;
  s_LastMouseY = s_MouseY;
  
  s_ScrollDeltaX = 0;
  s_ScrollDeltaY = 0;
}

void Input::OnTerminate()
{
  // We don't really need to delete anything.
  // This function is just here for API consistency
}

void Input::OnEvent(Scope<Event> &event)
{
  EventQueue::Dispatch<KeyPressedEvent>(event, [](const KeyPressedEvent& event)
  {
    s_Keys[event.GetKeyCode()] = true;
    return false;
  });
  
  EventQueue::Dispatch<KeyReleasedEvent>(event, [](const KeyReleasedEvent& event)
  {
    s_Keys[event.GetKeyCode()] = false;
    return false;
  });
  
  EventQueue::Dispatch<MousePressedEvent>(event, [](const MousePressedEvent& event)
  {
    s_MouseButtons[event.GetMouseCode()] = true;
    return false;
  });
  
  EventQueue::Dispatch<MouseReleasedEvent>(event, [](const MouseReleasedEvent& event)
  {
    s_MouseButtons[event.GetMouseCode()] = false;
    return false;
  });
  
  EventQueue::Dispatch<MouseMovedEvent>(event, [](const MouseMovedEvent& event)
  {
    s_MouseX = event.GetXPosition();
    s_MouseY = event.GetYPosition();
    return false;
  });
  
  EventQueue::Dispatch<MouseScrolledEvent>(event, [](const MouseScrolledEvent& event)
  {
    s_ScrollDeltaY = event.GetYScroll();
    s_ScrollDeltaX = event.GetXScroll();
    return false;
  });
}

bool Input::KeyDown(KeyCode key) noexcept
{
  NEPTUNE_ASSERT(key >= 0 && key < s_NumKeys, "Unknown keycode!");
  return s_Keys[key];
}

bool Input::KeyPress(KeyCode key) noexcept
{
  NEPTUNE_ASSERT(key >= 0 && key < s_NumKeys, "Unknown keycode!");
  return s_Keys[key] && !s_LastKeys[key];
}

bool Input::KeyRelease(KeyCode key) noexcept
{
  NEPTUNE_ASSERT(key >= 0 && key < s_NumKeys, "Unknown keycode!");
  return !s_Keys[key] && s_LastKeys[key];
}

bool Input::MouseDown(MouseCode btn) noexcept
{
  NEPTUNE_ASSERT(btn >= 0, "Unknown mousecode!");
  return s_MouseButtons[btn];
}

bool Input::MousePress(MouseCode btn) noexcept
{
  NEPTUNE_ASSERT(btn >= 0, "Unknown mousecode!");
  return s_MouseButtons[btn] && !s_LastMouseButtons[btn];
}

bool Input::MouseRelease(MouseCode btn) noexcept
{
  NEPTUNE_ASSERT(btn >= 0, "Unknown mousecode!");
  return !s_MouseButtons[btn] && s_LastMouseButtons[btn];
}

bool Input::MouseMoved() noexcept
{
  return (s_MouseX != s_LastMouseX) || (s_MouseY != s_LastMouseY);
}

Float32 Input::GetMouseX() noexcept
{
  return s_MouseX;
}

Float32 Input::GetMouseY() noexcept
{
  return s_MouseY;
}

Float32 Input::GetMouseDeltaX() noexcept
{
  return s_MouseX - s_LastMouseX;
}

Float32 Input::GetMouseDeltaY() noexcept
{
  return s_MouseY - s_LastMouseY;
}

bool Input::MouseScrolled() noexcept
{
  return (s_ScrollDeltaX != 0) || (s_ScrollDeltaY != 0);
}

Int32 Input::MouseScrollDeltaX() noexcept
{
  return s_ScrollDeltaX;
}

Int32 Input::MouseScrollDeltaY() noexcept
{
  return s_ScrollDeltaY;
}

} // namespace Neptune
