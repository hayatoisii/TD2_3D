#pragma once
#include <ViewProjection.h>
#include <Model.h>
#include <WorldTransform.h>
class PlayerBullet {
public:
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity);

	void Update();

	Vector3 GetWorldPosition();

	void Draw(const ViewProjection& viewProjection_);

	~PlayerBullet();

	bool IsDead() const { return isDead_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();


private:
	WorldTransform worldtransfrom_;

	Model* model_ = nullptr;

	// uint32_t textureHandle_ = 0;

	Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 3;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;
};
