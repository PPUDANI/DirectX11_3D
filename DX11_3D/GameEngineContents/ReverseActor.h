#pragma once
#include <GameEngineCore/GameEngineActor.h>

class ReverseActorInfo
{
public:
	float Time;
	float4 Pos;
};

// 중간에 랜더러가 삭제되고 
class ReverseRendererInfo 
{
public:
	float Time;
	// 내가 몇번째 랜더러의 
	int RendererIndex = -1;
	std::string_view SpriteName;
	int Frame;
};

// 설명 :
class ReverseActor : public GameEngineActor
{
	// 시간이 알맞지 않다면 리턴시켜버리거나.
	// static float ReverseTime = 0.05f;

public:
	// constrcuter destructer
	ReverseActor();
	~ReverseActor();

	// delete Function
	ReverseActor(const ReverseActor& _Other) = delete;
	ReverseActor(ReverseActor&& _Other) noexcept = delete;
	ReverseActor& operator=(const ReverseActor& _Other) = delete;
	ReverseActor& operator=(ReverseActor&& _Other) noexcept = delete;

protected:
	void Reverse();
	void ReverseUpdate(float _DeltaTime);

private:
	// 이게 true면 
	bool IsReverse = false;
	std::vector<std::shared_ptr<GameEngineSpriteRenderer>> Renderers;

	std::list<ReverseActorInfo> ActorInfo;
	std::list<ReverseRendererInfo> RendererInfo;
};

