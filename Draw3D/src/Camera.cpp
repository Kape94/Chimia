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

namespace {

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

glm::mat4x4 projection = createIdentity();
glm::mat4x4 view = createIdentity();

}

// ----------------------------------------------------------------------------

void
Camera::Projection::SetIdentity()
{
  projection = createIdentity();
}

// ----------------------------------------------------------------------------

void
Camera::Projection::SetPerspective(const float fieldOfView,
                                   const float aspect,
                                   const float nearClippingPlane,
                                   const float farClippingPlane)
{
  projection =
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
  projection = glm::ortho(left, right, bottom, top, near, far);
}

// ----------------------------------------------------------------------------

void
Camera::View::LookAt(const glm::vec3& eye, const glm::vec3& target)
{
  glm::vec3 up{ 0.0f, 1.0f, 0.0f };

  const glm::vec3 direction = glm::normalize(target);
  const float dotUpWithDirection = glm::dot(up, direction);
  const bool sameDirection = isEqual(dotUpWithDirection, 0.0f, 0.0001f);
  if (glm::dot(up, direction)) {
    up = { 0.0f, 0.0f, 1.0f };
  }

  view = glm::lookAt(eye, target, up);
}

// ----------------------------------------------------------------------------

glm::mat4x4
CameraPrivate::GetCurrentTransform()
{
  return projection * view;
}

// ----------------------------------------------------------------------------