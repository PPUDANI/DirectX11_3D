#pragma once
#include <GameEngineCore/GAMEENGINERENDERTARGET.H>

// Ό³Έν :
class OldFilm : public Effect
{
public:
	// constrcuter destructer
	OldFilm();
	~OldFilm();

	// delete Function
	OldFilm(const OldFilm& _Other) = delete;
	OldFilm(OldFilm&& _Other) noexcept = delete;
	OldFilm& operator=(const OldFilm& _Other) = delete;
	OldFilm& operator=(OldFilm&& _Other) noexcept = delete;

protected:

private:
	void Start() override;
	void EffectProcess(float _DeltaTime) override;

	int CurIndex = 0;

	std::shared_ptr<GameEngineSprite> FilmSprite;
};

