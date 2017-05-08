/*!
 *@brief	3Dオブジェクトの基底クラス。
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
protected:
	CSkinModelDataHandle	skinModelData;		//!<スキンモデルデータ。
	CSkinModel				skinModel;			//!<スキンモデル。
	CAnimation				animation;			//!<アニメーション。
	CVector3				position = CVector3::Zero;			//!<座標。
	CQuaternion				rotation = CQuaternion::Identity;	//!<回転。
	CVector3				scale = CVector3::One;				//!<拡大率。
	static CLight			defaultLight;						//!<デフォルトライト。
	const CCamera*			camera = nullptr;					//!<カメラ。
};
