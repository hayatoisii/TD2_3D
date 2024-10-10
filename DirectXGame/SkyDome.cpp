#include "Skydome.h"

void Skydome::Initialize(Model* model, ViewProjection* viewProjection, const Vector3& skydomeRotate) {

	worldTransform_.Initialize();
	worldTransform_.rotation_.y = skydomeRotate.y;
	model_ = model;
	viewProjection_ = viewProjection;

	worldTransform_.rotation_.y = std::numbers::pi_v<float> / -2.0f;
}

void Skydome::Update() {

	
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	worldTransform_.rotation_.y += 0.007f; // 回転速度は調整可能
	// 行列計算
	worldTransform_.UpdateMatrix();
}

void Skydome::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}