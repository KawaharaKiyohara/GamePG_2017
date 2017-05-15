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
	moveSpeed.y -= 9.8f * GameTime().GetFrameDeltaTime();
	characterController.Execute(moveSpeed, GameTime().GetFrameDeltaTime());
	position = characterController.GetPosition();
	AnimationControl();
}
void Enemy::AnimationControl()
{
	if (isAttackFlag == false) {	//攻撃中ではなければ
		//Question 1 プレイヤーとの距離が2m以下になったら攻撃アニメーションを再生する。
	}
	if(isAttackFlag == true){		//攻撃中なら
		//Question 2 攻撃アニメーションの再生が終わったら待機アニメーションを再生する。
	}
}