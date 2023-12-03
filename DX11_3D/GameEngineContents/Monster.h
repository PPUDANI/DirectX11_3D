#pragma once
#include <GameEngineCore/GameEngineActor.h>
#include <GameEngineBase/GameEngineSerializer.h>

// Ό³Έν :
class Monster : public GameEngineActor, public GameEngineSerializerObject
{
public:
	// constrcuter destructer
	Monster();
	~Monster();

	// delete Function
	Monster(const Monster& _Other) = delete;
	Monster(Monster&& _Other) noexcept = delete;
	Monster& operator=(const Monster& _Other) = delete;
	Monster& operator=(Monster&& _Other) noexcept = delete;

	std::shared_ptr<GameEngineSpriteRenderer> Renderer;
	std::shared_ptr<GameEngineCollision> Col;

	void Serializer(GameEngineSerializer& _Data) override;
	void DeSerializer(GameEngineSerializer& _Data) override;

protected:
	void Start() override;

private:
};

