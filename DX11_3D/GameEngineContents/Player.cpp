#include "PreCompile.h"
#include "Player.h"
#include "PlayMap.h"
#include "Monster.h"
#include "ContentsEnum.h"
#include "ContentsSpriteRenderer.h"

Player::Player() 
{	
}

Player::~Player() 
{
}

void Player::Start()
{
	{
		// 부모로 바꿔줘야 
		MainSpriteRenderer = CreateComponent<GameEngineSpriteRenderer>(0);
		MainSpriteRenderer->CreateAnimation("Run", "HoHoYee_Run", 0.1f, -1, -1, true);
		MainSpriteRenderer->CreateAnimation("Idle", "HoHoYee_Standingidle", 0.1f, -1, -1, true);
		MainSpriteRenderer->CreateAnimation("Smoke", "HoHoYee_Smoke", 0.1f, -1, -1, false);

		MainSpriteRenderer->AutoSpriteSizeOn();

		MainSpriteRenderer->RenderBaseInfoValue.Target1 = 1;
	}

	{
		PRenderer = CreateComponent<GameEngineParticleRenderer>(0);
	}

	{
		std::shared_ptr<GameEngineSpriteRenderer> FontRender = CreateComponent<GameEngineSpriteRenderer>(30);
		FontRender->SetText("돋움", "안녕하세요", 50.0f, float4::BLUE, FW1_CENTER);
	}

	{
		Col = CreateComponent<GameEngineCollision>(ContentsCollisionType::Player);
		Col->SetCollisionType(ColType::SPHERE2D);
		Col->Transform.SetLocalPosition({ 0.0f, -50.0f, 0.0f });
		Col->Transform.SetLocalScale({ 100.0f, 100.0f, 1.0f });
	}

	float4 HalfWindowScale = GameEngineCore::MainWindow.GetScale().Half();
	Transform.SetLocalPosition({ HalfWindowScale.X, -HalfWindowScale.Y, 100.0f });

	GameEngineInput::AddInputObject(this);

	// GetLevel()->GetMainCamera()->CameraTargetSetting(Transform, float4::BACKWARD * 500.0f);

	{
		CreateStateParameter NewPara;

		// 한번만 실행되는 겁니다.
		NewPara.Init = [=](class GameEngineState* _Parent)
			{
				// MainSpriteRenderer->ChangeAnimation("Idle");
			};


		NewPara.Start = [=](class GameEngineState* _Parent) 
			{
				MainSpriteRenderer->ChangeAnimation("Idle");
			};

		NewPara.Stay = [=](float _DeltaTime, class GameEngineState* _Parent)
			{
				if (3.0f <= _Parent->GetStateTime())
				{
					_Parent->ChangeState(PlayerState::Smoke);
					return;
				}

				if (GameEngineInput::IsPress('A', this))
				{
					MainSpriteRenderer->LeftFlip();
					_Parent->ChangeState(PlayerState::Move);
				}


				if (GameEngineInput::IsPress('D', this))
				{
					MainSpriteRenderer->RightFlip();
					_Parent->ChangeState(PlayerState::Move);
				}
			};

		PlayerState.CreateState(PlayerState::Idle, NewPara);
	}

	{
		CreateStateParameter NewPara;

		NewPara.Start = [=](class GameEngineState* _Parent)
			{
				MainSpriteRenderer->ChangeAnimation("Smoke");
			};

		NewPara.Stay = [=](float _DeltaTime, class GameEngineState* _Parent)
			{
				if (true == MainSpriteRenderer->IsCurAnimationEnd())
				{
					_Parent->ChangeState(PlayerState::Idle);
					return;
				}

				if (GameEngineInput::IsPress('A', this))
				{
					MainSpriteRenderer->LeftFlip();
					_Parent->ChangeState(PlayerState::Move);
				}


				if (GameEngineInput::IsPress('D', this))
				{
					MainSpriteRenderer->RightFlip();
					_Parent->ChangeState(PlayerState::Move);
				}
			};

		PlayerState.CreateState(PlayerState::Smoke, NewPara);
	}

	{
		CreateStateParameter NewPara;

		NewPara.Start = [=](class GameEngineState* _Parent)
			{
				MainSpriteRenderer->ChangeAnimation("Run");
			};

		NewPara.Stay = [=](float _Delta, class GameEngineState* _Parent)
			{
				if (GameEngineInput::IsFree('A', this) && GameEngineInput::IsFree('D', this))
				{			
					_Parent->ChangeState(PlayerState::Idle);
					return;
				}


				if (GameEngineInput::IsPress('A', this))
				{
					MainSpriteRenderer->LeftFlip();
					Transform.AddLocalPosition(float4::LEFT * _Delta * MoveSpeed);
				}


				if (GameEngineInput::IsPress('D', this))
				{
					MainSpriteRenderer->RightFlip();
					Transform.AddLocalPosition(float4::RIGHT * _Delta * MoveSpeed);
				}
			};


		PlayerState.CreateState(PlayerState::Move, NewPara);
	}

	PlayerState.ChangeState(PlayerState::Idle);

}

void Player::TestEvent(GameEngineRenderer* _Renderer)
{
	// int a = 0;
}

void Player::Update(float _Delta)
{
	GameEngineDebug::DrawLine({ 100, 100 }, { 300, 300 });

	float4 ScreenPos = MainSpriteRenderer->GetScreenPosition();

	// OutputDebugStringA(ScreenPos.ToString().c_str());

	PlayerState.Update(_Delta);

	//float Speed = 100.0f;

	//EventParameter Para;
	//Para.Stay = [](class GameEngineCollision* _This, class GameEngineCollision* _Other)
	//{
	//		_Other->Off();
	//		int a = 0;
	//};

	//Col->CollisionEvent(ContentsCollisionType::Monster, Para);

	//if (GameEngineInput::IsPress('A', this))
	//{
	//	MainSpriteRenderer->LeftFlip();
	//	Transform.AddLocalPosition(float4::LEFT * _Delta * Speed);
	//}


	//if (GameEngineInput::IsPress('D', this))
	//{
	//	MainSpriteRenderer->RightFlip();
	//	Transform.AddLocalPosition(float4::RIGHT * _Delta * Speed);
	//}

	//if (GameEngineInput::IsPress('W', this))
	//{
	//	Transform.AddLocalPosition(float4::UP * _Delta * Speed);
	//}

	//if (GameEngineInput::IsPress('S', this))
	//{
	//	Transform.AddLocalPosition(float4::DOWN * _Delta * Speed);
	//}

	//if (GameEngineInput::IsPress('Q', this))
	//{
	//	GetLevel()->GetMainCamera()->AddZoomValue(-_Delta);
	//	Transform.AddLocalRotation({ 0.0f, 0.0f, 360.0f * _Delta });
	//}

	//if (GameEngineInput::IsPress('E', this))
	//{
	//	GetLevel()->GetMainCamera()->AddZoomValue(+_Delta);
	//	Transform.AddLocalRotation({ 0.0f, 0.0f, -360.0f * _Delta });
	//}
	//
	// float4 WorldMousePos = GetLevel()->GetMainCamera()->GetWorldMousePos2D();
	// OutputDebugStringA(WorldMousePos.ToString("\n").c_str());

	// OutputDebugStringA(GameEngineCore::MainWindow.GetMousePos().ToString("\n").c_str());

	//GameEngineColor Color = PlayMap::MainMap->GetColor(Transform.GetWorldPosition(), GameEngineColor::RED);

	//if (GameEngineColor::RED != Color)
	//{
	//	GrivityForce.Y -= _Delta * 100.0f;
	//	Transform.AddLocalPosition(GrivityForce * _Delta);
	//}
	//else 
	//{
	//	GrivityForce = 0.0f;
	//}
	// 땅에 딱붙게하고 싶다면 while돌려서 올려주세요.


	// float4 Color = GetColor(Transform.GetWorldPosition());
}