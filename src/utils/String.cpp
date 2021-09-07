#include "neptunepch.h"
#include "String.h"

#include "utils/Utils.h"

#include <cstring>

namespace Neptune
{

String::String() noexcept
	: m_Size(0), m_Data(nullptr)
{
}

String::String(const char* data) noexcept
	: m_Size(0), m_Data(nullptr)
{

  m_Size = std::strlen(data);
  
  try
  {
    m_Data = new char[m_Size + 1];
  } catch (...)
  {
    m_Size = 0; // if this fails we'll reset size for strong guarantee
    throw;
  }
  
  std::memcpy(m_Data, data, m_Size);
  m_Data[m_Size] = 0;
}

String::String(const String& other)
	: m_Size(0), m_Data(nullptr)
{
  m_Data = new char[other.m_Size + 1]; // if this throws, we hold strong guarantee
  std::memcpy(m_Data, other.m_Data, other.m_Size);
  m_Data[other.m_Size] = 0;
 
  m_Size = other.m_Size;
}

String::String(String&& other) noexcept
	: m_Size(0), m_Data(nullptr)
{
  Swap(m_Size, other.m_Size);
  Swap(m_Data, other.m_Data);
}
  
String& String::operator=(String other)
{
  Swap(m_Size, other.m_Size);
  Swap(m_Data, other.m_Data);
  
  return (*this);
}

String::~String()
{
  delete[] m_Data;
  
  m_Data = nullptr;
  m_Size = 0;
}
  
char& String::operator[](Neptune::Size index) noexcept
{
  return m_Data[index];
}

char String::operator[](Neptune::Size index) const noexcept
{
  return m_Data[index];
}
  
const char* String::Raw() const noexcept
{
  return m_Data;
}

} // namespace Neptune
