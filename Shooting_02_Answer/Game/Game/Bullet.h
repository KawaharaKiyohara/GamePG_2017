#pragma once
#include "GameCamera.h"

class Bullet : public IGameObject
{
public:
	///////////////////////////////////////////////////////
	//ここからメンバ関数。
	///////////////////////////////////////////////////////
	Bullet();
	~Bullet();
	bool Start() override;
	void Update() override;
	void Render(CRenderContext& rc) override;
	///////////////////////////////////////////////////////
	//ここからメンバ変数。
	///////////////////////////////////////////////////////
	CSkinModelDataHandle	m_skinModelData;		//!<スキンモデルデータ。
	CSkinModel				m_skinModel;			//!<スキンモデル。
	CVector3				m_position = CVector3::Zero;	//!<座標。
	CVector3				m_moveSpeed = CVector3::Zero;	//!<移動速度。
	GameCamera*				m_gameCamera = NULL;			//!<ゲームカメラ。
	int						m_life = 60;					//!<寿命。
};

