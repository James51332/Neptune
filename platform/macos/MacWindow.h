#pragma once

#include "core/Window.h"

namespace Neptune
{

class MacWindow : public Window
{
public:
  MacWindow(const WindowDesc& desc);
  ~MacWindow();

  void SetDesc(const WindowDesc& desc);
  void SetContext(const Ref<RenderContext>& context);
  
  void Show();
  void Hide();
  void Close();
  
private:
  void* m_Window;
  void* m_WindowDelegate;
  void* m_View;
};

} // namespace Neptune
