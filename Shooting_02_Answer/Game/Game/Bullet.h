#pragma once
#include "GameCamera.h"

class Bullet : public IGameObject
{
public:
	///////////////////////////////////////////////////////
	//�������烁���o�֐��B
	///////////////////////////////////////////////////////
	Bullet();
	~Bullet();
	bool Start() override;
	void Update() override;
	void Render(CRenderContext& rc) override;
	///////////////////////////////////////////////////////
	//�������烁���o�ϐ��B
	///////////////////////////////////////////////////////
	CSkinModelDataHandle	m_skinModelData;		//!<�X�L�����f���f�[�^�B
	CSkinModel				m_skinModel;			//!<�X�L�����f���B
	CVector3				m_position = CVector3::Zero;	//!<���W�B
	CVector3				m_moveSpeed = CVector3::Zero;	//!<�ړ����x�B
	GameCamera*				m_gameCamera = NULL;			//!<�Q�[���J�����B
	int						m_life = 60;					//!<�����B
};

