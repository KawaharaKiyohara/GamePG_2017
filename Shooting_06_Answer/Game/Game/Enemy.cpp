#include "stdafx.h"
#include "Enemy.h"
#include "GameCamera.h"
#include "Bullet.h"
#include "tkEngine/sound/tkSoundSource.h"

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
	
	//�w�肵�����O�̃Q�[���I�u�W�F�N�g���������āA���������瓽�����\�b�h�̏��������s����B
	FindGO<Bullet>(
		"Bullet",
		//���������Ƃ��̏����B
		[&](Bullet* bullet) {
			CVector3 diff = m_position - bullet->m_position;
			if ( diff.Length() < 20.0f) {
				//���S�B
				CSoundSource* s = NewGO<CSoundSource>(0);
				s->Init("Assets/sound/explosion.wav");
				s->Play(false);
				DeleteGO(this);
			}
		}
	);
	m_interval++;
	if (m_interval == 50) {
		//�e�ۂ𐶐��B
		Bullet* bullet = NewGO<Bullet>(0, "EnemyBullet");
		bullet->m_moveSpeed.x = 0.0f;
		bullet->m_moveSpeed.y = 0.0f;
		bullet->m_moveSpeed.z = -5.0f;
		bullet->m_position = m_position;
		m_interval = 0;
	}
}
void Enemy::Render(CRenderContext& rc)
{
	//GameCamera�������B
	GameCamera* gcam = FindGO<GameCamera>("GameCamera");
	m_skinModel.Draw(rc, gcam->m_camera.GetViewMatrix(), gcam->m_camera.GetProjectionMatrix());
}
