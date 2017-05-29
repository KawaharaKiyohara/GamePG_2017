/*!
 *@brief	�v���C���[�B
 */
#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "CollisionWorld.h"

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
				//�U���R���W�����𔭐�������B
				collisionWorld->Add(emitPos, 0.5f, 0.5f, enCollisionAttr_PlayerAttack);
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
//�U���R���W�������G�~�b�g�B
//pos �G�~�b�g����ʒu�B
//delayTime �G�~�b�g�܂łɂ����鎞�ԁB
void Player::EmitAttackCollision(CVector3 pos, float delayTime)
{
	EmitterAttackCollision* emitAttackColl = NewGO<EmitterAttackCollision>(0);
	emitAttackColl->emitPos = pos;
	emitAttackColl->timer = delayTime;
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
	animation.SetAnimationLoopFlag(AnimationDamage, false);
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
	//�_���[�W����B
	CheckDamage();
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
	if (state == State_Attack || state == State_Damage) {
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
	if (state == State_Attack || state == State_Damage) {
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
	if (state != State_Jump) {
		
		if (Pad(0).IsTrigger(enButtonA)) {
			PlayAnimation(AnimationJump);
			state = State_Jump;
		}
	}
	if (state != State_Jump && state != State_Attack) {
		//�U������B
		if (Pad(0).IsPress(enButtonX)) {
			//�U��������Ȃ��Ƃ���X�{�^���������ꂽ�珬�U���E
			//�U���A�j���[�V�������Đ��B
			PlayAnimation(AnimationAttack_00);
			//�U�����̃t���O�𗧂Ă�B
			state = State_Attack;
			//�U���R���W�����𔭐�������B
			EmitAttackCollision(position, 0.2f);
		}
		if (Pad(0).IsPress(enButtonY)) {
			//�U��������Ȃ��Ƃ���Y�{�^���������ꂽ�璆�U���B
			//�U���A�j���[�V�������Đ��B
			PlayAnimation(AnimationAttack_01);
			//�U�����̃t���O�𗧂Ă�B
			state = State_Attack;
			//Question 1 �U���R���W�����𔭐�������B
			
		}
		if (Pad(0).IsPress(enButtonB)) {
			//�U��������Ȃ��Ƃ���Y�{�^���������ꂽ���U���B
			//�U���A�j���[�V�������Đ��B
			PlayAnimation(AnimationAttack_02);
			//�U�����̃t���O�𗧂Ă�B
			state = State_Attack;
			//Question 2 �U���R���W�����𔭐�������B
			
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
			//�ҋ@��ԂɑJ��
			PlayAnimation(AnimationStand);
			state = State_Idle;
		}
	}
	else if (state == State_Jump) {
		if (animation.IsPlay() == false) {
			//�ҋ@��ԂɑJ��
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
// �v���C���[�̃_���[�W����B
///////////////////////////////////////////////////////////////
void Player::CheckDamage()
{
	//Question 3 �v���C���[�̃_���[�W����B
	// �q���g
	//  Enemy.cpp��83�s�ځ`103�s�ڂ܂ł̃v���O�������G�l�~�[�̃_���[�W����̏����B
	//  ������Q�l�ɂ���Ύ����ł��邩���H


}