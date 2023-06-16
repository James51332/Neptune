#pragma once

#include "event/Event.h"
#include "event/KeyEvent.h"
#include "event/MouseEvent.h"

#include "utils/Queue.h"
#include "utils/SmartPointer.h"

namespace Neptune
{

class EventQueue
{
public:
  EventQueue();
  
  void PushEvent(Scope<Event> event);
  bool PopEvent(Scope<Event>& event);
  
  template <typename T, typename F>
  static void Dispatch(Scope<Event>& event, const F& func)
  {
    if (!event->Handled && T::GetStaticType() == event->GetType())
    {
      event->Handled = func(*static_cast<T*>(event.Raw()));
    }
  }
  
  template <typename T, typename F>
  bool Dispatch(const F& func)
  {
    Scope<Event> e;
    if (!PopEvent(e))
      return false;

    Dispatch<T, F>(e, func);
    return true;
  }
  
private:
  Queue<Scope<Event>> m_EventQueue;
};

} // namespace Neptune
