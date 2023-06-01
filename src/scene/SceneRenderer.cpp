#include "neptunepch.h"
#include "SceneRenderer.h"

#include "Components.h"

#include "renderer/Renderer2D.h"

namespace Neptune
{

Scene* SceneRenderer::s_Scene = nullptr;

void SceneRenderer::OnInit(Scene* scene)
{
  s_Scene = scene;
}

void SceneRenderer::OnTerminate()
{
  
}

void SceneRenderer::ChangeScene(Scene *scene)
{
  s_Scene = scene;
}

void SceneRenderer::Render(const Camera& camera)
{
  // First, we'd normally handle 3d rendering
  // Next, we'll handle 2d renderering, which is all we're doing for now
  auto view = s_Scene->GetView<TransformComponent, SpriteRendererComponent>();
  
  Renderer2D::Begin(camera);
  for (auto entity : view)
  {
    auto& transform = view.get<TransformComponent>(entity);
    auto& sprite = view.get<SpriteRendererComponent>(entity);
    
    if (sprite.Texture)
    	Renderer2D::DrawQuad(transform.Matrix, sprite.Texture, sprite.Color, sprite.TilingFactor);
    else
      Renderer2D::DrawQuad(transform.Matrix, sprite.Color);
  }
  Renderer2D::End();
}

void SceneRenderer::RenderRuntime()
{
  // Find the active camera within the scene (we'll assume there is only one)
  Camera cam;
  auto view = s_Scene->GetView<CameraComponent>();
  
  for (auto entity : view)
  {
    auto& camera = view.get<CameraComponent>(entity);
    if (camera.MainCamera)
    {
      cam = camera.Camera;
      break;
    }
  }
  
  SceneRenderer::Render(cam);
}


} // namespace Neptune
