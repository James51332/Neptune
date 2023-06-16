#include "neptunepch.h"
#include "EventQueue.h"

namespace Neptune
{

EventQueue::EventQueue()
{
}

void EventQueue::PushEvent(Scope<Event> event)
{
  m_EventQueue.enqueue(Neptune::Move(event));
}

bool EventQueue::PopEvent(Scope<Event>& e)
{
  return m_EventQueue.try_dequeue(e);
}

} // namespace Neptune
