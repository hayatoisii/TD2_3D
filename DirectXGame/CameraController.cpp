#include "CameraController.h"
#include <algorithm>

// 線形補間関数
template<typename T> T Lerp(const T& a, const T& b, float t) { return a * (1 - t) + b * t; }

void CameraController::Initialize() {

	viewProjection_.Initialize();
	// カメラの初期設定など
	viewProjection_.rotation_ = {0, 10, -16.0f}; // カメラの位置
}

void CameraController::Update() {
	viewProjection_.rotation_.x = angle; // X軸を45度回転させる

	// ビュープロジェクションの更新
	viewProjection_.UpdateMatrix();
}

void CameraController::Reset() {}

ViewProjection CameraController::GetViewPosition() { return ViewProjection(); }