#pragma once
#include "AABB.h"
#include "Player.h"

#include <viewprojection.h>
#include <3d/Model.h>
#include <3d/WorldTransform.h>
enum class BulletType {
	Bom,
	Atk,
};

class EnemyBullet {
public:
	BulletType bulletType_;
	void Initialize(Model* model, const Vector3& position, const Vector3& velocity, BulletType type);

	void Update();

	void Draw(const ViewProjection& camera);

	~EnemyBullet();

	bool IsDead() const { return isDead_; }

	// 衝突を検出したら呼び出されるコールバック関数
	void OnCollision();

	Vector3 GetWorldPosition();

	AABB GetAABB();

	Player* player_ = nullptr;

	void SetTarget(Player* target);

private:
	WorldTransform worldtransfrom_;
	Model* model_ = nullptr;
	Vector3 velocity_;

	// 寿命<frm>
	static const int32_t kLifeTime = 60 * 3;
	// デスタイマー
	int32_t deathTimer_ = kLifeTime;
	// デスフラグ
	bool isDead_ = false;

	static inline const float kWidth = 1.0f;
	static inline const float kHeight = 1.0f;
};