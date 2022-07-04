#include "neptunepch.h"

#include "EditorLayer.h"

#include <Neptune/Neptune.h>

namespace Neptune
{

class EditorApp : public Application
{
public:
  EditorApp()
  	: Application({1280, 720, "Neptune Editor", true})
  {
  }
  
  ~EditorApp()
  {
  }
};

Application* CreateApplication()
{
  Application* app = new EditorApp();
  app->PushLayer(new EditorLayer());
  return app;
}

} // namespace Neptune
