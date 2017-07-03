#include "stdafx.h"
#include "Enemy.h"
#include "GameCamera.h"
#include "Bullet.h"

Enemy::Enemy()
{
}
Enemy::~Enemy()
{
}
bool Enemy::Start() 
{
	//3D���f���f�[�^�����[�h���ď������B
	m_skinModelData.LoadModelData("Assets/modelData/enemy.X", &m_animation);
	m_skinModel.Init(m_skinModelData.GetBody());
	m_skinModel.SetLight(g_defaultLight);
	return true;
}
void Enemy::Update()
{
	//���f���̃��[���h�s����X�V�B
	m_skinModel.Update(m_position, m_rotation, CVector3::One);
	
	//�w�肵�����O�̃Q�[���I�u�W�F�N�g���������āA���������烉���_���̏��������s����B
	FindGO<Bullet>(
		"Bullet",
		//���������Ƃ��̏����B
		[&](Bullet* bullet) {
			CVector3 diff = m_position - bullet->m_position;
			if (diff.Length() < 20.0f) {
				//���S�B
				DeleteGO(this);
			}
		}
	);
}
void Enemy::Render(CRenderContext& rc)
{
	//GameCamera�������B
	GameCamera* gcam = FindGO<GameCamera>("GameCamera");
	m_skinModel.Draw(rc, gcam->m_camera.GetViewMatrix(), gcam->m_camera.GetProjectionMatrix());
}
