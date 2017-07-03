#pragma once
class Ground : public IGameObject
{
public:
	///////////////////////////////////////////////////////
	//ここからメンバ関数。
	///////////////////////////////////////////////////////
	Ground();
	~Ground();
	bool Start() override;
	void Update() override;
	void Render(CRenderContext& rc) override;
	///////////////////////////////////////////////////////
	//ここからメンバ変数。
	///////////////////////////////////////////////////////
	CSkinModelDataHandle	m_skinModelData;						//!<スキンモデルデータ。
	CSkinModel				m_skinModel;							//!<スキンモデル。
	CAnimation				m_animation;							//!<アニメーション。 
	CVector3				m_position = CVector3::Zero;			//!<座標。
	CQuaternion				m_rotation = CQuaternion::Identity;		//!<回転。
};

