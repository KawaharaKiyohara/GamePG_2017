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

	//�}�e���A���������B
	CSkinModelMaterial* mat = skinModelData.GetBody()->FindMaterial("Skeleton_D.png");
	if (mat != nullptr) {
		//���������B
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
	if (isAttackFlag == false) {	//�U�����ł͂Ȃ����
		//Question 1 �v���C���[�Ƃ̋�����2m�ȉ��ɂȂ�����U���A�j���[�V�������Đ�����B
		CVector3 playerPos = player->GetPosition();
		//�v���C���[�Ƃ̋��������l�ȉ��ɂȂ�����U������B
		CVector3 diff = player->GetPosition();
		diff = playerPos - position;
		if (diff.Length() < 2.0f) {
			//2m�ȉ��ɂȂ�����
			animation.PlayAnimation(AnimationAttack);
			isAttackFlag = true;	//�U���t���O�𗧂Ă�B
		}
	}
	if(isAttackFlag == true){
		//Question 2 �U���A�j���[�V�����̍Đ����I�������ҋ@�A�j���[�V�������Đ�����B
		//�U����
		if (animation.IsPlay() == false) {
			//�U���A�j���[�V�������I������̂őҋ@�A�j���[�V�������Đ�����B
			animation.PlayAnimation(AnimationStand);
			isAttackFlag = false;
		}
	}
}