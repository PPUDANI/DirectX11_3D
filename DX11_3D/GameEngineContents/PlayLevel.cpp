#include "PreCompile.h"
#include "PlayLevel.h"
#include "Player.h"
#include "PlayMap.h"
#include "Monster.h"
#include "TileMap.h"
#include "MainUIActor.h"
#include <GameEngineCore/GameEngineCoreWindow.h>
#include <GameEngineCore/FadePostEffect.h>
#include "PlayerEffect.h"
#include "OldFilm.h"
#include <GameEngineCore/BlurPostEffect.h>

PlayLevel::PlayLevel() 
{
}

PlayLevel::~PlayLevel() 
{
}

void PlayLevel::Start()
{
	GameEngineInput::AddInputObject(this);

	GameEngineCore::GetBackBufferRenderTarget()->SetClearColor({0, 0, 0, 1});

	CoreWindow = GameEngineGUI::FindGUIWindow<GameEngineCoreWindow>("GameEngineCoreWindow");

	if (nullptr != CoreWindow)
	{
		CoreWindow->AddDebugRenderTarget(0, "PlayLevelRenderTarget", GetMainCamera()->GetCameraAllRenderTarget());
	}

	float4 HalfWindowScale = GameEngineCore::MainWindow.GetScale().Half();

	GetMainCamera()->Transform.SetLocalPosition({ HalfWindowScale.X, -HalfWindowScale.Y, 0.0f});
	GetMainCamera()->SetProjectionType(EPROJECTIONTYPE::Orthographic);

	{
		std::shared_ptr<Player> Object = CreateActor<Player>(0, "Player");
		PlayerObject = Object;
	}


	//{
	//	std::shared_ptr<PlayMap> Object = CreateActor<PlayMap>(0, "Map");
	//	Map = Object;
	//}

	//{
	//	std::shared_ptr<GameEngineActor> NewTest = CreateActor<GameEngineActor>(3, "Test");
	//	float4 HalfWindowScale = GameEngineCore::MainWindow.GetScale().Half();
	//	NewTest->Transform.SetLocalPosition({ HalfWindowScale.X, -HalfWindowScale.Y, -100.0f });
	//	std::shared_ptr<GameEngineRenderer> NewRender = NewTest->CreateComponent<GameEngineSpriteRenderer>(5);
	//	NewRender->SetRenderOrder(5);
	//	NewRender->RenderBaseInfoValue.Target2 = 1;

	//	// 대규학생이 해야할것.
	//	// 순서가
	//	// GetMainCamera()->GetCameraAllRenderTarget()->CreateEffect();
	//}

	//{
	//	// GetMainCamera()->GetCameraAllRenderTarget()->CreateEffect<PlayerEffect>();
	//	GetMainCamera()->GetCameraAllRenderTarget()->CreateEffect<OldFilm>();
	//	GetMainCamera()->GetCameraAllRenderTarget()->CreateEffect<BlurPostEffect>();
	//	GetMainCamera()->GetCameraAllRenderTarget()->CreateEffect<BlurPostEffect>();
	//}


	//{
	//	std::shared_ptr<MainUIActor> Object = CreateActor<MainUIActor>(ContentsObjectType::UI);
	//}

	//{
	//	GameEngineRandom NewRanadom;
	//	for (size_t i = 0; i < 10; i++)
	//	{
	//		std::shared_ptr<Monster> Object = CreateActor<Monster>(ContentsObjectType::Monster);
	//		Object->Transform.SetLocalPosition(NewRanadom.RandomVectorBox2D(0, 1280, 0, -720));
	//	}
	//}


	//{
	//	std::shared_ptr<TileMap> Object = CreateActor<TileMap>(ContentsObjectType::BackGround);

	//	size_t TileX = 300;
	//	size_t TileY = 300;

	//	Object->TileRenderer->CreateTileMap({ TileX, TileY, {32, 32}, "HoHoYee_AttackABC" });

	//	for (size_t y = 0; y < TileY; y++)
	//	{
	//		for (size_t x = 0; x < TileX; x++)
	//		{
	//			Object->TileRenderer->SetTileIndex({y, x});
	//		}
	//	}

	//	TileMapObject = Object;
	//}
}

void PlayLevel::Update(float _Delta)
{
	if (true == GameEngineInput::IsDown(VK_F7, this))
	{
		CoreWindow->OnOffSwitch();
	}
}

void PlayLevel::LevelStart(GameEngineLevel* _PrevLevel)
{
	int a = 0;
}

void PlayLevel::LevelEnd(GameEngineLevel* _NextLevel)
{
	int a = 0;
}
