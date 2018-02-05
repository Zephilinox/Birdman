#pragma once
class Scene
{
	friend class Play;

	struct SceneLink
	{
		Scene* scene = nullptr;
		std::string stage_description = "a dead end...";
	};

public:
	std::string getName() const;
	std::string getDescription() const;

private:
	std::string name;
	std::string stage_description;

	SceneLink light;
	SceneLink dark;
	SceneLink shocking;
	SceneLink sad;
};