#pragma once

#include "Model.h"
#include "Viewprojection.h"
#include "WorldTransform.h"
#include <numbers>
#include <assert.h>

class Skydome {

public:
	void Initialize(Model* model, ViewProjection* viewProjection);

	void Update();

	void Draw();

private:
	// ワールド変換データ
	WorldTransform worldTransform_;
	ViewProjection* viewProjection_ = nullptr;

	// モデル
	Model* model_ = nullptr;

	// モデルの移動速度や位置を格納する変数
	Vector3 velocity_; // 移動速度
};