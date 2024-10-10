#include "Bom.h"

void Bom::Initialize(Model* model, ViewProjection* viewProjection) {

	worldTransform_.Initialize();
	model_ = model;
	viewProjection_ = viewProjection;
}

void Bom::Update() {}

void Bom::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}
