#include "PreCompile.h"
#include "ContentsControlWindow.h"
#include "MapEditorLevel.h"
#include "Monster.h"


void MapEditorTab::Start()
{
	GameEngineCore::ChangeLevel("MapEditor");
}

void MapEditorTab::OnGUI(GameEngineLevel* _Level, float _DeltaTime)
{
	if (_Level->GetName() != "MapEditor")
	{
		return;
	}

	if (ImGui::Button("Save"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExistsChild("ContentsResources");
		Dir.MoveChild("ContentsResources");
		Dir.MoveChild("Data");


		OPENFILENAMEA OFN;
		char filePathName[100] = "";
		char lpstrFile[100] = "";
		static char filter[] = "모든 파일\0*.*\0텍스트 파일\0*.txt\0fbx 파일\0*.fbx";

		std::string Path = Dir.GetStringPath();

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = GameEngineCore::MainWindow.GetHWND();
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 100;
		OFN.lpstrDefExt = "GameData";
		OFN.lpstrInitialDir = Path.c_str();

		if (GetSaveFileNameA(&OFN) != 0) {
			SavePath = OFN.lpstrFile;
		}
	}

	//InputPath.resize(256);

	//std::string Labal = "저장경로";
	//Labal = GameEngineString::AnsiToUTF8(Labal);
	//ImGui::InputText(Labal.c_str(), &InputPath[0], InputPath.size());

	//GameEngineDirectory Dir;
	//Dir.MoveParentToExistsChild("ContentsResources");
	//Dir.MoveChild("ContentsResources");
	//Dir.MoveChild("Data");

	//std::string PathName = Dir.GetFileName();

	//DefSavePath = PathName + InputPath + ".GameData";


	MapEditorLevel* MapLevel = dynamic_cast<MapEditorLevel*>(_Level);

	if (nullptr == MapLevel)
	{
		return;
	}

	// MapLevel->BackGroundRenderer->SetSprite

	std::string Labal = "저장경로";
	Labal = "BackImageName";
	Labal = GameEngineString::AnsiToUTF8(Labal);
	ImGui::InputText(Labal.c_str(), BackGroundName, 256);

	if (ImGui::Button("Setting"))
	{

		std::shared_ptr<GameEngineTexture> Tex = GameEngineTexture::Find(BackGroundName);

		float4 HScale = Tex->GetScale().Half();
		HScale.Y *= -1.0f;

		MapLevel->BackGroundRenderer->SetSprite(BackGroundName);
		MapLevel->BackGroundRenderer->Transform.SetLocalPosition(HScale);
	}

	if ("" != SavePath)
	{
		ImGui::Text(SavePath.c_str());

		if (ImGui::Button("MapDataSave"))
		{
			GameEngineSerializer BinSer;
			BinSer << MapLevel->BackGroundRenderer->GetSprite()->GetName();
			std::vector<std::shared_ptr<Monster>> ObjectType = _Level->GetObjectGroupConvert<Monster>(ContentsObjectType::Monster);
			BinSer << static_cast<unsigned int>(ObjectType.size());
			for (size_t i = 0; i < ObjectType.size(); i++)
			{
				ObjectType[i]->Serializer(BinSer);
			}


			GameEngineFile File = SavePath;
			File.Open(FileOpenType::Write, FileDataType::Binary);
			File.Write(BinSer);

		}
	}

	if (ImGui::Button("Load"))
	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExistsChild("ContentsResources");
		Dir.MoveChild("ContentsResources");
		Dir.MoveChild("Data");


		OPENFILENAMEA OFN;
		char filePathName[100] = "";
		char lpstrFile[100] = "";
		static char filter[] = "모든 파일\0*.*\0텍스트 파일\0*.txt\0fbx 파일\0*.fbx";

		std::string Path = Dir.GetStringPath();

		memset(&OFN, 0, sizeof(OPENFILENAME));
		OFN.lStructSize = sizeof(OPENFILENAME);
		OFN.hwndOwner = GameEngineCore::MainWindow.GetHWND();
		OFN.lpstrFilter = filter;
		OFN.lpstrFile = lpstrFile;
		OFN.nMaxFile = 100;
		OFN.lpstrDefExt = "GameData";
		OFN.lpstrInitialDir = Path.c_str();

		if (GetOpenFileNameA(&OFN) != 0) {
			LoadPath = OFN.lpstrFile;
		}
	}

	if (LoadPath != "")
	{
		ImGui::Text(LoadPath.c_str());

		if (ImGui::Button("MapDataLoad"))
		{
			GameEngineSerializer BinSer;

			GameEngineFile File = LoadPath;
			File.Open(FileOpenType::Read, FileDataType::Binary);
			File.DataAllRead(BinSer);

			std::vector<std::shared_ptr<Monster>> ObjectType = _Level->GetObjectGroupConvert<Monster>(ContentsObjectType::Monster);
			for (size_t i = 0; i < ObjectType.size(); i++)
			{
				// 다 죽인다.
				ObjectType[i]->Death();
			}


			std::string BackFileName;
			BinSer >> BackFileName;
			unsigned int MonsterCount = 0;
			BinSer >> MonsterCount;

			for (size_t i = 0; i < MonsterCount; i++)
			{
				std::shared_ptr<Monster> Object = _Level->CreateActor<Monster>(ContentsObjectType::Monster);
				Object->DeSerializer(BinSer);
			}

			

			// GameEngineSerializer BinSer;
			// SaveBin << MapLevel->BackGroundRenderer->GetSprite()->GetName();
		}
	}

	// 일반적으로 그냥 클래스를 저장할수는 없다.
	// 포인터는 저장의 의미가 없다.

}

void TestTab::OnGUI(GameEngineLevel* _Level, float _DeltaTime)
{
	std::string MousePos = GameEngineCore::MainWindow.GetMousePos().ToString();

	if (ImGui::Button(MousePos.c_str()))
	{

	}

	if (ImGui::Button("Collision OnOff"))
	{
		GameEngineLevel::IsDebug = !GameEngineLevel::IsDebug;
	}

	std::list<std::shared_ptr<GameEngineObject>> ObjectLists = _Level->GetObjectGroupInt(0);

	std::vector<std::shared_ptr<GameEngineObject>> Objects;

	for (std::shared_ptr<GameEngineObject> Ptr : ObjectLists)
	{
		Objects.push_back(Ptr);
	}


	if (Objects.size())
	{
		std::vector<std::string> Names;

		for (std::shared_ptr<GameEngineObject> Ptr : Objects)
		{
			Names.push_back(Ptr->GetName());
		}

		//Names.push_back("aaaa");
		//Names.push_back("bbbb");

		std::vector<const char*> CNames;

		for (size_t i = 0; i < Names.size(); i++)
		{
			CNames.push_back(Names[i].c_str());
		}

		if (ImGui::ListBox("ObjectList", &Select, &CNames[0], static_cast<int>(Names.size())))
		{
			SelectObject = Objects[Select];
		}

		if (nullptr != SelectObject)
		{
			if (ImGui::Button("Select Object Off"))
			{
				SelectObject->Off();
			}

		}
	}
}


void ContentsControlWindow::Start()
{
	Tabs.push_back(std::make_shared<LevelChangeTab>("LevelChangeTab"));
	CurTab = Tabs[0];
	Tabs.push_back(std::make_shared<TestTab>("Test"));
	Tabs.push_back(std::make_shared<MapEditorTab>("MapEditor"));
	
}

void LevelChangeTab::OnGUI(GameEngineLevel* _Level, float _DeltaTime)
{
	std::map<std::string, std::shared_ptr<GameEngineLevel>>& AllLevels = GameEngineCore::GetAllLevel();

	for (std::pair<const std::string, std::shared_ptr<GameEngineLevel>> Pair : AllLevels)
	{
		if (ImGui::Button(Pair.first.c_str()))
		{
			GameEngineCore::ChangeLevel(Pair.first);
		}
	}
}


void ContentsControlWindow::OnGUI(GameEngineLevel* _Level, float _DeltaTime)
{
	for (size_t i = 0; i < Tabs.size(); i++)
	{
		if (ImGui::Button(Tabs[i]->Name.c_str()))
		{
			CurTab = Tabs[i];
			CurTab->Start();
		}
		ImGui::SameLine();
	}

	if (ImGui::Button("Off"))
	{
		Off();
	}

	if (nullptr != CurTab)
	{
		CurTab->OnGUI(_Level, _DeltaTime);
	}
}
