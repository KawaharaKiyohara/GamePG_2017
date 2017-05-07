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
	moveSpeed.y -= 9.8f * GameTime().GetFrameDeltaTime();
	if (Pad(0).IsPress(enButtonRight)) {     //もしもゲームパッドの右ボタンが押されていたら
		moveSpeed.x = 2.0f;				     //移動速度を1にする。
	}
	else if (Pad(0).IsPress(enButtonLeft)) { //右のボタンが押されていなくて、左のボタンが押されているなら。
		moveSpeed.x = -2.0f;                 //移動速度を-1にする
	}
	else {
		moveSpeed.x = 0.0f;                  //何も入力されていないので移動速度を０にする。
	}
	if (Pad(0).IsPress(enButtonUp)) {
		moveSpeed.z = 2.0f;
	}
	else if (Pad(0).IsPress(enButtonDown)) {
		moveSpeed.z = -2.0f;
	}
	else {
		moveSpeed.z = 0.0f;
	}	
	if (Pad(0).IsTrigger(enButtonA)) { //Aボタンが押されたら
		//ジャンプ
		moveSpeed.y = 3.0;
		characterController.Jump();
	}
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
	//Question 1 キャラクタを右に向かせてみよう。
	if (Pad(0).IsPress(enButtonRight)) {     //もしもゲームパッドの右ボタンが押されていたら
		rotation.SetRotationDeg(CVector3::AxisY, 90.0f);
	}
	//Question 2 キャラクタを左に向かせてみよう。
	if (Pad(0).IsPress(enButtonLeft)) {     //もしもゲームパッドの左ボタンが押されていたら
		rotation.SetRotationDeg(CVector3::AxisY, -90.0f);
	}
	//Question 3 キャラクタを奥に向かせてみよう。
	if (Pad(0).IsPress(enButtonUp)) {     //もしもゲームパッドの上ボタンが押されていたら
		rotation.SetRotationDeg(CVector3::AxisY, 0.0f);
	}
	//Question 4 キャラクタを手前に向かせてみよう。
	if (Pad(0).IsPress(enButtonDown)) {     //もしもゲームパッドの上ボタンが押されていたら
		rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	}
}
//////////////////////////////////////////////////////////////////////
// ここからプレイヤーのアニメーションを制御するプログラムが記述されてるよ。
//////////////////////////////////////////////////////////////////////
void Player::AnimationControl()
{
	//Question 1　走りアニメーションを再生してみよう。
	if (attackFlag == 0) {	//攻撃中じゃなければ。
		if (!characterController.IsJump()) {
			if (Pad(0).IsPress(enButtonUp)) {
				PlayAnimation(AnimationRun);
			}
			else if (Pad(0).IsPress(enButtonDown)) {
				PlayAnimation(AnimationRun);
			}
			else if (Pad(0).IsPress(enButtonRight)) {
				PlayAnimation(AnimationRun);
			}
			else if (Pad(0).IsPress(enButtonLeft)) {
				PlayAnimation(AnimationRun);
			}
			else {
				PlayAnimation(AnimationStand);
			}
		}
		//Question 2 ジャンプアニメーションを再生してみよう。
		if (Pad(0).IsTrigger(enButtonA)) {
			PlayAnimation(AnimationJump);
		}
	}
	AttackAnimationControl();
}
//////////////////////////////////////////////////////////////////////
// ここからプレイヤーの攻撃アニメーションを制御するプログラムが記述されているよ。
//////////////////////////////////////////////////////////////////////
void Player::AttackAnimationControl()
{
	if (attackFlag == 1) {
		//攻撃中！
		//Question 4 攻撃アニメーションの再生が終わったら攻撃フラグを下す。
		if (!animation.IsPlay()) {
			//アニメーションの再生が終わったので攻撃フラグを下す。
			attackFlag = 0;
		}
	}
	else {
		//攻撃中じゃない！
		//Question 1 Xボタンが押されたら小攻撃アニメーションを再生する。
		if (Pad(0).IsPress(enButtonX)) {
			//攻撃中じゃないときにXボタンが押されたら小攻撃・
			//攻撃アニメーションを再生。
			PlayAnimation(AnimationAttack_00);
			//攻撃中のフラグを立てる。
			attackFlag = 1;
		}
		//Question 2 Yボタンが押されたら中攻撃アニメーションを再生する。
		if (Pad(0).IsPress(enButtonY)) {
			//攻撃中じゃないときにYボタンが押されたら中攻撃。
			//攻撃アニメーションを再生。
			PlayAnimation(AnimationAttack_01);
			//攻撃中のフラグを立てる。
			attackFlag = 1;
		}
		//Question 3 Bボタンが押されたら大攻撃アニメーションを再生する。
		if (Pad(0).IsPress(enButtonB)) {
			//攻撃中じゃないときにYボタンが押されたら大攻撃。
			//攻撃アニメーションを再生。
			PlayAnimation(AnimationAttack_02);
			//攻撃中のフラグを立てる。
			attackFlag = 1;
		}
	}
}