/*!
 *@brief	3D�I�u�W�F�N�g�̊��N���X�B
 */

#pragma once

class Object3D : public IGameObject{
public:
	Object3D()
	{
	}
	virtual ~Object3D()
	{
	}
	void Init(const char* filePath, const CCamera& camera);
	void Update() override;
	void Render(CRenderContext& renderContext) override;
	const CVector3& GetPosition() const
	{
		return position;
	}
	void SetPosition(const CVector3& pos)
	{
		position = pos;
	}
	void SetRotation(const CQuaternion& rot)
	{
		rotation = rot;
	}
protected:
	CSkinModelDataHandle	skinModelData;		//!<�X�L�����f���f�[�^�B
	CSkinModel				skinModel;			//!<�X�L�����f���B
	CAnimation				animation;			//!<�A�j���[�V�����B
	CVector3				position = CVector3::Zero;			//!<���W�B
	CQuaternion				rotation = CQuaternion::Identity;	//!<��]�B
	CVector3				scale = CVector3::One;				//!<�g�嗦�B
	static CLight			defaultLight;						//!<�f�t�H���g���C�g�B
	const CCamera*			camera = nullptr;					//!<�J�����B
};
