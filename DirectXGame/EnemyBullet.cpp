#include "EnemyBullet.h"
//#include "Enemy.h"

#include <algorithm>
#include <cassert>

EnemyBullet::~EnemyBullet() { model_ = nullptr; }

void EnemyBullet::Initialize(Model* model, const Vector3& position, const Vector3& velocity, BulletType type) {
	assert(model);
	model_ = model;
	worldtransfrom_.translation_ = position;
	worldtransfrom_.Initialize();
	velocity_ = velocity; // プレイヤー方向に向かう初期速度を正しく設定
	bulletType_ = type;
}

// ワールド座標を取得
Vector3 EnemyBullet::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldtransfrom_.matWorld_.m[3][0];
	worldPos.y = worldtransfrom_.matWorld_.m[3][1];
	worldPos.z = worldtransfrom_.matWorld_.m[3][2];

	return worldPos;
}

AABB EnemyBullet::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void EnemyBullet::Update() {
	// 弾の寿命を減らす
	if (--deathTimer_ <= 0) {
		isDead_ = true;
		return;
	}

	if (!player_)
		return;

	 switch (bulletType_) {
	case BulletType::Bom:
		// Bom用の縦回転などの処理
		{

			// 弾の現在位置とプレイヤーの位置を取得
			Vector3 bulletPosition = GetWorldPosition();
			Vector3 playerPosition = player_->GetWorldPosition();

			// プレイヤーへのベクトルを計算
			Vector3 toPlayer = Normalize(playerPosition - bulletPosition);

			// 現在の進行方向を少しずつプレイヤー方向に補正
			float adjustmentFactor = 0.045f; // 補正の割合。値を調整することで追尾の滑らかさを変更
			velocity_ = Normalize(velocity_ + toPlayer * adjustmentFactor);

			// 速度に基づいて弾の位置を更新
			worldtransfrom_.translation_.x += velocity_.x * 0.9f;
			worldtransfrom_.translation_.y += velocity_.y * 0.9f;
			worldtransfrom_.translation_.z += velocity_.z * 0.9f;

			// ワールド行列を更新
			worldtransfrom_.UpdateMatrix();
		}
		break;

	case BulletType::Atk:

		// 弾の現在位置とプレイヤーの位置を取得
		Vector3 bulletPosition = GetWorldPosition();
		Vector3 playerPosition = player_->GetWorldPosition();

		// プレイヤーへのベクトルを計算
		Vector3 toPlayer = Normalize(playerPosition - bulletPosition);

		// 現在の進行方向を少しずつプレイヤー方向に補正
		float adjustmentFactor = 0.025f; // 補正の割合。値を調整することで追尾の滑らかさを変更
		velocity_ = Normalize(velocity_ + toPlayer * adjustmentFactor);

		// 回転角度を少しずつ増加
		float rotationSpeed = 0.5f;                  // 回転速度
		worldtransfrom_.rotation_.y += rotationSpeed; // Y方向に回転を加える

		// 速度に基づいて弾の位置を更新（速度をゆっくりに調整）
		float movementSpeed = 0.5f; // 移動速度の調整
		worldtransfrom_.translation_.x += velocity_.x * movementSpeed;
		worldtransfrom_.translation_.y += velocity_.y * movementSpeed;
		worldtransfrom_.translation_.z += velocity_.z * movementSpeed;

		// ワールド行列を更新
		worldtransfrom_.UpdateMatrix();
	}

}

void EnemyBullet::OnCollision() { isDead_ = true;


}

void EnemyBullet::Draw(const ViewProjection& camera) {

	if (!isDead_) {
		// モデルの描画
		model_->Draw(worldtransfrom_, camera);
	}
}

