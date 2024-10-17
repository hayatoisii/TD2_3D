#include "Enemy.h"
#include "Player.h"

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
	modelbullet_ = Model::CreateFromOBJ("cube", true);
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

void Enemy::OnCollision() {}

void EnemyBullet::SetTarget(Player* target) {
	player_ = target; // プレイヤーをターゲットとして設定
}

void Enemy::Fire() {

	assert(player_);

	spawnTimer--;

	if (spawnTimer < -0.0f) {

		Vector3 moveBullet = worldtransfrom_.translation_;

		// 弾の初期速度
		const float kBulletSpeed = 0.05f;

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

	/*/
	switch (phase_) {
	case Phase::Approach:
	default:
	    // 移動(ベクトルを加算)
	    worldtransfrom_.translation_.z -= accessSpeed.z;
	    // 規定の位置に到達したら離脱
	    if (worldtransfrom_.translation_.z < 0.0f) {
	        phase_ = Phase::Leave;
	    }
	    break;
	case Phase::Leave:
	    // 移動(ベクトルを加算)
	    worldtransfrom_.translation_.y += eliminationSpeed.y;
	    break;

	}
	/*/

	worldtransfrom_.UpdateMatrix();
}

void Enemy::Draw() {

	model_->Draw(worldtransfrom_, *camera_);

	for (EnemyBullet* bullet : bullets_) {
		bullet->Draw(*camera_);
	}
}