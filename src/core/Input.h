#pragma once

#include "KeyCode.h"
#include "MouseCode.h"
#include "KeyEvent.h"
#include "MouseEvent.h"

namespace Neptune
{

// ----- Input -----------------

class Input
{
public:
  static void OnInit();
  static void OnUpdate();
  static void OnTerminate();
  static void OnEvent(Scope<Event>& event);
  
  static bool KeyDown(KeyCode key) noexcept;
  static bool KeyPress(KeyCode key) noexcept;
  static bool KeyRelease(KeyCode key) noexcept;
  static bool MouseDown(MouseCode btn) noexcept;
  static bool MousePress(MouseCode btn) noexcept;
  static bool MouseRelease(MouseCode btn) noexcept;
  
  static bool MouseMoved() noexcept;
  static Float32 GetMouseX() noexcept;
  static Float32 GetMouseY() noexcept;
  static Float32 GetMouseDeltaX() noexcept;
  static Float32 GetMouseDeltaY() noexcept;
  
  static bool MouseScrolled() noexcept;
  static Int32 MouseScrollDeltaX() noexcept;
  static Int32 MouseScrollDeltaY() noexcept;
  
private:
  // Enough to index all ASCII codes
  // TODO: If performance of copying this each frame
  // becomes an issue, then we should use a bitset
  // instead of bools to reduce copying eightfold.
  constexpr static Int32 s_NumKeys = 350;
  constexpr static Int32 s_NumMouseButtons = 8;
  
  static bool s_Keys[s_NumKeys];
  static bool s_LastKeys[s_NumKeys];
  static bool s_MouseButtons[s_NumMouseButtons];
  static bool s_LastMouseButtons[s_NumMouseButtons];
  
  static Float32 s_MouseX, s_LastMouseX;
  static Float32 s_MouseY, s_LastMouseY;
  static Int32 s_ScrollDeltaX;
  static Int32 s_ScrollDeltaY;
};

} // namespace Neptune
