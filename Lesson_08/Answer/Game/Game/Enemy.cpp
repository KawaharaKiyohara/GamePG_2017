#include "stdafx.h"
#include "Enemy.h"
#include "GameCamera.h"
#include "Player.h"
#include "CollisionWorld.h"

namespace {
	class EmitterAttackCollision : public IGameObject {

	public:
		float timer = 0.0f;
		CVector3 emitPos;
		Enemy* enemy = nullptr;
		void Update() override
		{
			if (enemy->IsDead() || enemy->state != Enemy::State_Attack) {
				DeleteGO(this);
			}
			timer -= GameTime().GetFrameDeltaTime();
			if (timer < 0.0f) {
				//�U���R���W�����𔭐�������B
				collisionWorld->Add(emitPos, 0.5f, 0.5f, enCollisionAttr_EnemyAttack);
				DeleteGO(this);
			}
		}
	};
}

Enemy::Enemy()
{
}


Enemy::~Enemy()
{
}
//�U���R���W�������G�~�b�g�B
//pos �G�~�b�g����ʒu�B
//delayTime �G�~�b�g�܂łɂ����鎞�ԁB
void Enemy::EmitAttackCollision(CVector3 pos, float delayTime)
{
	EmitterAttackCollision* emitAttackColl = NewGO<EmitterAttackCollision>(0);
	emitAttackColl->emitPos = pos;
	emitAttackColl->timer = delayTime;
	emitAttackColl->enemy = this;
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
	animation.SetAnimationLoopFlag(AnimationDamage, false);
	return true;
}
void Enemy::Update()
{
	Object3D::Update();
	
	moveSpeed.x = 0.0f;
	moveSpeed.z = 0.0f;
	moveSpeed.y -= 9.8f * GameTime().GetFrameDeltaTime();

	UpdateFSM();
	CheckDamage();
	characterController.Execute(moveSpeed, GameTime().GetFrameDeltaTime());
	position = characterController.GetPosition();
	//����
	Turn();
	
}
//���񏈗��B
void Enemy::Turn()
{
	//�����X�^�[����񂳂���B
	float angle = atan2f(direction.x, direction.z);
	rotation.SetRotation(CVector3::AxisY, angle);
}
//�_���[�W����B
void Enemy::CheckDamage()
{
	if (state == State_Damage) {
		//�_���[�W���Ȃ烊�^�[���B
		return;
	}
	//�v���C���[�̍U���R���W�����Ƃ̂����蔻����s���B
	int numCollision = collisionWorld->m_collisionList.size();
	for (int i = 0; i < numCollision; i++) {
		if (collisionWorld->m_collisionList[i]->attr == enCollisionAttr_PlayerAttack) {
			//�v���C���[�������������U���R���W�����Ƃ̂�����𒲂ׂ�B
			//�R���W�����Ƃ̋����𒲂ׂ�B
			CVector3 diff = collisionWorld->m_collisionList[i]->pos - position;
			if (diff.Length() < 3.0f) {
				//�_���[�W���󂯂�B
				animation.PlayAnimation(AnimationDamage, 0.2f);
				state = State_Damage;
			}
		}
	}
}
void Enemy::UpdateFSM()
{
	//�v���C���[�Ƃ̋��������l�ȉ��ɂȂ�����U������B
	CVector3 diff;
	diff = player->position - position;
	
	
	if (state == State_Idle) {
		//�ҋ@��ԁB
		//������2m�ȉ��Ȃ�U����ԂɑJ�ځB
		if (diff.Length() < 2.0f) {
			animation.PlayAnimation(AnimationAttack, 0.2f);
			EmitAttackCollision(position, 1.0f);
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
		//�v���C���[�̕��ɐi�ނƂ����������s�����߂ɂ́A�v���C���[�̕����������Ă���A�����x�N�g�������߂�K�v������B
		//diff�ɂ̓G�l�~�[����v���C���[�܂ł̃x�N�g���������Ă���B
		//���̃x�N�g���̑傫�����P�ɂ����(���K��)�����x�N�g���ɂȂ�B
		CVector3 toPlayerDirection = diff;
		toPlayerDirection.Normalize();
		moveSpeed = toPlayerDirection * 2.0f;
		if(moveSpeed.Length() > 0.01f) {
			//�i�s�����������B
			direction = moveSpeed;
			direction.Normalize();
		}
		
		if (diff.Length() < 2.0f) {
			//�v���C���[�Ƃ̋�����2m�ȉ��ɂȂ����̂ōU�����s���B
			animation.PlayAnimation(AnimationAttack, 0.2f);
			EmitAttackCollision(position, 1.0f);
			state = State_Attack;	//�U����ԂɑJ�ځB
		}
		

		if (diff.Length() > 10.0f) {
			//�v���C���[�Ƃ̋��������l�ȏ�ɂȂ����̂ŒǐՂ�������߂đҋ@��ԂɑJ�ڂ���B
			animation.PlayAnimation(AnimationStand, 0.2f);
			state = State_Idle;
		}
	}
	else if (state == State_Attack) {
		//�U����
		CVector3 toPlayerDirection = diff;
		toPlayerDirection.Normalize();
		direction = toPlayerDirection;
		direction.Normalize();
		
		if (animation.IsPlay() == false) {
			//�_���[�W�A�j���[�V�������I�������̂őҋ@��ԂɑJ�ڂ���B
			animation.PlayAnimation(AnimationStand, 0.2f);
			state = State_Idle;
		}
	}
	else if (state == State_Damage) {
		//�_���[�W���󂯂Ă���B
		if (animation.IsPlay() == false) {
			//�_���[�W�A�j���[�V�������I�������̂őҋ@��ԂɑJ�ڂ���B
			animation.PlayAnimation(AnimationStand, 0.2f);
			state = State_Idle;
		}
	}
}