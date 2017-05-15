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
	animation.SetAnimationLoopFlag(AnimationAttack_00, false);
	animation.SetAnimationLoopFlag(AnimationAttack_01, false);
	animation.SetAnimationLoopFlag(AnimationAttack_02, false);
	animation.SetAnimationEndTime(AnimationAttack_00, 0.6f);
	animation.SetAnimationEndTime(AnimationAttack_01, 0.8f);
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
	if (attackFlag == 1) {
		//�U�����Ȃ烊�^�[���B
		return;
	}
	//Question 1 �p�b�h�̓��͂ŃL�����N�^�[���ړ������Ă݂悤�B
	//Question 2 �p�b�h��A�{�^��(�L�[�{�[�h��J)�������ꂽ��L�����N�^�[���W�����v�����Ă݂悤�B
	//�ړ��B
	characterController.Execute(moveSpeed, GameTime().GetFrameDeltaTime());
	position = characterController.GetPosition();
}
//////////////////////////////////////////////////////////////////////
// ��������v���C���[����]����v���O�������L�q����Ă��B
//////////////////////////////////////////////////////////////////////
void Player::Rotation()
{
	if (attackFlag == 1) {
		//�U�����Ȃ烊�^�[���B
		return;
	}
	//Question 3 �p�b�h�̓��͂ŃL�����N�^�[�̕�����ς�����悤�ɂ��Ă݂悤�B
}
//////////////////////////////////////////////////////////////////////
// ��������v���C���[�̃A�j���[�V�����𐧌䂷��v���O�������L�q����Ă��B
//////////////////////////////////////////////////////////////////////
void Player::AnimationControl()
{
	if (attackFlag == 0) {	//�U��������Ȃ���΁B
		//QUestion 4 �p�b�h�̓��͂ő���A�j���[�V�������Đ����Ă݂悤�B
		//Question 5 A�{�^��(�L�[�{�[�h��J)�������ꂽ��W�����v�A�j���[�V�������Đ����Ă݂悤�B
	}
	AttackAnimationControl();
}
//////////////////////////////////////////////////////////////////////
// ��������v���C���[�̍U���A�j���[�V�����𐧌䂷��v���O�������L�q����Ă����B
//////////////////////////////////////////////////////////////////////
void Player::AttackAnimationControl()
{
}