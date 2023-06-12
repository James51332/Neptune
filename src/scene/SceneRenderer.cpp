#include "neptunepch.h"
#include "SceneRenderer.h"

#include "Components.h"
#include "SceneManager.h"

#include "renderer/Renderer2D.h"

namespace Neptune
{

Ref<Scene> SceneRenderer::s_Scene = nullptr;
Float32 SceneRenderer::s_AspectRatio = 1.0f;

Entity SceneRenderer::s_RuntimeCamera;

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

void SceneRenderer::Render(const Camera& camera)
{
  // First, we'd normally handle 3d rendering
  // Next, we'll handle 2d renderering, which is all we're doing for now
  Renderer2D::Begin(camera);
  
  auto view = s_Scene->GetView<TransformComponent, SpriteRendererComponent>();
  for (auto entity : view)
  {
    auto& transform = view.get<TransformComponent>(entity);
    auto& sprite = view.get<SpriteRendererComponent>(entity);
    
    // TODO: We should probably not have to do this each frame (but we'll profile it later).
    transform.CalculateTransformMatrix();
    
    if (sprite.Texture)
      Renderer2D::DrawQuad(transform.Matrix, sprite.Texture, sprite.Color, sprite.TilingFactor);
    else
      Renderer2D::DrawQuad(transform.Matrix, sprite.Color);
  }
  
  Renderer2D::End();
}

void SceneRenderer::RenderRuntime()
{
  NEPTUNE_ASSERT(s_RuntimeCamera, "Cannot render in runtime without a main camera!");

  // Get the components need to render in runtime
  auto& camera = s_RuntimeCamera.GetComponent<CameraComponent>();
  auto& transform = s_RuntimeCamera.GetComponent<TransformComponent>();
  
  // Move the camera according to its transform component.
  // TODO: We shouldn't recalculate our matrices when they aren't updated.
  CameraDesc desc = camera.Camera.GetDesc();
  desc.Position = transform.Position;
  desc.Rotation = transform.Rotation;
  desc.Aspect = s_AspectRatio;
  camera.Camera.SetDesc(desc);
    
  // Render using our camera
  Render(camera.Camera);
}

} // namespace Neptune
