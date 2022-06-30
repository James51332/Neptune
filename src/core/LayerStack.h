#pragma once

#include "Layer.h"

namespace Neptune
{

// ----- LayerStack -----------------

class LayerStack
{
public:
  LayerStack();
  ~LayerStack();
  
  // There doesn't really need to be an API to remove layers
  void PushLayer(Layer* layer);
  
  DynamicArray<Layer*>::Iterator begin() { return m_Layers.begin(); }
  DynamicArray<Layer*>::Iterator end() { return m_Layers.end(); }
  DynamicArray<Layer*>::ConstIterator begin() const { return m_Layers.begin(); }
  DynamicArray<Layer*>::ConstIterator end() const { return m_Layers.end(); }
  DynamicArray<Layer*>::Iterator rbegin() { return m_Layers.rbegin(); }
  DynamicArray<Layer*>::Iterator rend() { return m_Layers.rend(); }
  DynamicArray<Layer*>::ConstIterator rbegin() const { return m_Layers.rbegin(); }
  DynamicArray<Layer*>::ConstIterator rend() const { return m_Layers.rend(); }
private:
  DynamicArray<Layer*> m_Layers;
};

} // namespace Neptune
