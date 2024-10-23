#include "Enemy.h"
#include "Player.h"


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

	// DeathParticles
	deathParticles_ = new DeathParticles;
	deathParticlesModel_ = Model::CreateFromOBJ("deathParticle", true); // 3Dモデルの生成
	deathParticles_->Initialize(initialPosition_, deathParticlesModel_, camera);
	velocity_ = {0, -kWalkSpeed, 0};
	walkTimer = 0.0f;
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
		enemyhp -= 1000;
	}

	if (enemyhp <= 0) {
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
		newBullet->Initialize(modelbullet_, moveBullet, velocity, BulletType::Bom);

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
		newBullet->Initialize(modelbullet_, moveBullet, velocity, BulletType::Bom);

		// ターゲット（プレイヤー）を追尾するように設定
		newBullet->SetTarget(player_);

		// 弾をリストに追加
		bullets_.push_back(newBullet);

		// 発射タイマーリセット
		spawnTimer = kFireInterval2;
	}
}

// 攻撃パターンの実装
void Enemy::AttackPattern() {
	if (enemyhp > 1500) {
		NormalFire(); // 2000 ~ 1500 の攻撃
	} else if (enemyhp > 1000) {
		EnhancedFire(); // 1500 ~ 1000 の攻撃
		Fire();
	} else if (enemyhp > 500) {
		Fire();
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

	AtkBulletspawnTimer--;

	if (AtkBulletspawnTimer < -0.0f) {

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
		AtkBulletspawnTimer = 70;
	}
}
// ボム攻撃
void Enemy::BombFire() {
	assert(player_);

	BomBulletspawnTimer--;

	if (BomBulletspawnTimer < -0.0f) {

		// 弾の発射位置（敵の中心）
		Vector3 moveBullet = worldtransfrom_.translation_;

		// 弾の初期速度
		const float kBulletSpeed = 0.005f;

		// 円の半径を設定する
		const float radius = 15.0f; // 半径5.0で大きく円を描く

		// 弾を円周上に2個ずつ展開する処理
		for (int i = 0; i < 2; i++) {
			// 弾の角度を設定 (180度間隔で展開)
			float angle = i * 3.14159f; // 2つの弾が180度の間隔で展開

			// 弾の速度を円を描くように設定
			Vector3 velocity = {
			    std::cos(angle) * radius, // X軸方向の速度
			    0.0f,                     // Y軸方向はそのまま
			    std::sin(angle) * radius  // Z軸方向の速度
			};

			// 速度ベクトルを正規化して速度を調整
			velocity = Normalize(velocity) * kBulletSpeed;

			// 弾を生成し、初期化
			EnemyBullet* newBullet = new EnemyBullet();
			newBullet->Initialize(atkbullet_, moveBullet, velocity, BulletType::Atk);

			// ターゲット（プレイヤー）を追尾するように設定
			newBullet->SetTarget(player_);

			// 弾をリストに追加
			bullets_.push_back(newBullet);
		}

		// 発射タイマーリセット
		BomBulletspawnTimer = 110;
	}
}

// 最終攻撃
void Enemy::FinalFire() {
	assert(player_);
	spawnTimer--;

	// 弾の発射タイミングを管理するためのカウンター
	static int bulletIndex = 0;
	static int bulletSpawnCounter = 0; // 弾の発射間隔を制御するカウンター
	const int bulletSpawnInterval = 7; // 8フレームごとに1発発射

	// 渦巻きの半径増加速度と初期角度
	static float angle = 30.0f;          // 弾が回転する角度
	const float angleIncrement = 20.0f;  // 角度の増加量を大きくして、弾を散らばらせる
	const float radiusIncrement = 10.0f; // 半径の増加量を大きくしてさらに広がるように設定

	// 全弾発射完了までの処理
	if (bulletSpawnCounter <= 0) {
		// 渦巻きの弾発射処理（15発を順番に発射）
		if (bulletIndex < 15) {
			// 弾の発射位置（敵の中心）
			Vector3 moveBullet = worldtransfrom_.translation_; // 敵の中心に固定

			// 弾の初期速度
			const float kBulletSpeed = 0.01f; // 弾の速度を少し早めに設定

			// 渦巻きを描くための角度と半径の増加
			float currentAngle = angle + bulletIndex * 2.0f * 3.14159f / 15.0f; // 15等分された角度
			float currentRadius = radiusIncrement * (bulletIndex + 1);          // 半径は弾ごとに大きく広がっていく

			// 速度を渦巻き状に設定（弾は敵の中心から放たれる）
			Vector3 velocity = {
			    std::cos(currentAngle) * currentRadius * kBulletSpeed, // X軸方向の速度
			    0.0f,                                                  // Y軸方向は一定
			    std::sin(currentAngle) * currentRadius * kBulletSpeed  // Z軸方向の速度
			};

			// 弾を生成し、初期化（弾の発射位置は敵の中心）
			EnemyBullet* newBullet = new EnemyBullet();
			newBullet->Initialize(atkbullet_, moveBullet, velocity, BulletType::Atk);

			// ターゲット設定は不要（プレイヤー追従しないため）
			newBullet->SetTarget(player_);

			// 弾をリストに追加
			bullets_.push_back(newBullet);

			// 次の弾の発射に向けてインデックスを更新
			bulletIndex++;

			// 次の弾の発射タイミングを設定
			bulletSpawnCounter = bulletSpawnInterval;
		}
	}

	// カウンターをデクリメント
	if (bulletSpawnCounter > 0) {
		bulletSpawnCounter--;
	}

	// 全弾発射が完了したらリセットしてFireを呼び出す
	if (bulletIndex >= 15) {
		bulletIndex = 0;         // インデックスをリセット
		angle += angleIncrement; // 次の発射に向けて角度を更新

		// ここで Fire() を呼び出す
		Fire();
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
		deathParticles_->Update();
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

	//ImGui::Text("EnemyHP:%d", enemyhp);
	/*ImGui::Text("FireTimer:%d", FireTimer_);*/
}

void Enemy::DeathEnemyUpdate() {

	worldtransfrom_.translation_.y += velocity_.y;
	walkTimer += 1.0f / 60.0f;

	float parm = std::sin(std::numbers::pi_v<float> * 2.0f * walkTimer / kWalkMotionTime);
	float radian = kWalkMotionAngleStart + kWalkMontionAngleEnd * (parm + 1.0f) / 2.0f;
	worldtransfrom_.rotation_.z = radian;

	worldtransfrom_.UpdateMatrix();
}

void Enemy::fallingUpdate() { worldtransfrom_.UpdateMatrix(); }

bool Enemy::ShouldTransitionPhase() const {
	if (isDead_ && (clock() >= clearStartTime_ + 1000)) {
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
	if (enemyhp == 0) {
		deathParticles_->Draw();
	}

	for (EnemyBullet* bullet : bullets_) {
		if (enemyhp > 0) {
			// HPが0以上のときのみ弾丸を描画
			bullet->Draw(*camera_);
		} else {
			bullet->Deactivate();
			
			
			break; // HPが0なので処理を打ち切る
		}
	}
}