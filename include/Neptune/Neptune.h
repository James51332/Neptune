#pragma once

// ----- Entry Point ----------
#include "core/EntryPoint.h"

// ----- Core Engine ----------
#include "core/PlatformDetector.h"
#include "core/Intrinsics.h"
#include "core/Application.h"
#include "core/Logger.h"
#include "core/KeyCode.h"
#include "core/MouseCode.h"
#include "core/Event.h"
#include "core/EventQueue.h"
#include "core/MouseEvent.h"
#include "core/KeyEvent.h"
#include "core/WindowEvent.h"
#include "core/Input.h"
#include "core/Timestep.h"
#include "core/Profile.h"

// ----- Renderer ----------
#include "renderer/Renderer.h"
#include "renderer/Renderer2D.h"
#include "renderer/Camera.h"
#include "renderer/CameraController.h"
#include "renderer/Buffer.h"
#include "renderer/Shader.h"
#include "renderer/PipelineState.h"
#include "renderer/RenderCommand.h"
#include "renderer/ImGuiRenderer.h"
#include "renderer/Sync.h"
#include "renderer/RenderDevice.h"
#include "renderer/ImGuiRenderer.h"
#include "renderer/Mesh.h"
#include "renderer/Material.h"
#include "renderer/Model.h"

// ----- Math ----------
#include "math/Vector.h"
#include "math/Matrix.h"

// ----- Events ----------
#include "core/Event.h"
#include "core/KeyEvent.h"
#include "core/MouseEvent.h"
#include "core/WindowEvent.h"
#include "core/EventQueue.h"

// ----- Scene ------------
#include "scene/Scene.h"
#include "scene/Entity.h"
#include "scene/Components.h"
#include "scene/SceneRenderer.h"
#include "scene/SceneManager.h"
#include "scene/SceneSerializer.h"

// ----- Utilities ------------
#include "utils/Utils.h"
#include "utils/Types.h"
#include "utils/DynamicArray.h"
#include "utils/SmartPointer.h"
#include "utils/Function.h"
#include "utils/HashMap.h"
#include "utils/String.h"
#include "utils/Lock.h"
#include "utils/Thread.h"
#include "utils/Queue.h"
