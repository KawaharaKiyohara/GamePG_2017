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

	CVector3 lightPos = position;
	lightPos.y += 2.0f;
	lightPos.z += 1.0f;
	ShadowMap().SetLightPosition(lightPos);
	CVector3 lightTarget = position;
	ShadowMap().SetLightTarget(lightTarget);
}

void Player::Move()
{
	moveSpeed.y -= 0.1f;
	if (Pad(0).IsPress(enButtonRight)) {     //もしもゲームパッドの右ボタンが押されていたら
		moveSpeed.x = 1.0f;				     //移動速度を1にする。
	}
	else if (Pad(0).IsPress(enButtonLeft)) { //右のボタンが押されていなくて、左のボタンが押されているなら。
		moveSpeed.x = -1.0f;                 //移動速度を-1にする
	}
	else {
		moveSpeed.x = 0.0f;                  //何も入力されていないので移動速度を０にする。
	}
	if (Pad(0).IsPress(enButtonUp)) {
		moveSpeed.z = 1.0f;
	}
	else if (Pad(0).IsPress(enButtonDown)) {
		moveSpeed.z = -1.0f;
	}
	else {
		moveSpeed.z = 0.0f;
	}	
	if (Pad(0).IsTrigger(enButtonA)) { //Aボタンが押されたら
		//ジャンプ
		moveSpeed.y = 2.0;
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
	//Question 1 キャラクタを右に向かせてみよう。
	//Question 2 キャラクタを左に向かせてみよう。
	//Question 3 キャラクタを奥に向かせてみよう。
	//Question 4 キャラクタを手前に向かせてみよう。
}