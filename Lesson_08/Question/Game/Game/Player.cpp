/*!
 *@brief	プレイヤー。
 */
#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "CollisionWorld.h"
#include "tkEngine/Sound/tkSoundSource.h"

Player* player = nullptr;

namespace {
	class EmitterAttackCollision : public IGameObject {
	public:
		float timer = 0.0f;
		CVector3 emitPos;
		void Update() override
		{
			timer -= GameTime().GetFrameDeltaTime();
			if (timer < 0.0f) {
				//攻撃コリジョンを発生させる。
				collisionWorld->Add(emitPos, 0.5f, 0.5f, enCollisionAttr_PlayerAttack);
				CSoundSource* s = NewGO<CSoundSource>(0);
				s->Init("Assets/titleBgm.wav");
				s->Play(false);
				DeleteGO(this);
			}
		}
	};
}
Player::Player()
{
}
Player::~Player()
{
}
//攻撃コリジョンをエミット。
//pos エミットする位置。
//delayTime エミットまでにかかる時間。
void Player::EmitAttackCollision(CVector3 pos, float delayTime)
{
	EmitterAttackCollision* emitAttackColl = NewGO<EmitterAttackCollision>(0);
	emitAttackColl->emitPos = pos;
	emitAttackColl->timer = delayTime;
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
	animation.SetAnimationLoopFlag(AnimationDamage, false);
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
	//ダメージ判定。
	CheckDamage();
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
	if (state == State_Attack || state == State_Damage) {
		//攻撃中ならリターン。
		return;
	}
	const float fMoveSpeed = 4.0f;
	moveSpeed.y -= 9.8f * GameTime().GetFrameDeltaTime();
	if (Pad(0).IsPress(enButtonRight)) {     //もしもゲームパッドの右ボタンが押されていたら
		moveSpeed.x = fMoveSpeed;				     //移動速度を1にする。
	}
	else if (Pad(0).IsPress(enButtonLeft)) { //右のボタンが押されていなくて、左のボタンが押されているなら。
		moveSpeed.x = -fMoveSpeed;                 //移動速度を-1にする
	}
	else {
		moveSpeed.x = 0.0f;                  //何も入力されていないので移動速度を０にする。
	}
	if (Pad(0).IsPress(enButtonUp)) {
		moveSpeed.z = fMoveSpeed;
	}
	else if (Pad(0).IsPress(enButtonDown)) {
		moveSpeed.z = -fMoveSpeed;
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
	if (state == State_Attack || state == State_Damage) {
		//攻撃中ならリターン。
		return;
	}
	
	if (Pad(0).IsPress(enButtonRight)) {     //もしもゲームパッドの右ボタンが押されていたら
		rotation.SetRotationDeg(CVector3::AxisY, 90.0f);
	}
	
	if (Pad(0).IsPress(enButtonLeft)) {     //もしもゲームパッドの左ボタンが押されていたら
		rotation.SetRotationDeg(CVector3::AxisY, -90.0f);
	}
	
	if (Pad(0).IsPress(enButtonUp)) {     //もしもゲームパッドの上ボタンが押されていたら
		rotation.SetRotationDeg(CVector3::AxisY, 0.0f);
	}
	
	if (Pad(0).IsPress(enButtonDown)) {     //もしもゲームパッドの上ボタンが押されていたら
		rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	}
}
//////////////////////////////////////////////////////////////////////
// ここからプレイヤーのアニメーションを制御するプログラムが記述されてるよ。
//////////////////////////////////////////////////////////////////////
void Player::AnimationControl()
{
	if (state != State_Jump) {
		
		if (Pad(0).IsTrigger(enButtonA)) {
			PlayAnimation(AnimationJump);
			state = State_Jump;
		}
	}
	if (state != State_Jump && state != State_Attack) {
		//攻撃判定。
		if (Pad(0).IsPress(enButtonX)) {
			//攻撃中じゃないときにXボタンが押されたら小攻撃・
			//攻撃アニメーションを再生。
			PlayAnimation(AnimationAttack_00);
			//攻撃中のフラグを立てる。
			state = State_Attack;
			//攻撃コリジョンを発生させる。
			EmitAttackCollision(position, 0.2f);
		}
		if (Pad(0).IsPress(enButtonY)) {
			//攻撃中じゃないときにYボタンが押されたら中攻撃。
			//攻撃アニメーションを再生。
			PlayAnimation(AnimationAttack_01);
			//攻撃中のフラグを立てる。
			state = State_Attack;
			//Question 1 攻撃コリジョンを発生させる。
			
		}
		if (Pad(0).IsPress(enButtonB)) {
			//攻撃中じゃないときにYボタンが押されたら大攻撃。
			//攻撃アニメーションを再生。
			PlayAnimation(AnimationAttack_02);
			//攻撃中のフラグを立てる。
			state = State_Attack;
			//Question 2 攻撃コリジョンを発生させる。
			
		}
	}
	if (state == State_Idle) {	//
		if (Pad(0).IsPress(enButtonUp)) {
			PlayAnimation(AnimationRun);
			state = State_Run;
		}
		else if (Pad(0).IsPress(enButtonDown)) {
			PlayAnimation(AnimationRun);
			state = State_Run;
		}
		else if (Pad(0).IsPress(enButtonRight)) {
			PlayAnimation(AnimationRun);
			state = State_Run;
		}
		else if (Pad(0).IsPress(enButtonLeft)) {
			PlayAnimation(AnimationRun);
			state = State_Run;
		}
		
	}
	else if (state == State_Run) {
		if (!Pad(0).IsPress(enButtonUp)
			&& !Pad(0).IsPress(enButtonDown)
			&& !Pad(0).IsPress(enButtonLeft)
			&& !Pad(0).IsPress(enButtonRight)
		) {
			//待機状態に遷移
			PlayAnimation(AnimationStand);
			state = State_Idle;
		}
	}
	else if (state == State_Jump) {
		if (animation.IsPlay() == false) {
			//待機状態に遷移
			PlayAnimation(AnimationStand);
			state = State_Idle;
		}
	}
	else if (state == State_Attack) {
		if (!animation.IsPlay()) {
			PlayAnimation(AnimationStand);
			state = State_Idle;
		}
	}
	else if (state == State_Damage) {
		if (!animation.IsPlay()) {
			PlayAnimation(AnimationStand);
			state = State_Idle;
		}
	}
	
	
}

///////////////////////////////////////////////////////////////
// プレイヤーのダメージ判定。
///////////////////////////////////////////////////////////////
void Player::CheckDamage()
{
	//Question 3 プレイヤーのダメージ判定。
	// ヒント
	//  Enemy.cppの83行目～103行目までのプログラムがエネミーのダメージ判定の処理。
	//  それを参考にすれば実装できるかも？


}