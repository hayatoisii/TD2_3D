#include "Ground2.h"
#include "imgui.h"

void Ground2::Initialize(Model* model, ViewProjection* viewProjection) {
	worldTransform_.Initialize();
	model_ = model;
	viewProjection_ = viewProjection;
	worldTransform_.translation_.y = -1.5f;
	worldTransform_.translation_.z = 113.0f; // 初期位置
}

void Ground2::Update() {
	// 行列を定数バッファに転送
	worldTransform_.TransferMatrix();

	// Z軸に沿って地面を動かす
	worldTransform_.translation_.z -= 0.6f; // スクロール速度を設定

	// 地面が一定のZ位置を超えたらリセットする
	if (worldTransform_.translation_.z < -113.0f) {
		worldTransform_.translation_.z = worldTransform_.translation_.z + 226.0f; // Ground の終わりと連携して位置を調整
	}

	ImGui::Begin("debug");
	ImGui::Text("Ground2PosY: %f", worldTransform_.translation_.y);
	ImGui::Text("Ground2PosX: %f", worldTransform_.translation_.x);
	ImGui::Text("Ground2PosZ: %f", worldTransform_.translation_.z);
	ImGui::End();

	worldTransform_.UpdateMatrix();
}

void Ground2::Draw() {
	// 3Dモデル描画
	model_->Draw(worldTransform_, *viewProjection_);
}
