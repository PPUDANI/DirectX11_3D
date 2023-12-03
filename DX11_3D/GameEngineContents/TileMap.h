#pragma once

// Ό³Έν :
class TileMap : public GameEngineActor
{
public:
	// constrcuter destructer
	TileMap();
	~TileMap();

	// delete Function
	TileMap(const TileMap& _Other) = delete;
	TileMap(TileMap&& _Other) noexcept = delete;
	TileMap& operator=(const TileMap& _Other) = delete;
	TileMap& operator=(TileMap&& _Other) noexcept = delete;

	std::shared_ptr<GameEngineTileMap> TileRenderer;

protected:
	void Start() override;

private:
};

