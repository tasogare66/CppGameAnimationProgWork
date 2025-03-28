#include "Camera.h"
#include <glm/gtc/matrix_transform.hpp>

glm::mat4 Camera::getViewMatrix(OGLRenderData& renderData)
{
  float azimRad = glm::radians(renderData.rdViewAzimuth);
  float elevRad = glm::radians(renderData.rdViewElevation);

  float sinAzim = glm::sin(azimRad);
  float cosAzim = glm::cos(azimRad);
  float sinElev = glm::sin(elevRad);
  float cosElev = glm::cos(elevRad);

  /* update view direction */
  mViewDirection = glm::normalize(
    glm::vec3(sinAzim * cosElev, sinElev, -cosAzim * cosElev));

  /* calculate right and up direction */
  mRightDirection = glm::normalize(glm::cross(mViewDirection, mWorldUpVector));
  mUpDirection = glm::normalize(glm::cross(mRightDirection, mViewDirection));

  /* update camera position depending on desired movement */
  renderData.rdCameraWorldPosition +=
    renderData.rdMoveForward * renderData.rdTickDiff * mViewDirection
    + renderData.rdMoveRight * renderData.rdTickDiff * mRightDirection
    + renderData.rdMoveUp * renderData.rdTickDiff * mUpDirection;

  return glm::lookAt(renderData.rdCameraWorldPosition,
    renderData.rdCameraWorldPosition + mViewDirection, mWorldUpVector);
}
