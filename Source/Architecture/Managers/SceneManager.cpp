#include "SceneManager.h"


SceneManager * SceneManager::getSceneManager()
{
	static SceneManager* inst = nullptr;

	if(!inst)
	{
		inst = new SceneManager();
	}
	return inst;
}

void SceneManager::setCurrentScene(int new_scene)
{
	scene_id = new_scene;

	//TODO transition scenes here?
}