#include "stdafx.h"
#include "Enemy.h"
#include "GameCamera.h"
#include "Player.h"

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
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
	return true;
}
void Enemy::Update()
{
	Object3D::Update();
	
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;
	moveSpeed.y -= 9.8f * GameTime().GetFrameDeltaTime();

	UpdateFSM();

	characterController.Execute(moveSpeed, GameTime().GetFrameDeltaTime());
	position = characterController.GetPosition();
	
}
/////////////////////////////////////////////////////////////////////////
// ここからモンスターのFSMが実装されている。
/////////////////////////////////////////////////////////////////////////
void Enemy::UpdateFSM()
{
	
	//プレイヤーとの距離を調べる。
	CVector3 playerPos = player->GetPosition();
	//プレイヤーとの距離が一定値以下になったら攻撃する。
	CVector3 diff = player->GetPosition();
	diff = playerPos - position;
	
	if (state == State_Idle) {
		//待機状態。
		//距離が2m以下なら攻撃状態に遷移。
		if (diff.Length() < 2.0f) {
			animation.PlayAnimation(AnimationAttack, 0.2f);
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
		//Question1 プレイヤーの方に向かって進む。
		//(ヒント)
		//  プレイヤーの方に進むという処理を行うためには、プレイヤーの方向を向いている、向きベクトルを求める必要がある。
		//  diffにはエネミーからプレイヤーまでのベクトルが入っている。
		//  このベクトルの大きさを１にすると(正規化)向きベクトルになる。
		CVector3 toPlayerDirection = diff;
		toPlayerDirection.Normalize();
		moveSpeed = toPlayerDirection * 2.0f;
		//Question 2 プレイヤーとの距離が2.0m以下になったら攻撃を行う。
		if (diff.Length() < 2.0f) {
			animation.PlayAnimation(AnimationAttack, 0.2f);
			state = State_Attack;	//攻撃状態に遷移。
		}
		
		
		
		//Question 3 プレイヤーと敵の距離が10m以上になったら、追跡終了で待機状態に戻す・
		if (diff.Length() >= 10.0f) {
			animation.PlayAnimation(AnimationStand, 0.2f);
			state = State_Idle;
		}
		
		
		
		
	}
	else if (state == State_Attack) {
		//攻撃中
		if (animation.IsPlay() == false) {
			//攻撃アニメーションが終わったので待機アニメーションを再生する。
			animation.PlayAnimation(AnimationStand, 0.2f);
			state = State_Idle;
		}
	}
}