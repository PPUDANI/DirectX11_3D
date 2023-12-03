#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class PlayMap : public GameEngineActor
{
public:
	static PlayMap* MainMap;

public:
	// constrcuter destructer
	PlayMap();
	~PlayMap();

	// delete Function
	PlayMap(const PlayMap& _Other) = delete;
	PlayMap(PlayMap&& _Other) noexcept = delete;
	PlayMap& operator=(const PlayMap& _Other) = delete;
	PlayMap& operator=(PlayMap&& _Other) noexcept = delete;

	GameEngineColor GetColor(float4 _Pos, GameEngineColor _DefaultColor = {255, 255, 255, 255});

protected:
	void Start() override;
	void Update(float _DeltaTime) override;

private:
	std::shared_ptr<class GameEngineSpriteRenderer> Renderer;
};

