#pragma once
#include <GameEngineCore/GAMEENGINERENDERTARGET.H>

// Ό³Έν :
class PlayerEffect : public Effect
{
public:
	// constrcuter destructer
	PlayerEffect();
	~PlayerEffect();

	// delete Function
	PlayerEffect(const PlayerEffect& _Other) = delete;
	PlayerEffect(PlayerEffect&& _Other) noexcept = delete;
	PlayerEffect& operator=(const PlayerEffect& _Other) = delete;
	PlayerEffect& operator=(PlayerEffect&& _Other) noexcept = delete;

protected:

private:
	void Start() override;
	void EffectProcess(float _DeltaTime) override;
};

