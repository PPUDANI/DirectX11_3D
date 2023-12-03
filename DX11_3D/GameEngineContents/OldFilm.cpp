#include "PreCompile.h"
#include "OldFilm.h"
#include <GameEngineCore/GAMEENGINECORE.H>

OldFilm::OldFilm() 
{
}

OldFilm::~OldFilm() 
{
}

void OldFilm::Start()
{
	std::shared_ptr<GameEngineRenderTarget> Target = GameEngineCore::GetCurLevel()->GetMainCamera()->GetCameraAllRenderTarget();

	if (nullptr != Target)
	{
		ResultTarget = Target->CreateChildRenderTarget({0});
	}

	EffectUnit.SetMesh("fullrect");
	EffectUnit.SetMaterial("OldFilm");

	FilmSprite = GameEngineSprite::Find("OldFilmPart");

	EffectUnit.ShaderResHelper.SetSampler("Tex0Sampler", "POINT");
	//EffectUnit.ShaderResHelper.SetSampler("Tex1Sampler", "POINT");


}

void OldFilm::EffectProcess(float _DeltaTime)
{
	ResultTarget->Setting();

	EffectUnit.ShaderResHelper.SetTexture("Tex0", FilmSprite->GetSpriteData(CurIndex).Texture);

	++CurIndex;

	if (CurIndex >= static_cast<int>(FilmSprite->GetSpriteCount()))
	{
		CurIndex = 0;
	}

	EffectUnit.Render();
	EffectUnit.ShaderResHelper.AllShaderResourcesReset();

	GameEngineRenderTarget::RenderTargetReset();
}