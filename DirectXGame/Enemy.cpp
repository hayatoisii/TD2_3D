#include "Enemy.h"
#include "Player.h"
#include "imgui.h"

#include <cstdlib> // rand() 関数を使うために必要
#include <ctime>   // time() 関数を使うために必要

Enemy::~Enemy() {

	delete modelbullet_;
	for (EnemyBullet* bullet : bullets_) {
		delete bullet;
	}
}

void Enemy::Initialize(Model* model, ViewProjection* camera, const Vector3& pos) {
	assert(model);
	model_ = model;
	camera_ = camera;
	modelbullet_ = Model::CreateFromOBJ("Bom", true);
	atkbullet_ = Model::CreateFromOBJ("Bullet", true);
	worldtransfrom_.translation_ = pos;
	worldtransfrom_.Initialize();
	input_ = Input::GetInstance();
	
}

Vector3 Enemy::GetWorldPosition() {

	// ワールド座標を入れる変数
	Vector3 worldPos;
	// ワールド行列の平行移動成分を取得（ワールド座標）
	worldPos.x = worldtransfrom_.matWorld_.m[3][0];
	worldPos.y = worldtransfrom_.matWorld_.m[3][1];
	worldPos.z = worldtransfrom_.matWorld_.m[3][2];

	return worldPos;
}

void Enemy::OnCollision() {
	isDamage_ = true;

	if (isDamage_ == true) {
		hp -= 100;
	}

	if (hp <= 0) {
		isDead_ = true;
		isBlinking_ = true;           // 点滅を開始する
		blinkTimer_ = kBlinkDuration; // 点滅時間を設定
		clearStartTime_ = clock();    // 敵が倒された瞬間の時間を記録
	}
}

void EnemyBullet::SetTarget(Player* target) {
	player_ = target; // プレイヤーをターゲットとして設定
}

void Enemy::Fire() {

	assert(player_);

	spawnTimer--;

	if (spawnTimer < -0.0f) {

		Vector3 moveBullet = worldtransfrom_.translation_;

		// 弾の初期速度
		const float kBulletSpeed = 0.005f;

		// プレイヤーへのベクトルを計算
		Vector3 playerWorldPosition = player_->GetWorldPosition();
		Vector3 enemyWorldPosition = GetWorldPosition();
		Vector3 toPlayer = Normalize(playerWorldPosition - enemyWorldPosition);

		// 初期速度はプレイヤーに向かうベクトルで設定
		Vector3 velocity = toPlayer * kBulletSpeed;

		// 弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(modelbullet_, moveBullet, velocity,BulletType::Bom);


		// ターゲット（プレイヤー）を追尾するように設定
		newBullet->SetTarget(player_);

		// 弾をリストに追加
		bullets_.push_back(newBullet);

		// 発射タイマーリセット
		spawnTimer = kFireInterval;
	}
}

void Enemy::preFire() {
	assert(player_);

	spawnTimer--;

	if (spawnTimer < -0.0f) {

		Vector3 moveBullet = worldtransfrom_.translation_;

		// 弾の初期速度
		const float kBulletSpeed = 0.005f;

		// プレイヤーへのベクトルを計算
		Vector3 playerWorldPosition = player_->GetWorldPosition();
		Vector3 enemyWorldPosition = GetWorldPosition();
		Vector3 toPlayer = Normalize(playerWorldPosition - enemyWorldPosition);

		// 初期速度はプレイヤーに向かうベクトルで設定
		Vector3 velocity = toPlayer * kBulletSpeed;

		// 弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(modelbullet_, moveBullet, velocity,BulletType::Bom);

		// ターゲット（プレイヤー）を追尾するように設定
		newBullet->SetTarget(player_);

		// 弾をリストに追加
		bullets_.push_back(newBullet);

		// 発射タイマーリセット
		spawnTimer = kFireInterval;
	}
}

// 攻撃パターンの実装
void Enemy::AttackPattern() {
	if (hp > 1500) {
		EnhancedFire(); // 1500 ~ 1000 の攻撃
	} else if (hp > 1000) {
		NormalFire(); // 2000 ~ 1500 の攻撃
	} else if (hp > 500) {
		BombFire(); // 1000 ~ 500 の攻撃
	} else {
		FinalFire(); // 500 ~ 0 の攻撃
	}
}

// ノーマルショット
void Enemy::NormalFire() {
	// 通常攻撃の発射処理
	Fire(); // 既存の発射処理を使う
}

void Enemy::EnhancedFire() {
	assert(player_);

	spawnTimer--;

	if (spawnTimer < -0.0f) {

		Vector3 moveBullet = worldtransfrom_.translation_;

		// 弾の初期速度
		const float kBulletSpeed = 0.005f;

		// プレイヤーへのベクトルを計算
		Vector3 playerWorldPosition = player_->GetWorldPosition();
		Vector3 enemyWorldPosition = GetWorldPosition();
		Vector3 toPlayer = Normalize(playerWorldPosition - enemyWorldPosition);

		// 初期速度はプレイヤーに向かうベクトルで設定
		Vector3 velocity = toPlayer * kBulletSpeed;

		// Atkタイプの弾を生成し、初期化
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(atkbullet_, moveBullet, velocity, BulletType::Atk);

		// ターゲット（プレイヤー）を追尾するように設定
		newBullet->SetTarget(player_);

		// 弾をリストに追加
		bullets_.push_back(newBullet);

		// 発射タイマーリセット
		spawnTimer = kFireInterval;
	}
}
// ボム攻撃
void Enemy::BombFire() {
	// 大きな爆発を伴う攻撃
	for (int i = 0; i < 5; i++) {
		Vector3 offset = {std::sin(i * 0.5f), 0.0f, std::cos(i * 0.5f)}; // 円形に弾を展開
		Vector3 bulletPos = worldtransfrom_.translation_ + offset;
		Vector3 velocity = offset * 0.02f; // 外側に拡がる速度
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(modelbullet_, bulletPos, velocity,BulletType::Atk);
		bullets_.push_back(newBullet);
	}
}

// 最終攻撃
void Enemy::FinalFire() {
	// 全力の攻撃を行う
	for (int i = 0; i < 10; i++) {
		Vector3 offset = {std::sin(i * 0.2f), 0.0f, std::cos(i * 0.2f)};
		Vector3 bulletPos = worldtransfrom_.translation_ + offset;
		Vector3 velocity = offset * 0.05f; // 広範囲に散らばる弾
		EnemyBullet* newBullet = new EnemyBullet();
		newBullet->Initialize(modelbullet_, bulletPos, velocity,BulletType::Atk);
		bullets_.push_back(newBullet);
	}
}

void Enemy::Update() {

	FireTimer_++;

	if (FireTimer_ >= 800 && FireTimer_ <= 1320) {
		preFire();
	}
	if (input_->TriggerKey(DIK_RETURN)) { // スペースキーが押されたら
		FireTimer_ = 1600;                // ディレイが開始されたことをフラグで管理
	}
	if (FireTimer_ >= 1700) {
		AttackPattern();
	}
	// sin波に基づいて上下に動く処理
	static float time = 0.0f;              // 時間を記録するカウンター
	time += 0.08f;                         // 時間を進める
	float yOffset = std::sin(time) * 0.5f; // 振幅0.5のsin波で上下に動かす

	// 初期位置からのY軸のオフセットを加える
	worldtransfrom_.translation_.y = initialPosition_.y + yOffset;

	// 点滅の更新
	if (isBlinking_) {
		blinkTimer_--;
		if (blinkTimer_ <= 0) {
			isBlinking_ = false;
			isClear_ = true; // 点滅が終了したら敵をクリア状態にする
		}
	}

	// 弾更新
	for (EnemyBullet* bullet : bullets_) {
		bullet->Update();
	}

	// デスフラグの立った弾を削除
	bullets_.remove_if([](EnemyBullet* bullet) {
		if (bullet->IsDead()) {
			delete bullet;
			return true;
		}
		return false;
	});

	// キャラクターの移動ベクトル
	Vector3 move = {0, 0, 0};
	// 接近
	Vector3 accessSpeed = {0.1f, 0.1f, 0.1f};
	// 離脱
	Vector3 eliminationSpeed = {0.3f, 0.3f, 0.3f};

	worldtransfrom_.UpdateMatrix();

	ImGui::Text("EnemyHP:%d", hp);
	ImGui::Text("FireTimer:%d", FireTimer_);
}

bool Enemy::ShouldTransitionPhase() const {
	if (isDead_ && (clock() >= clearStartTime_ + 2200)) {
		// 点滅終了後、2秒経過した場合にシーン遷移を許可
		return true;
	}
	return false;
}

void Enemy::Draw() {
	// コマンドリストの取得

	if (!isClear_) {
		// 点滅中はフレームごとに描画するかどうかを切り替える
		if (!isBlinking_ || (blinkTimer_ / 5) % 2 == 0) {
			model_->Draw(worldtransfrom_, *camera_);
		}
	}

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}
}