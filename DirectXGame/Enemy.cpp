#include "Enemy.h"
#include "Player.h"
#include "imgui.h"

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
	worldtransfrom_.translation_ = pos;
	worldtransfrom_.Initialize();
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
		newBullet->Initialize(modelbullet_, moveBullet, velocity);

		// ターゲット（プレイヤー）を追尾するように設定
		newBullet->SetTarget(player_);

		// 弾をリストに追加
		bullets_.push_back(newBullet);

		// 発射タイマーリセット
		spawnTimer = kFireInterval;
	}
}

void Enemy::Update() {

	Fire();

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
}

bool Enemy::ShouldTransitionPhase() const {
	if (isDead_ && (clock() >= clearStartTime_ + 2200)) {
		// 点滅終了後、2秒経過した場合にシーン遷移を許可
		return true;
	}
	return false;
}

void Enemy::Draw() {

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