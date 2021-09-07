#pragma once

#include "utils/Types.h"

namespace Neptune {

// We'll need to handle small string optimization
// in the future but it shouldn't be that hard
class String
{
public:
  String() noexcept;
  String(const char*) noexcept;
  String(const String&);
  String(String&&) noexcept;
  
  String& operator=(String);
  ~String();
  
  char& operator[](Neptune::Size index) noexcept;
  char operator[](Neptune::Size index) const noexcept;
  
  const char* Raw() const noexcept;
  
private:
  char* m_Data; // m_Data is one byte bigger than m_Size to account for null terminator
  Size m_Size;
};



} // namespace Neptune
