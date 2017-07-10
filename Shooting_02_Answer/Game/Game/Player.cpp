#include "stdafx.h"
#include "Player.h"
#include "GameCamera.h"
#include "Bullet.h"

Player::Player()
{
}
Player::~Player()
{
}
///////////////////////////////////////////////
//�C���X�^���X�𐶐����Ĉ�x�����Ă΂�鏈���B
///////////////////////////////////////////////
bool Player::Start()
{
	//3D���f���f�[�^�����[�h���ď������B
	m_skinModelData.LoadModelData("Assets/modelData/enemy.X", &m_animation);
	m_skinModel.Init(m_skinModelData.GetBody());
	m_skinModel.SetLight(g_defaultLight);
	return true;
}
///////////////////////////////////////////////
//���t���[���Ă΂��X�V�����B
//���W�̈ړ��Ƃ��͂����ɏ����Ƃ�����B
///////////////////////////////////////////////
void Player::Update()
{
	//�ړ��������Ăяo���B
	Move();
	//���f���̃��[���h�s����X�V�B
	m_skinModel.Update(m_position, m_rotation, CVector3::One);
	//�e�ۂ̔��ˏ������L�q�B
	//���ˊԊu���o�߂����B
	if (m_interval == 0) {
		if (Pad(0).IsPress(enButtonA)) {
			Bullet* bullet = NewGO<Bullet>(0);
			bullet->m_position = m_position;
			bullet->m_moveSpeed.x = 0.0f;
			bullet->m_moveSpeed.y = 0.0f;
			bullet->m_moveSpeed.z = 10.0f;
			m_interval = 5;
		}
	}
	else {
		m_interval -= 1;
	}
}
///////////////////////////////////////////////
//���t���[���Ă΂��`�揈���B
//���f���̕`�揈���B
///////////////////////////////////////////////
void Player::Render(CRenderContext& rc)
{
	//GameCamera�������B
	GameCamera* gcam = FindGO<GameCamera>("GameCamera");
	m_skinModel.Draw(rc, gcam->m_camera.GetViewMatrix(), gcam->m_camera.GetProjectionMatrix());
}
///////////////////////////////////////////////
//�ړ������B
///////////////////////////////////////////////
void Player::Move()
{
	if (Pad(0).IsPress(enButtonLeft)) {
		m_position.x += 1.5f;
		//Z�������-10.0�����񂷂�B
		m_rotation.SetRotationDeg(CVector3::AxisZ, -10.0f);
	}
	if (Pad(0).IsPress(enButtonRight)) {
		m_position.x -= 1.5f;
		//Z�������10.0�����񂷂�B
		m_rotation.SetRotationDeg(CVector3::AxisZ, 10.0f);
	}

	if (Pad(0).IsPress(enButtonUp)) {
		m_position.y += 1.5f;
		//Z�������-10.0�����񂷂�B
		m_rotation.SetRotationDeg(CVector3::AxisX, 40.0f);
	}
	if (Pad(0).IsPress(enButtonDown)) {
		m_position.y -= 1.5f;
		m_rotation.SetRotationDeg(CVector3::AxisX, -40.0f);
	}
	//������m_position.x��150�����傫���Ȃ�����B
	if (m_position.x > 150.0f) {
		//150��������B
		m_position.x = 150.0f;
	}
	//������m_position.x��150�����������Ȃ�����B
	if (m_position.x < -150.0f) {
		//-150��������B
		m_position.x = -150.0f;
	}
	//������m_position.y��120�����傫���Ȃ�����B
	if (m_position.y > 120.0f) {
		//120��������B
		m_position.y = 120.0f;
	}
	//������m_position.y��0�����������Ȃ�����B
	if (m_position.y < 0.0f) {
		//0��������B
		m_position.y = 0.0f;
	}
}
