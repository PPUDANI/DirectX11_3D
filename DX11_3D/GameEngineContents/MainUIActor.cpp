#include "PreCompile.h"
#include "MainUIActor.h"

MainUIActor::MainUIActor() 
{
}

MainUIActor::~MainUIActor() 
{
}

void MainUIActor::Start()
{
	{
		// MainUI¾×ÅÍ °°
		HpBar = CreateComponent<GameEngineUIRenderer>(30);
		HpBar->Transform.SetLocalPosition({400, 300});
		HpBar->SetImageScale({ 300.0f, 30.0f });
	}
}
