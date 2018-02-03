#pragma once

//Class implemented as a singleton for safer access and no risk of multiple instances
//(much like a director)
class SceneManager
{
	SceneManager() = default;

public:
	static SceneManager* getSceneManager();

	int getCurrentScene() { return scene_id; }
	void setCurrentScene(int new_scene);

private:

	int scene_id = 0;
};
