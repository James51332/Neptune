#include "neptunepch.h"
#include "SceneRenderer.h"

#include "Components.h"
#include "SceneManager.h"

#include "renderer/Renderer2D.h"

namespace Neptune
{

Ref<Scene> SceneRenderer::s_Scene = nullptr;
Float32 SceneRenderer::s_AspectRatio = 1.0f;

void SceneRenderer::OnInit(Ref<Scene> scene)
{
  s_Scene = scene;
}

void SceneRenderer::OnTerminate()
{
  
}

void SceneRenderer::OnResize(Float32 aspect)
{
  s_AspectRatio = aspect;
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
  bool found = false;
  
  // Find active camera in scene (assuming there is one-editor must enforce)
  auto view = s_Scene->GetView<CameraComponent, TransformComponent>();
  for (auto entity : view)
  {
    auto& camera = view.get<CameraComponent>(entity);
    if (camera.MainCamera)
    {
      // Set our camera.
      cam = camera.Camera;
      found = true;
      
      // Now that we've found our camera, trasnform it to the camera's transform.
      // TODO: We shouldn't recalculate our matrices when they aren't updated.
      auto& transform = view.get<TransformComponent>(entity);
      CameraDesc desc = cam.GetDesc();
      desc.Position = transform.Position;
      desc.Rotation = transform.Rotation;
      desc.Aspect = s_AspectRatio;
      cam.SetDesc(desc);
      
      break;
    }
  }
  
  NEPTUNE_ASSERT(found, "Cannot render in runtime without a main camera!");
  Render(cam);
}

void SceneRenderer::Render(const Camera& camera)
{
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
