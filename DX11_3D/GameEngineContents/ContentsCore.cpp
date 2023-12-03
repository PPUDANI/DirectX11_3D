#include "PreCompile.h"
#include "ContentsCore.h"

#include <GameEngineCore/GameEngineSampler.h>
#include <GameEngineCore/GameEngineBlend.h>
#include <GameEngineCore/GameEngineSpriteRenderer.h>
#include <GameEngineCore/GameEngineCoreWindow.h>

#include "PlayLevel.h"
#include "TitleLevel.h"
#include "MapEditorLevel.h"
#include "ContentsControlWindow.h"

ContentsCore::ContentsCore() 
{
}

ContentsCore::~ContentsCore() 
{
}

void ContentsCore::UserRes()
{
	{

		{
			// 엔진용 쉐이더를 전부다 전부다 로드하는 코드를 친다.
			GameEngineDirectory Dir;
			Dir.MoveParentToExistsChild("ContentsResources");
			Dir.MoveChild("GameEngineContentsShader");
			std::vector<GameEngineFile> Files = Dir.GetAllFile({ ".fx" });

			for (size_t i = 0; i < Files.size(); i++)
			{
				// 구조적으로 잘 이해하고 있는지를 자신이 명확하게 인지하기 위해서
				GameEngineFile& File = Files[i];
				GameEngineShader::AutoCompile(File);
			}
		}
	}

	{
		std::shared_ptr<GameEngineMaterial> Mat = GameEngineMaterial::Create("Contents2DTexture");
		Mat->SetVertexShader("ContentsTextureShader_VS");
		Mat->SetPixelShader("ContentsTextureShader_PS");
	}

	{
		std::shared_ptr<GameEngineMaterial> Mat = GameEngineMaterial::Create("PlayerEffect");
		Mat->SetVertexShader("PlayerEffect_VS");
		Mat->SetPixelShader("PlayerEffect_PS");
		Mat->SetDepthState("AlwaysDepth");
	}

	{
		std::shared_ptr<GameEngineMaterial> Mat = GameEngineMaterial::Create("OldFilm");
		Mat->SetVertexShader("OldFilm_VS");
		Mat->SetPixelShader("OldFilm_PS");
		Mat->SetDepthState("AlwaysDepth");
	}


	{
		// 엔진용 쉐이더를 전부다 전부다 로드하는 코드를 친다.
		GameEngineDirectory Dir;
		Dir.MoveParentToExistsChild("GameEngineResources");
		Dir.MoveChild("ContentsResources");
		Dir.MoveChild("Texture");
		std::vector<GameEngineFile> Files = Dir.GetAllFile();

		for (size_t i = 0; i < Files.size(); i++)
		{
			// 구조적으로 잘 이해하고 있는지를 자신이 명확하게 인지하기 위해서
			GameEngineFile& File = Files[i];
			GameEngineTexture::Load(File.GetStringPath());
		}

		GameEngineSprite::CreateCut("TestPlayer.png", 6, 6);
	}

	{
		GameEngineDirectory Dir;
		Dir.MoveParentToExistsChild("GameEngineResources");
		Dir.MoveChild("ContentsResources");
		Dir.MoveChild("FolderTexture");
		std::vector<GameEngineDirectory> Directorys = Dir.GetAllDirectory();

		for (size_t i = 0; i < Directorys.size(); i++)
		{
			// 구조적으로 잘 이해하고 있는지를 자신이 명확하게 인지하기 위해서
			GameEngineDirectory& Dir = Directorys[i];
			GameEngineSprite::CreateFolder(Dir.GetStringPath());
		}

		GameEngineSprite::CreateCut("TestPlayer.png", 6, 6);
		GameEngineSprite::CreateSingle("TestMap.png");
		GameEngineSprite::CreateSingle("EditorTest.png");
	}

	{
		D3D11_BLEND_DESC Desc = {};
		Desc.IndependentBlendEnable = false;
		Desc.RenderTarget[0].BlendEnable = true;
		Desc.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;

		Desc.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlend = D3D11_BLEND_ONE; // src팩터
		Desc.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;

		Desc.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
		Desc.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ZERO;
		Desc.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;

		std::shared_ptr<GameEngineBlend> Blend = GameEngineBlend::Create("OverRay", Desc);
	}

	{
		std::shared_ptr<GameEngineMaterial> Mat = GameEngineMaterial::Create("2DTextureOver");
		Mat->SetVertexShader("TextureShader_VS");
		Mat->SetPixelShader("TextureShader_PS");
		Mat->SetBlendState("OverRay");
	}
}


void ContentsCore::Start()
{
	UserRes();

	GameEngineGUI::CreateGUIWindow<GameEngineCoreWindow>("GameEngineCoreWindow");
	// GameEngineCoreWindow::OnGUI

	// GameEngineGUI::CreateGUIWindow<ContentsControlWindow>("ContentsControlWindow");

	//기본적으로 SpriteRenderer를 만들때 넣어줄 샘플러를 지정합니다.
	GameEngineCore::CreateLevel<PlayLevel>("PlayLevel");
	GameEngineCore::CreateLevel<TitleLevel>("TitleLevel");
	GameEngineCore::CreateLevel<MapEditorLevel>("MapEditor");

	GameEngineCore::ChangeLevel("PlayLevel");
	
	// 자기 텍스처 로드해야 한다.

}

void ContentsCore::Update(float _Delta)
{

}

void ContentsCore::Release()
{

}