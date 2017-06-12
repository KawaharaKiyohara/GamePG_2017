#pragma once
class Sky : public IGameObject
{
public:
	///////////////////////////////////////////////////////
	//ここからメンバ関数。
	///////////////////////////////////////////////////////
	Sky();
	~Sky();
	bool Start() override;
	void Update() override;
	void Render(CRenderContext& rc) override;
	///////////////////////////////////////////////////////
	//ここからメンバ変数。
	///////////////////////////////////////////////////////
	CLight					m_light;
	CSkinModelDataHandle	m_skinModelData;						//!<スキンモデルデータ。
	CSkinModel				m_skinModel;							//!<スキンモデル。
	CAnimation				m_animation;							//!<アニメーション。 
	CVector3				m_position = CVector3::Zero;			//!<座標。
	CQuaternion				m_rotation = CQuaternion::Identity;		//!<回転。
};

