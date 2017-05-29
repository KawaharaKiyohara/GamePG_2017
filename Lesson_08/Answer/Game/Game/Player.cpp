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
	const float fMoveSpeed = 4.0f;
	moveSpeed.y -= 9.8f * GameTime().GetFrameDeltaTime();
	if (Pad(0).IsPress(enButtonRight)) {     //�������Q�[���p�b�h�̉E�{�^����������Ă�����
		moveSpeed.x = fMoveSpeed;				     //�ړ����x��1�ɂ���B
	}
	else if (Pad(0).IsPress(enButtonLeft)) { //�E�̃{�^����������Ă��Ȃ��āA���̃{�^����������Ă���Ȃ�B
		moveSpeed.x = -fMoveSpeed;                 //�ړ����x��-1�ɂ���
	}
	else {
		moveSpeed.x = 0.0f;                  //�������͂���Ă��Ȃ��̂ňړ����x���O�ɂ���B
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
	if (attackFlag == 1) {
		//�U�����Ȃ烊�^�[���B
		return;
	}
	
	if (Pad(0).IsPress(enButtonRight)) {     //�������Q�[���p�b�h�̉E�{�^����������Ă�����
		rotation.SetRotationDeg(CVector3::AxisY, 90.0f);
	}
	
	if (Pad(0).IsPress(enButtonLeft)) {     //�������Q�[���p�b�h�̍��{�^����������Ă�����
		rotation.SetRotationDeg(CVector3::AxisY, -90.0f);
	}
	
	if (Pad(0).IsPress(enButtonUp)) {     //�������Q�[���p�b�h�̏�{�^����������Ă�����
		rotation.SetRotationDeg(CVector3::AxisY, 0.0f);
	}
	
	if (Pad(0).IsPress(enButtonDown)) {     //�������Q�[���p�b�h�̏�{�^����������Ă�����
		rotation.SetRotationDeg(CVector3::AxisY, 180.0f);
	}
}
//////////////////////////////////////////////////////////////////////
// ��������v���C���[�̃A�j���[�V�����𐧌䂷��v���O�������L�q����Ă��B
//////////////////////////////////////////////////////////////////////
void Player::AnimationControl()
{
	if (attackFlag == 0) {	//�U��������Ȃ���΁B
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
		if (Pad(0).IsTrigger(enButtonA)) {
			PlayAnimation(AnimationJump);
		}
	}
	AttackAnimationControl();
}
//////////////////////////////////////////////////////////////////////
// ��������v���C���[�̍U���A�j���[�V�����𐧌䂷��v���O�������L�q����Ă����B
//////////////////////////////////////////////////////////////////////
void Player::AttackAnimationControl()
{
	if (attackFlag == 1) {
		//�U�����I
		if (!animation.IsPlay()) {
			//�A�j���[�V�����̍Đ����I������̂ōU���t���O�������B
			attackFlag = 0;
		}
	}
	else {
		//�U��������Ȃ��I
		if (Pad(0).IsPress(enButtonX)) {
			//�U��������Ȃ��Ƃ���X�{�^���������ꂽ�珬�U���E
			//�U���A�j���[�V�������Đ��B
			PlayAnimation(AnimationAttack_00);
			//�U�����̃t���O�𗧂Ă�B
			attackFlag = 1;
		}
		if (Pad(0).IsPress(enButtonY)) {
			//�U��������Ȃ��Ƃ���Y�{�^���������ꂽ�璆�U���B
			//�U���A�j���[�V�������Đ��B
			PlayAnimation(AnimationAttack_01);
			//�U�����̃t���O�𗧂Ă�B
			attackFlag = 1;
		}
		if (Pad(0).IsPress(enButtonB)) {
			//�U��������Ȃ��Ƃ���Y�{�^���������ꂽ���U���B
			//�U���A�j���[�V�������Đ��B
			PlayAnimation(AnimationAttack_02);
			//�U�����̃t���O�𗧂Ă�B
			attackFlag = 1;
		}
	}
}