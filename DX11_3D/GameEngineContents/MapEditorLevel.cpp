#include "PreCompile.h"
#include "MapEditorLevel.h"
#include "Monster.h"


MapEditorLevel::MapEditorLevel() 
{
}

MapEditorLevel::~MapEditorLevel() 
{
}

void MapEditorLevel::Start()
{
	float4 HalfWindowScale = GameEngineCore::MainWindow.GetScale().Half();
	GetMainCamera()->Transform.SetLocalPosition({ HalfWindowScale.X, -HalfWindowScale.Y, 0.0f });
	GetMainCamera()->SetProjectionType(EPROJECTIONTYPE::Orthographic);

	{
		BackGroundActor = CreateActor<GameEngineActor>();
		BackGroundRenderer = BackGroundActor->CreateComponent<GameEngineSpriteRenderer>();
	}

	GameEngineInput::AddInputObject(this);
}

void MapEditorLevel::Update(float _Delta)
{
	if (GameEngineInput::IsDown(VK_LBUTTON, this))
	{
		std::shared_ptr<Monster> Object = CreateActor<Monster>(ContentsObjectType::Monster);
		Object->Transform.SetLocalPosition(GetMainCamera()->GetWorldMousePos2D());
	}

	float MoveSpeed = 200;

	if (GameEngineInput::IsPress('A', this))
	{
		GetMainCamera()->Transform.AddLocalPosition(float4::LEFT * _Delta * MoveSpeed);
	}


	if (GameEngineInput::IsPress('D', this))
	{
		GetMainCamera()->Transform.AddLocalPosition(float4::RIGHT * _Delta * MoveSpeed);
	}

	if (GameEngineInput::IsPress('W', this))
	{
		GetMainCamera()->Transform.AddLocalPosition(float4::UP * _Delta * MoveSpeed);
	}


	if (GameEngineInput::IsPress('S', this))
	{
		GetMainCamera()->Transform.AddLocalPosition(float4::DOWN * _Delta * MoveSpeed);
	}
}

void MapEditorLevel::LevelStart(GameEngineLevel* _PrevLevel)
{

}
void MapEditorLevel::LevelEnd(GameEngineLevel* _NextLevel)
{

}
