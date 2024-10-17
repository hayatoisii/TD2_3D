#include "Ground.h"
#include "imgui.h"

void Ground::Initialize(Model* model, ViewProjection* viewProjection) {
	worldTransform_.Initialize();
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.translation_.y = -1.9f;
	worldTransform_.translation_.z = 0.0f; // 初期位置
}

void Ground::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// Z軸に沿って地面を動かす
	worldTransform_.translation_.z -= 0.6f; // スクロール速度を設定

	// 地面が一定のZ位置を超えたらリセットする
	if (worldTransform_.translation_.z < -113.0f) {
		worldTransform_.translation_.z = worldTransform_.translation_.z + 226.0f; // Ground2 の終わりと連携して位置を調整
	}

	ImGui::Begin("debug");
	ImGui::Text("GroundPosY: %f", worldTransform_.translation_.y);
	ImGui::Text("GroundPosX: %f", worldTransform_.translation_.x);
	ImGui::Text("GroundPosZ: %f", worldTransform_.translation_.z);
	ImGui::End();

	worldTransform_.UpdateMatrix();
}

void Ground::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}
