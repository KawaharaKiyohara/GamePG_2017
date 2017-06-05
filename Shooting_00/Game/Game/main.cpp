#include "stdafx.h"
#include "GameCamera.h"
#include "Sky.h"

CLight g_defaultLight;
/*!
 * @brief	tkEngineの初期化。
 */
void InitTkEngine( HINSTANCE hInst )
{
	SInitParam initParam;
	memset(&initParam, 0, sizeof(initParam));
	//コマンドバッファのサイズのテーブル。
	int commandBufferSizeTbl[] = {
		10 * 1024 * 1024,		//10MB
	};
	initParam.hInstance = hInst;
	initParam.gameObjectPrioMax = 255;
	initParam.numRenderContext = 1;	//レンダリングコンテキストは一本
	initParam.commandBufferSizeTbl = commandBufferSizeTbl;
	initParam.screenHeight = 720;
	initParam.screenWidth = 1280;
	initParam.frameBufferHeight = 720;
	initParam.frameBufferWidth = 1280;
	//Bloom
	initParam.graphicsConfig.bloomConfig.isEnable = true;
	initParam.graphicsConfig.edgeRenderConfig.isEnable = false;
	initParam.graphicsConfig.edgeRenderConfig.idMapWidth = initParam.frameBufferWidth;
	initParam.graphicsConfig.edgeRenderConfig.idMapHeight = initParam.frameBufferHeight;
	//Shadow
	initParam.graphicsConfig.shadowRenderConfig.Init();
	initParam.graphicsConfig.shadowRenderConfig.isEnable = true;
	initParam.graphicsConfig.shadowRenderConfig.shadowMapWidth = 1024;
	initParam.graphicsConfig.shadowRenderConfig.shadowMapHeight = 1024;
	initParam.graphicsConfig.shadowRenderConfig.numShadowMap = 3;
	
	//reflection
	initParam.graphicsConfig.reflectionMapConfig.isEnable = false;
	initParam.graphicsConfig.reflectionMapConfig.reflectionMapWidth = 512;
	initParam.graphicsConfig.reflectionMapConfig.reflectionMapHeight = 512;
	//DOF
	initParam.graphicsConfig.dofConfig.isEnable = false;
	//AA
	initParam.graphicsConfig.aaConfig.isEnable = true;
	//Tonemap
	initParam.graphicsConfig.tonemapConfig.isEnable = false;
	Engine().Init(initParam);	//初期化。
	
	ShadowMap().SetNear(2.0f);
	ShadowMap().SetFar(40.0f);

	g_defaultLight.SetDiffuseLightDirection(0, CVector3(0.707f, 0.0f, -0.707f));
	g_defaultLight.SetDiffuseLightDirection(1, CVector3(-0.707f, 0.0f, -0.707f));
	g_defaultLight.SetDiffuseLightDirection(2, CVector3(0.0f, 0.707f, 0.707f));
	g_defaultLight.SetDiffuseLightDirection(3, CVector3(0.0f, -0.707f, 0.0f));


	g_defaultLight.SetDiffuseLightColor(0, CVector4(0.3f, 0.3f, 0.3f, 1.0f));
	g_defaultLight.SetDiffuseLightColor(1, CVector4(0.3f, 0.3f, 0.3f, 1.0f));
	g_defaultLight.SetDiffuseLightColor(2, CVector4(0.3f, 0.3f, 0.3f, 1.0f));
	g_defaultLight.SetDiffuseLightColor(3, CVector4(0.3f, 0.3f, 0.3f, 1.0f));

	g_defaultLight.SetAmbinetLight({ 0.2f, 0.2f, 0.2f });
	g_defaultLight.SetLimLightColor(CVector4(0.0f, 0.0f, 0.0f, 1.0f));
	g_defaultLight.SetLimLightDirection(CVector3(0.0f, 0.0f, -1.0f));

	
}

int WINAPI wWinMain(
	HINSTANCE hInst,
	HINSTANCE hPrevInstance,
	LPWSTR lpCmdLine,
	int nCmdShow
	)
{
	//tkEngineの初期化。
	InitTkEngine( hInst );
	//空を追加。
	NewGO<::Sky>(0, "Sky");
	//ゲームカメラを追加。
	GameCamera* cam = NewGO<GameCamera>(0, "GameCamera");
	ShadowMap().SetCamera(cam->m_camera);

	Engine().RunGameLoop();		//ゲームループを実行。

	return 0;
}