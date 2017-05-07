/*!
 *@brief	Actor
 */

#include "stdafx.h"
#include "Lib/Object3D.h"


CLight			Object3D::defaultLight;

void Object3D::Init(const char* filePath, const CCamera& camera)
{
	defaultLight.SetDiffuseLightDirection(0, CVector3(0.707f, 0.0f, -0.707f));
	defaultLight.SetDiffuseLightDirection(1, CVector3(-0.707f, 0.0f, -0.707f));
	defaultLight.SetDiffuseLightDirection(2, CVector3(0.0f, 0.707f, 0.707f));
	defaultLight.SetDiffuseLightDirection(3, CVector3(0.0f, -0.707f, 0.0f));


	defaultLight.SetDiffuseLightColor(0, CVector4(2.0f, 2.0f, 2.0f, 10.0f));
	defaultLight.SetDiffuseLightColor(1, CVector4(0.8f, 0.8f, 0.8f, 1.0f));
	defaultLight.SetDiffuseLightColor(2, CVector4(0.8f, 0.8f, 0.8f, 1.0f));
	defaultLight.SetDiffuseLightColor(3, CVector4(0.8f, 0.8f, 0.8f, 1.0f));


	defaultLight.SetLimLightColor(CVector4(2.0f, 2.0f, 2.0f, 1.0f));
	defaultLight.SetLimLightDirection(CVector3(0.0f, 0.0f, -1.0f));

	skinModelData.LoadModelData(filePath, &animation);
	skinModel.Init(skinModelData.GetBody());
	skinModel.SetLight(&defaultLight);
	skinModel.SetShadowCasterFlag(true);
	skinModel.SetShadowReceiverFlag(true);
	this->camera = &camera;
}
void Object3D::Update()
{
	animation.Update(GameTime().GetFrameDeltaTime());
	skinModel.Update(position, rotation, scale);
}
void Object3D::Render(CRenderContext& renderContext)
{
	if (camera != nullptr) {
		skinModel.Draw(renderContext, camera->GetViewMatrix(), camera->GetProjectionMatrix());
	}
	else {
		TK_LOG("warning camera is null!!!");
	}
}


