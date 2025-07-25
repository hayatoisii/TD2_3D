#pragma once
#include "Vector3.h"
#include "Viewprojection.h"

class Player;

class CameraController {
public:
	struct Rect {
		float left = 0.0f;
		float right = 1.0f;
		float bottom = 0.0f;
		float top = 1.0f;
	};

	// 座標補間割合
	static inline const float kInterpolationRate = 0.1f; // 補間率を設定
	static inline const float kVelocityBias = 30.0f;     // 速度バイアス

	Rect movableArea_ = {0, 100, 0, 100};
	Rect margin_ = {0, 0, 0, 0}; // 各方向へのマージンを追加
	Player* target_ = nullptr;

	void Initialize();
	void Update();
	ViewProjection GetViewPosition(const Vector3& v1, const Vector3& v2, float t);
	void Reset();

	void SetMovableArea(const Rect& area) { movableArea_ = area; }
	void SetMargin(const Rect& margin) { margin_ = margin; } // マージン設定メソッドを追加
	void setTarget(Player* target) { target_ = target; }
	const ViewProjection& GetViewProjection() const { return viewProjection_; }

	ViewProjection viewProjection_;
	Vector3 targetOffset_ = {0, 0, -7.0f};
	Vector3 targetPosition_; // 目標座標を追加

private:
};