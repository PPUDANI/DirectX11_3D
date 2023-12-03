#pragma once
#include <GameEngineCore/GameEngineActor.h>

// Ό³Έν :
class MainUIActor : public GameEngineActor
{
public:
	// constrcuter destructer
	MainUIActor();
	~MainUIActor();

	// delete Function
	MainUIActor(const MainUIActor& _Other) = delete;
	MainUIActor(MainUIActor&& _Other) noexcept = delete;
	MainUIActor& operator=(const MainUIActor& _Other) = delete;
	MainUIActor& operator=(MainUIActor&& _Other) noexcept = delete;

protected:
	void Start() override;

private:
	std::shared_ptr<class GameEngineUIRenderer> HpBar;

};

