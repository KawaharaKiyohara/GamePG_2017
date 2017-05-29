#include "stdafx.h"
#include "Enemy.h"
#include "GameCamera.h"
#include "Player.h"
#include "CollisionWorld.h"

namespace {
	class EmitterAttackCollision : public IGameObject {

	public:
		float timer = 0.0f;
		CVector3 emitPos;
		Enemy* enemy = nullptr;
		void Update() override
		{
			if (enemy->IsDead() || enemy->state != Enemy::State_Attack) {
				DeleteGO(this);
			}
			timer -= GameTime().GetFrameDeltaTime();
			if (timer < 0.0f) {
				//攻撃コリジョンを発生させる。
				collisionWorld->Add(emitPos, 0.5f, 0.5f, enCollisionAttr_EnemyAttack);
				DeleteGO(this);
			}
		}
	};
}

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}
//攻撃コリジョンをエミット。
//pos エミットする位置。
//delayTime エミットまでにかかる時間。
void Enemy::EmitAttackCollision(CVector3 pos, float delayTime)
{
	EmitterAttackCollision* emitAttackColl = NewGO<EmitterAttackCollision>(0);
	emitAttackColl->emitPos = pos;
	emitAttackColl->timer = delayTime;
	emitAttackColl->enemy = this;
}
bool Enemy::Start()
{
	Init("Assets/modelData/enemy_00.X", gameCamera->GetCamera());

	//マテリアルを検索。
	CSkinModelMaterial* mat = skinModelData.GetBody()->FindMaterial("Skeleton_D.png");
	if (mat != nullptr) {
		//見つかった。
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_NormalMap, *TextureResources().Load("Assets/modelData/Skeleton_N.png"));
	}
	skinModel.SetHasNormalMap(true);
	skinModel.SetFresnelFlag(true);
	characterController.Init(0.6f, 0.3f, position);
	animation.SetAnimationLoopFlag(AnimationAttack, false);
	animation.SetAnimationLoopFlag(AnimationDamage, false);
	return true;
}
void Enemy::Update()
{
	Object3D::Update();
	
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;
	moveSpeed.y -= 9.8f * GameTime().GetFrameDeltaTime();

	UpdateFSM();
	CheckDamage();
	characterController.Execute(moveSpeed, GameTime().GetFrameDeltaTime());
	position = characterController.GetPosition();
	//旋回
	Turn();
	
}
//旋回処理。
void Enemy::Turn()
{
	//モンスターを旋回させる。
	float angle = atan2f(direction.x, direction.z);
	rotation.SetRotation(CVector3::AxisY, angle);
}
//ダメージ判定。
void Enemy::CheckDamage()
{
	if (state == State_Damage) {
		//ダメージ中ならリターン。
		return;
	}
	//プレイヤーの攻撃コリジョンとのあたり判定を行う。
	int numCollision = collisionWorld->m_collisionList.size();
	for (int i = 0; i < numCollision; i++) {
		if (collisionWorld->m_collisionList[i]->attr == enCollisionAttr_PlayerAttack) {
			//プレイヤーが発生させた攻撃コリジョンとのあたりを調べる。
			//コリジョンとの距離を調べる。
			CVector3 diff = collisionWorld->m_collisionList[i]->pos - position;
			if (diff.Length() < 3.0f) {
				//ダメージを受ける。
				animation.PlayAnimation(AnimationDamage, 0.2f);
				state = State_Damage;
			}
		}
	}
}
void Enemy::UpdateFSM()
{
	//プレイヤーとの距離が一定値以下になったら攻撃する。
	CVector3 diff;
	diff = player->position - position;
	
	
	if (state == State_Idle) {
		//待機状態。
		//距離が2m以下なら攻撃状態に遷移。
		if (diff.Length() < 2.0f) {
			animation.PlayAnimation(AnimationAttack, 0.2f);
			EmitAttackCollision(position, 1.0f);
			state = State_Attack;	//攻撃状態に遷移。
		}
		else if (diff.Length() < 5.0f) {
			animation.PlayAnimation(AnimationWalk, 0.2f);
			state = State_Chase;
		}
	}
	else if (state == State_Chase)
	{
		//追跡中。
		//プレイヤーの方に進むという処理を行うためには、プレイヤーの方向を向いている、向きベクトルを求める必要がある。
		//diffにはエネミーからプレイヤーまでのベクトルが入っている。
		//このベクトルの大きさを１にすると(正規化)向きベクトルになる。
		CVector3 toPlayerDirection = diff;
		toPlayerDirection.Normalize();
		moveSpeed = toPlayerDirection * 2.0f;
		if(moveSpeed.Length() > 0.01f) {
			//進行方向を向く。
			direction = moveSpeed;
			direction.Normalize();
		}
		
		if (diff.Length() < 2.0f) {
			//プレイヤーとの距離が2m以下になったので攻撃を行う。
			animation.PlayAnimation(AnimationAttack, 0.2f);
			EmitAttackCollision(position, 1.0f);
			state = State_Attack;	//攻撃状態に遷移。
		}
		

		if (diff.Length() > 10.0f) {
			//プレイヤーとの距離が一定値以上になったので追跡をあきらめて待機状態に遷移する。
			animation.PlayAnimation(AnimationStand, 0.2f);
			state = State_Idle;
		}
	}
	else if (state == State_Attack) {
		//攻撃中
		CVector3 toPlayerDirection = diff;
		toPlayerDirection.Normalize();
		direction = toPlayerDirection;
		direction.Normalize();
		
		if (animation.IsPlay() == false) {
			//ダメージアニメーションが終了したので待機状態に遷移する。
			animation.PlayAnimation(AnimationStand, 0.2f);
			state = State_Idle;
		}
	}
	else if (state == State_Damage) {
		//ダメージを受けている。
		if (animation.IsPlay() == false) {
			//ダメージアニメーションが終了したので待機状態に遷移する。
			animation.PlayAnimation(AnimationStand, 0.2f);
			state = State_Idle;
		}
	}
}