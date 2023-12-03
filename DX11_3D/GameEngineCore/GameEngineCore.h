#pragma once
#include "GameEngineLevel.h"
#include "GameEngineDevice.h"
#include <GameEnginePlatform/GameEngineWindow.h>

// ���� :
class GameEngineCore
{
	friend class GameEngineLevel;

public:
	static GameEngineTime MainTime;
	static GameEngineWindow MainWindow;

	// constrcuter destructer
	GameEngineCore();
	~GameEngineCore();

	// delete Function
	GameEngineCore(const GameEngineCore& _Other) = delete;
	GameEngineCore(GameEngineCore&& _Other) noexcept = delete;
	GameEngineCore& operator=(const GameEngineCore& _Other) = delete;
	GameEngineCore& operator=(GameEngineCore&& _Other) noexcept = delete;


	// GameEngineCoreObject�� ��ӹ��� Ŭ������ ���ø����� ��� �����ϴ�.
	template<typename ObjectType>
	static void EngineStart(HINSTANCE _Inst) 
	{
		CoreObject = std::make_shared<ObjectType>();
		EngineProcess(_Inst, ObjectType::GetWindowTitle(), ObjectType::GetStartWindowPos(), ObjectType::GetStartWindowSize());
	}


	template<typename LevelType>
	static void CreateLevel(const std::string& _Name)
	{
		std::string Upper = GameEngineString::ToUpperReturn(_Name);

		// �̹� ���ο� TitleLevel�� �����Ѵ�.
		if (AllLevel.end() != AllLevel.find(Upper))
		{
			MsgBoxAssert(Upper + "�� �̸��� ���� GameEngineLevel�� �̹� �����մϴ�.");
			return;
		}

		std::shared_ptr<GameEngineLevel> TempCurLevel = CurLevel;
		std::shared_ptr<GameEngineLevel> NewLevel = std::make_shared<LevelType>();
		CurLevel = NewLevel;
		LevelInit(NewLevel, _Name);
		CurLevel = TempCurLevel;
		AllLevel.insert(std::make_pair(Upper, NewLevel));
	}

	static void ChangeLevel(const std::string& _Name)
	{
		std::string Upper = GameEngineString::ToUpperReturn(_Name);

		std::map<std::string, std::shared_ptr<GameEngineLevel>>::iterator Finditer = AllLevel.find(Upper);

		// �̹� ���ο� TitleLevel�� �����Ѵ�.
		if (AllLevel.end() == Finditer)
		{
			MsgBoxAssert(Upper + "�� �̸��� ���� GameEngineLevel�� �������� �ʽ��ϴ�.");
			return;
		}

		NextLevel = Finditer->second;
	}

	static std::shared_ptr<class GameEngineRenderTarget> GetBackBufferRenderTarget()
	{
		return MainDevice.GetBackBufferRenderTarget();
	}

	static ID3D11Device* GetDevice() 
	{
		return MainDevice.GetDevice();
	}

	static ID3D11DeviceContext* GetContext()
	{
		return MainDevice.GetContext();
	}

	static std::map<std::string, std::shared_ptr<GameEngineLevel>>& GetAllLevel() 
	{
		return AllLevel;
	}

	static std::shared_ptr<GameEngineLevel> GetCurLevel() 
	{
		return CurLevel;
	}

protected:

private:
	static GameEngineDevice MainDevice;

	static void EngineProcess(HINSTANCE _Inst, const std::string& _Name, float4 _Pos, float4 _Size);
	static std::shared_ptr<GameEngineObject> CoreObject;

	static void LevelInit(std::shared_ptr<GameEngineLevel> _Level, std::string_view _Name);

	static std::shared_ptr<GameEngineLevel> CurLevel;
	static std::shared_ptr<GameEngineLevel> NextLevel;
	static std::map<std::string, std::shared_ptr<GameEngineLevel>> AllLevel;


	static void Start();
	static void Update();
	static void Release();
	


};
