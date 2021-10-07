#pragma once

#include "utils/Types.h"
#include "utils/SmartPointer.h"
#include "utils/String.h"

namespace Neptune
{

struct WindowDesc
{
  UInt32 Width, Height;
  String Name;
  
  bool Resizeable = false;
  
  WindowDesc(UInt32 width = 1280, UInt32 height = 720, String name = "Neptune Application")
  	: Width(width), Height(height), Name(name)
  {
  }
};

class Window
{
public:
  static Scope<Window> Create(const WindowDesc& desc = WindowDesc());
  virtual ~Window();
  
  const WindowDesc& GetDesc() const noexcept
  {
    return m_Desc;
  }
  virtual void SetDesc(const WindowDesc& desc) = 0;
  
  virtual void Show() = 0;
  virtual void Hide() = 0;
  virtual void Close() = 0;
  
protected:
  Window(const WindowDesc& desc)
  	: m_Desc(desc)
  {
  }
  
protected:
  WindowDesc m_Desc;
};

} // namespace Neptune
