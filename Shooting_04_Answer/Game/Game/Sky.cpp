#include "stdafx.h"
#include "Sky.h"
#include "GameCamera.h"

Sky::Sky()
{
}


Sky::~Sky()
{
}
bool Sky::Start()
{
	m_skinModelData.LoadModelData("Assets/modelData/sky.x", &m_animation);
	m_skinModel.Init(m_skinModelData.GetBody());
	m_light.SetAmbinetLight({ 1.0f, 1.0f, 1.0f });
	m_skinModel.SetLight(m_light);
	CSkinModelMaterial* mat = m_skinModelData.GetBody()->FindMaterial("sky.png");
	if (mat) {
		mat->SetTexture(CSkinModelMaterial::enTextureShaderHandle_SkyCubeMap, *TextureResources().Load("Assets/modelData/skyCubeMap.dds", true));
		mat->Build(CSkinModelMaterial::enTypeSky);
	}
	return true;
}
void Sky::Update()
{
	m_skinModel.Update(CVector3::Zero, CQuaternion::Identity, CVector3::One);
}
void Sky::Render(CRenderContext& rc)
{
	GameCamera* gcam = FindGO<GameCamera>("GameCamera");
	m_skinModel.Draw(rc, gcam->m_camera.GetViewMatrix(), gcam->m_camera.GetProjectionMatrix());
}