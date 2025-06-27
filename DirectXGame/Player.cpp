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
	PryAudio_ = Audio::GetInstance();
	DmgAudio_ = Audio::GetInstance();
	

	ParryAudioHandle_ = PryAudio_->LoadWave("./sound/parry.wav");
	DamageAudioHandle_ = DmgAudio_->LoadWave("./sound/damage.wav");

	// テクスチャロード
	HpHandle1_ = TextureManager::Load("/PlayerHp/1HUD.png");
	HpHandle2_ = TextureManager::Load("/PlayerHp/2HUD.png");
	HpHandle3_ = TextureManager::Load("/PlayerHp/3HUD.png");
	HpHandle4_ = TextureManager::Load("/PlayerHp/4HUD.png");
	HpHandle5_ = TextureManager::Load("/PlayerHp/5HUD.png");

	// スプライト生成
	HpSprite1_ = Sprite::Create(HpHandle1_, {0, 0});
	HpSprite2_ = Sprite::Create(HpHandle2_, {0, 0});
	HpSprite3_ = Sprite::Create(HpHandle3_, {0, 0});
	HpSprite4_ = Sprite::Create(HpHandle4_, {0, 0});
	HpSprite5_ = Sprite::Create(HpHandle5_, {0, 0});
}

void Player::OnCollision() {
	isDamage_ = true;

	if (isDamage_ == true && !isParry_) {
		hp -= Damage;
		DmgAudio_->playAudio(DamageAudio_, DamageAudioHandle_, false, 0.7f);
		// 点滅を開始する
		isBlinking_ = true;
		blinkTimer_ = kBlinkDuration;
	}

	if (hp <= 0) {
		isDead_ = true;
	}
}

void Player::Parry() {
	isParry_ = true;
	if (isParry_) {
		PryAudio_->playAudio(ParryAudio_, ParryAudioHandle_, false, 0.7f);
	}
}

void Player::Dead() {}

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
	/*const float kRotSpeed = 0.5f;*/

	// 押した方向で移動ベクトルを変更(左右)
	if (Input::GetInstance()->PushKey(DIK_A)) {
		move.x -= kCharacterSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_D)) {
		move.x += kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更(上下)
	if (Input::GetInstance()->PushKey(DIK_W)) {
		move.y += kCharacterSpeed;
	} else if (Input::GetInstance()->PushKey(DIK_S)) {
		move.y -= kCharacterSpeed;
	}

	// 押した方向で移動ベクトルを変更
	// スペースキーで回転を開始する
	if (Input::GetInstance()->TriggerKey(DIK_SPACE) && rotationTimer_ == 0) {
		rotationTimer_ = kRotationDuration; // 回転を開始する
	}

	if (rotationTimer_ > 0) {
		float rotationStep = (2.0f * 3.14159265359f) / kRotationDuration; // 1フレームごとの回転量（360度をkRotationDurationフレームで割る）
		worldtransfrom_.rotation_.y += rotationStep;
		rotationTimer_--;
	}
	worldtransfrom_.translation_.x += move.x;
	worldtransfrom_.translation_.y += move.y;

	const float kMoveLimitX = 3;
	const float kMoveLimitY = 1.5;

	worldtransfrom_.translation_.x = std::clamp(worldtransfrom_.translation_.x, -kMoveLimitX, kMoveLimitX);
	worldtransfrom_.translation_.y = std::clamp(worldtransfrom_.translation_.y, -kMoveLimitY, kMoveLimitY);

	// 点滅の更新
	if (isBlinking_) {
		blinkTimer_--;
		if (blinkTimer_ <= 0) {
			isBlinking_ = false;
		}
	}
	

	/*ImGui::Begin("Setmove");
	ImGui::SliderFloat("Move X", &worldtransfrom_.translation_.x, -1.0f, 1.0f);
	ImGui::SliderFloat("Move Y", &worldtransfrom_.translation_.y, -1.0f, 1.0f);
	ImGui::End();*/

	/*ImGui::Text("PlayerHP:%d", hp);*/
	worldtransfrom_.UpdateMatrix();
}

void Player::Update2() {

	// 回転角度（時間経過に応じて増加）
	static float angle = 0.0f;
	const float radius = 10.0f; // プレイヤーの回転半径
	const float speed = 0.01f;  // 回転速度

	// Z軸を中心に円を描くようにプレイヤー位置を更新
	worldtransfrom_.translation_.x = radius * cos(angle); // X座標を更新
	worldtransfrom_.translation_.y = radius * sin(angle); // Y座標は固定
	worldtransfrom_.translation_.z = radius * sin(angle); // Z座標を更新
	worldtransfrom_.rotation_.y += 0.03f;
	worldtransfrom_.rotation_.x += 0.02f;

	angle += speed;
	if (angle >= 2 * static_cast<float>(3.14f)) { // M_PIをfloatにキャスト
		angle -= 2 * static_cast<float>(3.14f);
	}

	worldtransfrom_.UpdateMatrix();
}

void Player::Update3() { worldtransfrom_.UpdateMatrix(); }


void Player::ClearUpdate() {

	// 回転角度（時間経過に応じて増加）
	static float angle = 0.0f;
	const float radius = 2.9f; // プレイヤーの回転半径
	const float speed = 0.01f; // 回転速度

	// Z軸を中心に円を描くようにプレイヤー位置を更新
	worldtransfrom_.translation_.x = radius * cos(angle);         // X座標を更新
	worldtransfrom_.translation_.y = radius * sin(angle) - 0.5f;  // Y座標は固定
	worldtransfrom_.translation_.z = radius * sin(angle) - 36.5f; // Z座標を更新
	worldtransfrom_.rotation_.y += 0.03f;
	worldtransfrom_.rotation_.x += 0.02f;

	angle += speed;
	if (angle >= 2 * static_cast<float>(3.14f)) {
		angle -= 2 * static_cast<float>(3.14f);
	}

	worldtransfrom_.UpdateMatrix();
}


void Player::HpDraw() {
	// HP描画
	if (hp == 500) {
		HpSprite5_->Draw();
	}
	if (hp == 400) {
		HpSprite4_->Draw();
	}
	if (hp == 300) {
		HpSprite3_->Draw();
	}
	if (hp == 200) {
		HpSprite2_->Draw();
	}
	if (hp == 100) {
		HpSprite1_->Draw();
	}
}

void Player::Draw() {
	if (!isDead_) {
		// 点滅中はフレームごとに描画するかどうかを切り替える
		if (!isBlinking_ || (blinkTimer_ / (kBlinkDuration / 10)) % 2 == 0) {
			model_->Draw(worldtransfrom_, *camera_);
		
		}
	}

	// 弾描画
	for (PlayerBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}
}
