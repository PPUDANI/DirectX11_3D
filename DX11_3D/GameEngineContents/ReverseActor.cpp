#include "PreCompile.h"
#include "ReverseActor.h"

ReverseActor::ReverseActor() 
{
}

ReverseActor::~ReverseActor() 
{
}

void ReverseActor::ReverseUpdate(float _DeltaTime)
{
	ActorInfo.push_back({ 0.0f, Transform.GetWorldPosition()});

	for (int i = 0; i < static_cast<int>(Renderers.size()); i++)
	{
		std::string_view SpriteName = Renderers[i]->GetSprite()->GetName();
		int Frame = Renderers[i]->GetCurIndex();
		RendererInfo.push_back({0.0f, i, SpriteName, Frame });
	}
}

void ReverseActor::Reverse()
{
	if (ActorInfo.size() != RendererInfo.size() / Renderers.size())
	{
		MsgBoxAssert("if (ActorInfo.size() != RendererInfo.size() / Renderers.size())");
	}

	ReverseActorInfo& Info = ActorInfo.back();
	Transform.SetWorldPosition(Info.Pos);
	ActorInfo.pop_back();

	// 중간에 랜더러수가 바뀐다거나하는 
	for (int i = 0; i < static_cast<int>(Renderers.size()); i++)
	{
		std::shared_ptr<GameEngineSpriteRenderer> Renderer = Renderers[i];
		ReverseRendererInfo& Info = RendererInfo.back();
		Renderer->SetSprite(Info.SpriteName, Info.Frame);
	}
}