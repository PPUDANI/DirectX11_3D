// App.cpp : �� ���Ͽ��� 'main' �Լ��� ���Ե˴ϴ�. �ű⼭ ���α׷� ������ ���۵ǰ� ����˴ϴ�.
#include <GameEngineContents/PreCompile.h>

#pragma comment(lib, "GameEngineBase.lib")
#pragma comment(lib, "GameEnginePlatform.lib")
#pragma comment(lib, "GameEngineCore.lib")
#pragma comment(lib, "GameEngineContents.lib")

#include <iostream>
#include <Windows.h>
#include <memory>
#include <GameEngineCore/GameEngineCore.h>
#include <GameEngineContents/ContentsCore.h>


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPWSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	// std::string Text = GameEngineString::Format("{}, {:.2f}", "aaaa", 2.222222f);

	GameEngineCore::EngineStart<ContentsCore>(hInstance);
}

