#include "Camera.h"
#include "CameraPrivate.h"

#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/matrix_float4x4.hpp>
#include <glm/ext/matrix_transform.hpp>
#include <glm/ext/vector_float3.hpp>
#include <glm/geometric.hpp>
#include <glm/mat4x4.hpp>

// ----------------------------------------------------------------------------

USING_CHIMIA_DRAW3D_NAMESPACE

// ----------------------------------------------------------------------------
// Utils
// ----------------------------------------------------------------------------

namespace CameraUtils {

glm::mat4x4
createIdentity()
{
  return glm::identity<glm::mat4x4>();
}

bool
isEqual(const float a, const float b, const float error)
{
  const float diff = std::abs(a - b);
  return diff < error;
}

glm::vec3
AdjustedUpDirection(const glm::vec3& up, const glm::vec3& target)
{
  const glm::vec3 direction = glm::normalize(target);
  const float dotUpWithDirection = glm::dot(up, direction);
  const bool upAndTargetAreInSameDirection =
    isEqual(dotUpWithDirection, 0.0f, 0.0001f);

  return upAndTargetAreInSameDirection ? glm::vec3{ 0.0f, 0.0f, 1.0f } : up;
}

}

// ----------------------------------------------------------------------------
// State
// ----------------------------------------------------------------------------

namespace CameraState {
glm::mat4x4 projection = CameraUtils::createIdentity();
glm::mat4x4 view = CameraUtils::createIdentity();

glm::vec3 cameraPosition{ 0.0f, 0.0f, 0.0f };
}

// ----------------------------------------------------------------------------
// Camera
// ----------------------------------------------------------------------------

void
Camera::Projection::SetIdentity()
{
  CameraState::projection = CameraUtils::createIdentity();
}

// ----------------------------------------------------------------------------

void
Camera::Projection::SetPerspective(const float fieldOfView,
                                   const float aspect,
                                   const float nearClippingPlane,
                                   const float farClippingPlane)
{
  CameraState::projection =
    glm::perspective(fieldOfView, aspect, nearClippingPlane, farClippingPlane);
}

// ----------------------------------------------------------------------------

void
Camera::Projection::SetOthographic(const float left,
                                   const float right,
                                   const float bottom,
                                   const float top,
                                   const float near,
                                   const float far)
{
  CameraState::projection = glm::ortho(left, right, bottom, top, near, far);
}

// ----------------------------------------------------------------------------

void
Camera::View::LookAt(const glm::vec3& eye, const glm::vec3& target)
{
  const glm::vec3 defaultUp{ 0.0f, 1.0f, 0.0f };
  const glm::vec3 up = CameraUtils::AdjustedUpDirection(defaultUp, target);

  CameraState::view = glm::lookAt(eye, target, up);
  CameraState::cameraPosition = eye;
}

// ----------------------------------------------------------------------------
// CameraPrivate
// ----------------------------------------------------------------------------

void
CameraPrivate::SetCameraOnShader(Rendering::Shader& shader)
{
  const glm::mat4x4 transform = CameraState::projection * CameraState::view;
  shader.SetUniform("cameraTransform", transform);
}

// ----------------------------------------------------------------------------

void
CameraPrivate::SetPointOfViewOnShader(Rendering::Shader& shader)
{
  shader.SetUniform("viewPosition", CameraState::cameraPosition);
}

// ----------------------------------------------------------------------------