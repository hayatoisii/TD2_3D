#include "Player.h"
#include "Enemy.h"
#include <algorithm>
#include <cassert>

Player::~Player() {
	// model_ = nullptr;
	// camera_ = nullptr;
	delete modelbullet_;
	for (PlayerBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Player::Initialize(Model* model, ViewProjection* camera, const Vector3& pos) {

	assert(model);
	model_ = model;
	camera_ = camera;
	modelbullet_ = Model::CreateFromOBJ("bom", true);
	worldtransfrom_.Initialize();
	worldtransfrom_.translation_ = pos;
	input_ = Input::GetInstance();
}

void Player::OnCollision() { isDead_ = true; }

void Player::Parry() { isParry_ = true; }

void Player::Attack() {

	// if (input_->TriggerKey(DIK_SPACE)) {
	if (isParry_ == true) {

		assert(enemy_);

		Vector3 moveBullet = worldtransfrom_.translation_;

		// 弾の速度
		const float kBulletSpeed = -3.0f;

		Vector3 velocity(0, 0, 0);

		Vector3 enemyWorldtransform = enemy_->GetWorldPosition();
		Vector3 playerWorldtransform = GetWorldPosition();
		Vector3 homingBullet = playerWorldtransform - enemyWorldtransform;
		homingBullet = Normalize(homingBullet);
		velocity.x += kBulletSpeed * homingBullet.x;
		velocity.y += kBulletSpeed * homingBullet.y;
		velocity.z += kBulletSpeed * homingBullet.z;

		// 弾を生成し、初期化
		PlayerBullet* newBullet = new PlayerBullet();
		newBullet->Initialize(modelbullet_, moveBullet, velocity);

		// 弾を登録する
		bullets_.push_back(newBullet);

		isParry_ = false;
	}
}

// ワールド座標を取得
Vector3 Player::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldtransfrom_.matWorld_.m[3][0];
	worldPos.y = worldtransfrom_.matWorld_.m[3][1];
	worldPos.z = worldtransfrom_.matWorld_.m[3][2];

	return worldPos;
}

AABB Player::GetAABB() {
	Vector3 worldPos = GetWorldPosition();

	AABB aabb;

	aabb.min = {worldPos.x - kWidth / 2.0f, worldPos.y - kHeight / 2.0f, worldPos.z - kWidth / 2.0f};
	aabb.max = {worldPos.x + kWidth / 2.0f, worldPos.y + kHeight / 2.0f, worldPos.z + kWidth / 2.0f};

	return aabb;
}

void Player::Update() {

	Attack();

	// 弾更新
	for (PlayerBullet* bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグの立った弾を削除
	bullets_.remove_if([](PlayerBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};

	// キャラクターの移動速さ
	const float kCharacterSpeed = 0.05f;
	// 回転速さ[ラジアン/frame]
	const float kRotSpeed = 0.5f;

	// 押した方向で移動ベクトルを変更(左右)
	if (Input::GetInstance()->PushKey(DIK_LEFT)) {
		move.x -= kCharacterSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_RIGHT)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (Input::GetInstance()->PushKey(DIK_UP)) {
		move.y += kCharacterSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_DOWN)) {
		move.y -= kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更
	if (Input::GetInstance()->PushKey(DIK_SPACE)) {
		worldtransfrom_.rotation_.y += kRotSpeed;
	}
	worldtransfrom_.translation_.x += move.x;
	worldtransfrom_.translation_.y += move.y;

	const float kMoveLimitX = 30;
	const float kMoveLimitY = 15;

	worldtransfrom_.translation_.x = std::clamp(worldtransfrom_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldtransfrom_.translation_.y = std::clamp(worldtransfrom_.translation_.y, -kMoveLimitY, kMoveLimitY);

	/*ImGui::Begin("Setmove");
	ImGui::SliderFloat("Move X", &worldtransfrom_.translation_.x, -1.0f, 1.0f);
	ImGui::SliderFloat("Move Y", &worldtransfrom_.translation_.y, -1.0f, 1.0f);
	ImGui::End();*/

	worldtransfrom_.UpdateMatrix();
}

void Player::Draw() {

	model_->Draw(worldtransfrom_, *camera_);

	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}
}