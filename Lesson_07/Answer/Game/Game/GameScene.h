/*!
 *@brief	�Q�[���V�[��
 */

#pragma once

class GameScene : public IGameObject{
public:
	GameScene();
	~GameScene();
private:
	bool Start() override;
	void Update() override;
};