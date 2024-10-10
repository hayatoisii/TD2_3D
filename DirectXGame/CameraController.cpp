#include "CameraController.h"
#include <algorithm>

// 線形補間関数
template<typename T> T Lerp(const T& a, const T& b, float t) { return a * (1 - t) + b * t; }

void CameraController::Initialize() { viewProjection_.Initialize(); }

void CameraController::Update() {

	
}

void CameraController::Reset() {
}

ViewProjection CameraController::GetViewPosition() { return ViewProjection(); }