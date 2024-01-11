#include <algorithm>

#include "Renderer/CameraController.hpp"

namespace Gui {

  OrthographicCameraController::OrthographicCameraController(u32 width, u32 height, f32 aspectRatio, f32 zNear, f32 zFar)
    : mAspectRatio{aspectRatio},
    mCamera(0.0f, width, height, 0.0f, zNear, zFar)
  {}

  void OrthographicCameraController::resize(u32 width, u32 height) {
    mAspectRatio = (f32)width / (f32)height;
    mCamera.setProjection(0.0f, width, height, 0.0f);
  }

  void OrthographicCameraController::setZoomLevel(const f32 zoomLevel) {
		mZoomLevel = std::clamp(zoomLevel, mZoomLevelMin, mZoomLevelMax);
		mCamera.setProjection(-mAspectRatio * mZoomLevel, mAspectRatio * mZoomLevel, -mZoomLevel, mZoomLevel);
  }

  void OrthographicCameraController::setRotation(const f32 rotation) {
    mCameraRotation = rotation;
    mCamera.setRotation(mCameraRotation);
  }

  void OrthographicCameraController::offsetPosition(const Vec3 offset) {
    mCameraPosition += offset;
    mCamera.setPosition(mCameraPosition);
  }

  void OrthographicCameraController::setPosition(const Vec3 position) {
    mCameraPosition = position;
    mCamera.setPosition(mCameraPosition);
  }

  void OrthographicCameraController::offsetZoomLevel(const f32 offset) {
    setZoomLevel(mZoomLevel + offset);
  }

  void OrthographicCameraController::offsetRotation(const f32 offset) {
    mCameraRotation += offset;
    mCamera.setRotation(mCameraRotation);
  }

} // namespace Gui