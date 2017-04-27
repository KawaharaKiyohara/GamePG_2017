/*!
 *@brief	�v���C���[�B
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
	//�v���C���[���������B
	Init("Assets/modelData/Player.X", gameCamera->GetCamera());
	//�L�����N�^�[�R���g���[���[���������B
	characterController.Init(0.6f, 0.3f, position);

	//�}�e���A���������B
	CSkinModelMaterial* mat = skinModelData.GetBody()->FindMaterial("Thethief_D.tga");
	if (mat != nullptr) {
		//���������B
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_NormalMap, *TextureResources().Load("Assets/modelData/Thethief_N.tga"));
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_SpecularMap, *TextureResources().Load("Assets/modelData/Thethief_S.tga"));
	}
	mat = skinModelData.GetBody()->FindMaterial("Thethief_wuqi_D.tga");
	if (mat != nullptr) {
		//���������B
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_NormalMap, *TextureResources().Load("Assets/modelData/Thethief_wuqi_N.tga"));
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_SpecularMap, *TextureResources().Load("Assets/modelData/Thethief_wuqi_S.tga"));
	}
	//�@���}�b�v�A�X�y�L�����}�b�v�A�t���l������ݒ肷��B
	skinModel.SetHasNormalMap(true);
	skinModel.SetHasSpeculerMap(true);
	skinModel.SetFresnelFlag(true);
	PlayAnimation(AnimationStand);
	animation.SetAnimationLoopFlag(AnimationJump, false);
	return true;
}
void Player::Update()
{
	//���N���X��Update���Ăяo���B
	Object3D::Update();
	//�v���C���[�𓮂����B
	Move();
	//�v���C���[���񂷁B
	Rotation();
	//�A�j���[�V��������B
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
// ��������v���C���[���ړ�������v���O�������L�q����Ă��B
//////////////////////////////////////////////////////////////////////
void Player::Move()
{
	moveSpeed.y -= 9.8f * GameTime().GetFrameDeltaTime();
	if (Pad(0).IsPress(enButtonRight)) {     //�������Q�[���p�b�h�̉E�{�^����������Ă�����
		moveSpeed.x = 2.0f;				     //�ړ����x��1�ɂ���B
	}
	else if (Pad(0).IsPress(enButtonLeft)) { //�E�̃{�^����������Ă��Ȃ��āA���̃{�^����������Ă���Ȃ�B
		moveSpeed.x = -2.0f;                 //�ړ����x��-1�ɂ���
	}
	else {
		moveSpeed.x = 0.0f;                  //�������͂���Ă��Ȃ��̂ňړ����x���O�ɂ���B
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
	if (Pad(0).IsTrigger(enButtonA)) { //A�{�^���������ꂽ��
		//�W�����v
		moveSpeed.y = 3.0;
		characterController.Jump();
	}
	//�ړ��B
	characterController.Execute(moveSpeed, GameTime().GetFrameDeltaTime());
	position = characterController.GetPosition();
}
//////////////////////////////////////////////////////////////////////
// ��������v���C���[����]����v���O�������L�q����Ă��B
//////////////////////////////////////////////////////////////////////
void Player::Rotation()
{
	//Question 1 �L�����N�^���E�Ɍ������Ă݂悤�B
	if (Pad(0).IsPress(enButtonRight)) {     //�������Q�[���p�b�h�̉E�{�^����������Ă�����
		rotation.SetRotationDeg(CVector3::AxisY, 90.0f);
	}
	//Question 2 �L�����N�^�����Ɍ������Ă݂悤�B
	if (Pad(0).IsPress(enButtonLeft)) {     //�������Q�[���p�b�h�̍��{�^����������Ă�����
		rotation.SetRotationDeg(CVector3::AxisY, -90.0f);
	}
	//Question 3 �L�����N�^�����Ɍ������Ă݂悤�B
	if (Pad(0).IsPress(enButtonUp)) {     //�������Q�[���p�b�h�̏�{�^����������Ă�����
		rotation.SetRotationDeg(CVector3::AxisY, 0.0f);
	}
	//Question 4 �L�����N�^����O�Ɍ������Ă݂悤�B
	if (Pad(0).IsPress(enButtonDown)) {     //�������Q�[���p�b�h�̏�{�^����������Ă�����
		rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	}
}
//////////////////////////////////////////////////////////////////////
// ��������v���C���[�̃A�j���[�V�����𐧌䂷��v���O�������L�q����Ă��B
//////////////////////////////////////////////////////////////////////
void Player::AnimationControl()
{
	//Question 1�@����A�j���[�V�������Đ����Ă݂悤�B
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
	//Question 2 �W�����v�A�j���[�V�������Đ����Ă݂悤�B
	if (Pad(0).IsTrigger(enButtonA)) {
		PlayAnimation(AnimationJump);
	}
}