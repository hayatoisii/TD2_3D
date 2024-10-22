
#include <algorithm>

#include "CameraController.h"
#include "Player.h"
#include <algorithm>
#include <iostream>

void CameraController::Initialize() { viewProjection_.Initialize(); }

float Lerp(float x1, float x2, float t) {
	return x1 + t * (x2 - x1); // 修正された補間公式
}

Vector3 Lerp(const Vector3& v1, const Vector3& v2, float t) { return Vector3(Lerp(v1.x, v2.x, t), Lerp(v1.y, v2.y, t), Lerp(v1.z, v2.z, t)); }

void CameraController::Update() {
	if (!target_) {
		return;
	}

	const WorldTransform& targetWorldTransform = target_->GetWorldTransform();
	const Vector3& targetVelocity = target_->GetVelocity();

	// 追従対象とオフセットから目標座標を計算
	targetPosition_.x = targetWorldTransform.translation_.x + targetOffset_.x + targetVelocity.x * kVelocityBias;
	targetPosition_.y = targetWorldTransform.translation_.y + targetOffset_.y + targetVelocity.y * kVelocityBias;
	targetPosition_.z = targetWorldTransform.translation_.z + targetOffset_.z + targetVelocity.z * kVelocityBias;

	targetOffset_.y = 1.0f;

	// 座標補間によりゆったり追従
	// camera_.translation_ = Lerp(camera_.translation_, targetPosition_, kInterpolationRate);
	viewProjection_.translation_ = targetPosition_;

	// 行列を更新する
	viewProjection_.UpdateMatrix();
}

void CameraController::Reset() {
	if (!target_) {
		return;
	}
	viewProjection_.UpdateMatrix();
}