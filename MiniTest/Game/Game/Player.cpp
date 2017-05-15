/*!
 *@brief	プレイヤー。
 */
#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"


Player* player = nullptr;

Player::Player()
{
}
Player::~Player()
{
}
bool Player::Start()
{
	//プレイヤーを初期化。
	Init("Assets/modelData/Player.X", gameCamera->GetCamera());
	//キャラクターコントローラーを初期化。
	characterController.Init(0.6f, 0.3f, position);

	//マテリアルを検索。
	CSkinModelMaterial* mat = skinModelData.GetBody()->FindMaterial("Thethief_D.tga");
	if (mat != nullptr) {
		//見つかった。
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_NormalMap, *TextureResources().Load("Assets/modelData/Thethief_N.tga"));
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_SpecularMap, *TextureResources().Load("Assets/modelData/Thethief_S.tga"));
	}
	mat = skinModelData.GetBody()->FindMaterial("Thethief_wuqi_D.tga");
	if (mat != nullptr) {
		//見つかった。
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_NormalMap, *TextureResources().Load("Assets/modelData/Thethief_wuqi_N.tga"));
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_SpecularMap, *TextureResources().Load("Assets/modelData/Thethief_wuqi_S.tga"));
	}
	//法線マップ、スペキュラマップ、フレネル等を設定する。
	skinModel.SetHasNormalMap(true);
	skinModel.SetHasSpeculerMap(true);
	skinModel.SetFresnelFlag(true);
	PlayAnimation(AnimationStand);
	animation.SetAnimationLoopFlag(AnimationJump, false);
	animation.SetAnimationLoopFlag(AnimationAttack_00, false);
	animation.SetAnimationLoopFlag(AnimationAttack_01, false);
	animation.SetAnimationLoopFlag(AnimationAttack_02, false);
	animation.SetAnimationEndTime(AnimationAttack_00, 0.6f);
	animation.SetAnimationEndTime(AnimationAttack_01, 0.8f);
	return true;
}
void Player::Update()
{
	//基底クラスのUpdateを呼び出す。
	Object3D::Update();
	//プレイヤーを動かす。
	Move();
	//プレイヤーを回す。
	Rotation();
	//アニメーション制御。
	AnimationControl();
	CVector3 lightPos = position;
	lightPos.y += 2.0f;
	lightPos.z += 1.0f;
	ShadowMap().SetLightPosition(lightPos);
	CVector3 lightTarget = position;
	ShadowMap().SetLightTarget(lightTarget);
}
void Player::PlayAnimation(AnimationNo animNo)
{
	if (playAnimNo != animNo) {
		animation.PlayAnimation(animNo, 0.2f);
		playAnimNo = animNo;
	}
}
//////////////////////////////////////////////////////////////////////
// ここからプレイヤーを移動させるプログラムが記述されてるよ。
//////////////////////////////////////////////////////////////////////
void Player::Move()
{
	if (attackFlag == 1) {
		//攻撃中ならリターン。
		return;
	}
	//Question 1 パッドの入力でキャラクターを移動させてみよう。
	//Question 2 パッドのAボタン(キーボードのJ)が押されたらキャラクターをジャンプさせてみよう。
	//移動。
	characterController.Execute(moveSpeed, GameTime().GetFrameDeltaTime());
	position = characterController.GetPosition();
}
//////////////////////////////////////////////////////////////////////
// ここからプレイヤーを回転するプログラムが記述されてるよ。
//////////////////////////////////////////////////////////////////////
void Player::Rotation()
{
	if (attackFlag == 1) {
		//攻撃中ならリターン。
		return;
	}
	//Question 3 パッドの入力でキャラクターの方向を変えられるようにしてみよう。
}
//////////////////////////////////////////////////////////////////////
// ここからプレイヤーのアニメーションを制御するプログラムが記述されてるよ。
//////////////////////////////////////////////////////////////////////
void Player::AnimationControl()
{
	if (attackFlag == 0) {	//攻撃中じゃなければ。
		//QUestion 4 パッドの入力で走りアニメーションを再生してみよう。
		//Question 5 Aボタン(キーボードのJ)が押されたらジャンプアニメーションを再生してみよう。
	}
	AttackAnimationControl();
}
//////////////////////////////////////////////////////////////////////
// ここからプレイヤーの攻撃アニメーションを制御するプログラムが記述されているよ。
//////////////////////////////////////////////////////////////////////
void Player::AttackAnimationControl()
{
}