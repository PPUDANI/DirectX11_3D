#include "PreCompile.h"
#include "PlayerEffect.h"
#include <GameEngineCore/GAMEENGINECORE.H>

PlayerEffect::PlayerEffect() 
{
}

PlayerEffect::~PlayerEffect() 
{
}

void PlayerEffect::Start()
{
	std::shared_ptr<GameEngineRenderTarget> Target = GameEngineCore::GetCurLevel()->GetMainCamera()->GetCameraAllRenderTarget();

	if (nullptr != Target)
	{
		ResultTarget = Target->CreateChildRenderTarget({0});
	}

	EffectUnit.SetMesh("fullrect");
	EffectUnit.SetMaterial("PlayerEffect");

	EffectUnit.ShaderResHelper.SetTexture("Tex0", Target->GetTexture(1));
	EffectUnit.ShaderResHelper.SetTexture("Tex1", Target->GetTexture(2));

	EffectUnit.ShaderResHelper.SetSampler("Tex0Sampler", "POINT");
	EffectUnit.ShaderResHelper.SetSampler("Tex1Sampler", "POINT");


}

void PlayerEffect::EffectProcess(float _DeltaTime)
{
	ResultTarget->Setting();
	EffectUnit.Render();
	EffectUnit.ShaderResHelper.AllShaderResourcesReset();

	GameEngineRenderTarget::RenderTargetReset();
}