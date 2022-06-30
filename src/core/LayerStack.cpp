#include "neptunepch.h"
#include "LayerStack.h"

namespace Neptune
{

// ----- LayerStack -----------------

LayerStack::LayerStack()
{
  
}

LayerStack::~LayerStack()
{
  for (auto& layer : m_Layers)
    delete layer;
}

void LayerStack::PushLayer(Layer* layer)
{
  m_Layers.PushBack(layer);
}

} // namespace Neptune
