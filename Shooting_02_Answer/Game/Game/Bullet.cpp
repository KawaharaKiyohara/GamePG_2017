#include "stdafx.h"
#include "Bullet.h"

Bullet::Bullet()
{
}


Bullet::~Bullet()
{
}
///////////////////////////////////////////////
//�C���X�^���X�𐶐����Ĉ�x�����Ă΂�鏈���B
///////////////////////////////////////////////
bool Bullet::Start()
{
	//3D���f���f�[�^�����[�h���ď������B
	m_skinModelData.LoadModelData("Assets/modelData/bullet.x", NULL);
	m_skinModel.Init(m_skinModelData.GetBody());
	//���C�g��ݒ�B
	m_skinModel.SetLight(g_defaultLight);
	//���t���[��FindGO����̂͏d���̂ŁA�������̎��ɃQ�[���J�������������Ă����B
	m_gameCamera = FindGO<GameCamera>("GameCamera");
	return true;
}
///////////////////////////////////////////////
//���t���[���Ă΂��X�V�����B
//���W�̈ړ��Ƃ��͂����ɏ����Ƃ�����B
///////////////////////////////////////////////
void Bullet::Update()
{
	m_position += m_moveSpeed;
	//�e�̃��f��������������̂Ŋg�嗦��30�{��ݒ肷��B
	CVector3 scale;
	scale.x = 30.0f;
	scale.y = 30.0f;
	scale.z = 30.0f;
	m_skinModel.Update(m_position, CQuaternion::Identity, scale);
}
///////////////////////////////////////////////
//���t���[���Ă΂��`�揈���B
//���f���̕`�揈���B
///////////////////////////////////////////////
void Bullet::Render(CRenderContext& rc)
{
	m_skinModel.Draw(
		rc, 
		m_gameCamera->m_camera.GetViewMatrix(), 
		m_gameCamera->m_camera.GetProjectionMatrix()
	);
}
