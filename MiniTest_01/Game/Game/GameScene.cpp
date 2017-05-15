/*!
 *@brief	ゲームシーン。
 */

#include "stdafx.h"
#include "GameScene.h"
#include "GameCamera.h"
#include "Player.h"
#include "Ground.h"

GameScene::GameScene()
{
}
GameScene::~GameScene()
{
}
bool GameScene::Start()
{
	gameCaemra = NewGO<GameCamera>(0);
	player = NewGO<Player>(0);
	return true;
}
void GameScene::Update()
{
}