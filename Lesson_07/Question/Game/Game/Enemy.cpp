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
	
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;
	moveSpeed.y -= 9.8f * GameTime().GetFrameDeltaTime();

	UpdateFSM();

	characterController.Execute(moveSpeed, GameTime().GetFrameDeltaTime());
	position = characterController.GetPosition();
	
}
/////////////////////////////////////////////////////////////////////////
// �������烂���X�^�[��FSM����������Ă���B
/////////////////////////////////////////////////////////////////////////
void Enemy::UpdateFSM()
{
	
	//�v���C���[�Ƃ̋����𒲂ׂ�B
	CVector3 playerPos = player->GetPosition();
	//�v���C���[�Ƃ̋��������l�ȉ��ɂȂ�����U������B
	CVector3 diff = player->GetPosition();
	diff = playerPos - position;
	
	if (state == State_Idle) {
		//�ҋ@��ԁB
		//������2m�ȉ��Ȃ�U����ԂɑJ�ځB
		if (diff.Length() < 2.0f) {
			animation.PlayAnimation(AnimationAttack, 0.2f);
			state = State_Attack;	//�U����ԂɑJ�ځB
		}
		else if (diff.Length() < 5.0f) {
			animation.PlayAnimation(AnimationWalk, 0.2f);
			state = State_Chase;
		}
	}
	else if (state == State_Chase)
	{
		//�ǐՒ��B
		//Question1 �v���C���[�̕��Ɍ������Đi�ށB
		//(�q���g)
		//  �v���C���[�̕��ɐi�ނƂ����������s�����߂ɂ́A�v���C���[�̕����������Ă���A�����x�N�g�������߂�K�v������B
		//  diff�ɂ̓G�l�~�[����v���C���[�܂ł̃x�N�g���������Ă���B
		//  ���̃x�N�g���̑傫�����P�ɂ����(���K��)�����x�N�g���ɂȂ�B
		
		//Question 2 �v���C���[�Ƃ̋�����2.0m�ȉ��ɂȂ�����U�����s���B
		
		
		
		
		//Question 3 �v���C���[�ƓG�̋�����10m�ȏ�ɂȂ�����A�ǐՏI���őҋ@��Ԃɖ߂��E
		
		
		
		
		
	}
	else if (state == State_Attack) {
		//�U����
		if (animation.IsPlay() == false) {
			//�U���A�j���[�V�������I������̂őҋ@�A�j���[�V�������Đ�����B
			animation.PlayAnimation(AnimationStand, 0.2f);
			state = State_Idle;
		}
	}
}