#include "neptunepch.h"
#include "SceneRenderer.h"

#include "Components.h"
#include "SceneManager.h"

#include "renderer/Renderer2D.h"

namespace Neptune
{

Ref<Scene> SceneRenderer::s_Scene = nullptr;

void SceneRenderer::OnInit(Ref<Scene> scene)
{
  s_Scene = scene;
}

void SceneRenderer::OnTerminate()
{
  
}

void SceneRenderer::ChangeScene(Ref<Scene> scene)
{
  s_Scene = scene;
}

void SceneRenderer::RenderEditor(const Camera &camera)
{
  // Determine whether to use editor or scene camera.
  if (SceneManager::GetRuntime())
    RenderRuntime();
  else
    Render(camera);
}

void SceneRenderer::RenderRuntime()
{
  Camera cam;

  // Find active camera in scene (assuming there is one-editor must enforce)
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
  
  Render(cam);
}

void SceneRenderer::Render(const Camera& camera)
{
  // Now that we've found the camera, handle the rendering.
  {
    // First, we'd normally handle 3d rendering
    // Next, we'll handle 2d renderering, which is all we're doing for now
    auto view = s_Scene->GetView<TransformComponent, SpriteRendererComponent>();
    
    Renderer2D::Begin(camera);
    for (auto entity : view)
    {
      auto& transform = view.get<TransformComponent>(entity);
      auto& sprite = view.get<SpriteRendererComponent>(entity);
      
      // TODO: We should probably not have to do this each frame.
      transform.CalculateTransformMatrix();
      
      if (sprite.Texture)
        Renderer2D::DrawQuad(transform.Matrix, sprite.Texture, sprite.Color, sprite.TilingFactor);
      else
        Renderer2D::DrawQuad(transform.Matrix, sprite.Color);
    }
    Renderer2D::End();
  }
}

} // namespace Neptune
