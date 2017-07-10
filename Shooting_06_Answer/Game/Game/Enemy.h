#pragma once
class Enemy : public IGameObject
{
public:
	///////////////////////////////////////////////////////
	//�������烁���o�֐��B
	///////////////////////////////////////////////////////
	Enemy();
	~Enemy();
	bool Start() override;
	void Update() override;
	void Render(CRenderContext& rc) override;
	///////////////////////////////////////////////////////
	//�������烁���o�ϐ��B
	///////////////////////////////////////////////////////
	CSkinModelDataHandle	m_skinModelData;						//!<�X�L�����f���f�[�^�B
	CSkinModel				m_skinModel;							//!<�X�L�����f���B
	CAnimation				m_animation;							//!<�A�j���[�V�����B 
	CVector3				m_position = CVector3::Zero;			//!<���W�B
	CQuaternion				m_rotation = CQuaternion::Identity;		//!<��]�B
	int						m_interval = 0;
};

