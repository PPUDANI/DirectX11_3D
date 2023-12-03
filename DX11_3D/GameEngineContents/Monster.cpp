#include "PreCompile.h"
#include "Monster.h"

Monster::Monster() 
{
}

Monster::~Monster() 
{
}


void Monster::Start()
{
	GameEngineRandom NewRandom;
	NewRandom.SetSeed(reinterpret_cast<long long>(this));
	float4 Scale = NewRandom.RandomVectorBox2D(10, 100, 10, 100);

	{
		Renderer = CreateComponent<GameEngineSpriteRenderer>(30);
		Renderer->SetSprite("HoHoYee_Smoke");
		Renderer->SetImageScale(Scale);
	}

	{
		Col = CreateComponent<GameEngineCollision>(ContentsCollisionType::Monster);
		Col->Transform.SetLocalScale(Scale);
	}
}


void Monster::Serializer(GameEngineSerializer& _Data)
{
	{
		const TransformData& Data = Transform.GetConstTransformDataRef();
		_Data.Write(&Data, sizeof(Data));
	}

	{
		const TransformData& Data = Renderer->Transform.GetConstTransformDataRef();
		_Data.Write(&Data, sizeof(Data));
	}

	{
		const TransformData& Data = Col->Transform.GetConstTransformDataRef();
		_Data.Write(&Data, sizeof(Data));
	}

}

void Monster::DeSerializer(GameEngineSerializer& _Data)
{
	TransformData Data;

	{
		_Data.Read(&Data, sizeof(Data));
		Transform.SetTransformData(Data);
	}

	{
		_Data.Read(&Data, sizeof(Data));
		Renderer->Transform.SetTransformData(Data);
	}

	{
		_Data.Read(&Data, sizeof(Data));
		Col->Transform.SetTransformData(Data);
	}

}