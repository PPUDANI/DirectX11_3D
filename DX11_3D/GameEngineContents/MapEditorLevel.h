#pragma once
#include <GameEngineCore/GameEngineLevel.h>
#include <GameEngineCore/GameEngineGUI.h>

// Ό³Έν :
class MapEditorLevel : public GameEngineLevel
{
public:
	// constrcuter destructer
	MapEditorLevel();
	~MapEditorLevel();

	// delete Function
	MapEditorLevel(const MapEditorLevel& _Other) = delete;
	MapEditorLevel(MapEditorLevel&& _Other) noexcept = delete;
	MapEditorLevel& operator=(const MapEditorLevel& _Other) = delete;
	MapEditorLevel& operator=(MapEditorLevel&& _Other) noexcept = delete;

	std::shared_ptr<GameEngineActor> BackGroundActor;
	std::shared_ptr<GameEngineSpriteRenderer> BackGroundRenderer;

protected:
	void Start() override;

	void Update(float _Delta) override;

	void LevelStart(GameEngineLevel* _PrevLevel) override;
	void LevelEnd(GameEngineLevel* _NextLevel) override;

private:
	
};

