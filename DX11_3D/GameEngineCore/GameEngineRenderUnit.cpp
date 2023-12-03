#include "PreCompile.h"
#include "GameEngineRenderUnit.h"
#include "GameEngineMesh.h"
#include "GameEngineInputLayOut.h"
#include "GameEngineMaterial.h"
#include "GameEngineRenderTarget.h"
#include "GameEngineShader.h"
#include "GameEngineVertexShader.h"
#include "GameEnginePixelShader.h"
#include "GameEngineGeometryShader.h"
#include "GameEngineRenderer.H"


GameEngineRenderUnit::GameEngineRenderUnit() 
{
}

GameEngineRenderUnit::~GameEngineRenderUnit() 
{
}

void GameEngineRenderUnit::SetText(const std::string& _Font, const std::string& _Text, float _Scale, float4 _Color, FW1_TEXT_FLAG Flag)
{
	Font = GameEngineFont::Find(_Font);

	if (nullptr == Font)
	{
		MsgBoxAssert("�ε����� �ʴ� ��Ʈ�� ����Ϸ��� �߽��ϴ�.");
	}
	

	FontText = _Text;
	FontScale = _Scale;
	FontColor = _Color;
	FontFlag = Flag;
}

void GameEngineRenderUnit::ChangeText(std::string_view _Text)
{
	if (nullptr == Font)
	{
		MsgBoxAssert("�������� �ʴ� ��Ʈ�� �����Ϸ� �߽��ϴ�.");
		return;
	}

	FontText = _Text;
}

void GameEngineRenderUnit::ChangeFontScale(float _Size)
{
	if (nullptr == Font)
	{
		MsgBoxAssert("�������� �ʴ� ��Ʈ�� �����Ϸ� �߽��ϴ�.");
		return;
	}

	FontScale = _Size;
}

void GameEngineRenderUnit::SetTextColor(const float4& _Color /*= float4::RED*/)
{
	if (nullptr == Font)
	{
		MsgBoxAssert("�������� �ʴ� ��Ʈ�� �����Ϸ� �߽��ϴ�.");
		return;
	}

	FontColor = _Color;
}

void GameEngineRenderUnit::SetTextAlpha(float _AlphaValue /*= 0.0f*/)
{
	if (nullptr == Font)
	{
		MsgBoxAssert("�������� �ʴ� ��Ʈ�� �����Ϸ� �߽��ϴ�.");
		return;
	}

	FontColor.A = _AlphaValue;
}


void GameEngineRenderUnit::ResSetting()
{
	if (nullptr != Font)
	{
		return;
	}

	Mesh->InputAssembler1();
	Material->VertexShader();
	Material->GeometryShader();
	LayOut->Setting();
	Mesh->InputAssembler2();
	Material->Rasterizer();
	Material->PixelShader();
	Material->Blend();
	Material->DepthStencil();

	ShaderResHelper.AllShaderResourcesSetting();
}

void GameEngineRenderUnit::ResReset()
{
	Material->GeometryShaderReset();
}



void GameEngineRenderUnit::Draw()
{
	if (nullptr != Font)
	{
		float4x4 ViewPort;
		float4 Scale = GameEngineCore::MainWindow.GetScale();
		ViewPort.ViewPort(Scale.X, Scale.Y, 0, 0);

		GameEngineActor* Parent = ParentRenderer->GetParent<GameEngineActor>();

		if (nullptr != Parent && ParentRenderer->GetCamera()->GetProjectionType() == EPROJECTIONTYPE::Orthographic)
		{
			float4 ScreenPos = ParentRenderer->Transform.GetWorldPosition();

			ScreenPos *= ParentRenderer->Transform.GetConstTransformDataRef().ViewMatrix;
			ScreenPos *= ParentRenderer->Transform.GetConstTransformDataRef().ProjectionMatrix;
			ScreenPos *= ViewPort;
			// WindowPos
			Font->FontDraw(FontText, FontScale, ScreenPos, FontColor, FontFlag);

			GameEngineCore::GetContext()->GSSetShader(nullptr, nullptr, 0);
		}
		else if (nullptr != Parent && ParentRenderer->GetCamera()->GetProjectionType() == EPROJECTIONTYPE::Perspective)
		{
			float4 ScreenPos = ParentRenderer->Transform.GetWorldPosition();

			ScreenPos *= ParentRenderer->Transform.GetConstTransformDataRef().ViewMatrix;
			ScreenPos *= ParentRenderer->Transform.GetConstTransformDataRef().ProjectionMatrix;


			const float RHW = 1.0f / ScreenPos.W;

			float4 PosInScreenSpace = float4(ScreenPos.X * RHW, ScreenPos.Y * RHW, ScreenPos.Z * RHW, ScreenPos.W);
			const float NormalizedX = (PosInScreenSpace.X / 2.f) + 0.5f;
			const float NormalizedY = 1.f - (PosInScreenSpace.Y / 2.f) - 0.5f;

			float4 RayStartViewRectSpace;

			RayStartViewRectSpace.X = NormalizedX * Scale.X;
			RayStartViewRectSpace.Y = NormalizedY * Scale.Y;

			float4 Result = RayStartViewRectSpace + float4(0, 0, 0, 0);


			Font->FontDraw(FontText, FontScale, Result, FontColor, FontFlag);
			GameEngineCore::GetContext()->GSSetShader(nullptr, nullptr, 0);
		}


		return;
	}

	switch (Mode)
	{
	case RenderMode::Indexed:
		Mesh->IndexedDraw();
		break;
	case RenderMode::Instancing:
		Mesh->InstancingDraw(InstancingCount);
		break;
	default:
		break;
	}
}

void GameEngineRenderUnit::SetMesh(std::string_view _Name)
{
	Mesh = GameEngineMesh::Find(_Name);

	if (nullptr == Mesh)
	{
		MsgBoxAssert("�������� �ʴ� �Ž��� �����Ϸ��� �߽��ϴ�.");
	}

	if (nullptr == LayOut && nullptr != Material)
	{
		LayOut = std::make_shared<GameEngineInputLayOut>();
		LayOut->ResCreate(Mesh->GetVertexBuffer(), Material->GetVertexShader());
	}

}

void GameEngineRenderUnit::SetMaterial(std::string_view _Name)
{
	Material = GameEngineMaterial::Find(_Name);

	ShaderResHelper.ResClear();

	if (nullptr == Material)
	{
		MsgBoxAssert("�������� �ʴ� �Ž��� �����Ϸ��� �߽��ϴ�.");
	}

	if (nullptr == LayOut && nullptr != Mesh)
	{
		LayOut = std::make_shared<GameEngineInputLayOut>();
		LayOut->ResCreate(Mesh->GetVertexBuffer(), Material->GetVertexShader());
	}

	// ���ؽ� ���̴��� �ȼ����̴��� �� ����ִ� ���´�.

	// �������� ���̴� ���ҽ� ���ۿ�
	// ���ؽ��� �ȼ����̴��� ���ҽ��������� ���� �޽��ϴ�.
	ShaderResHelper.ShaderResCopy(Material->GetVertexShader().get());
	ShaderResHelper.ShaderResCopy(Material->GetGeometryShader().get());
	ShaderResHelper.ShaderResCopy(Material->GetPixelShader().get());

	// �̰� ȸ���� ���.

	if (nullptr != ParentRenderer 
		&& ShaderResHelper.IsConstantBuffer("TransformData"))
	{
		const TransformData& Data = ParentRenderer->Transform.GetConstTransformDataRef();
		ShaderResHelper.SetConstantBufferLink("TransformData", Data);
	}

	//	//std::shared_ptr<GameEngineConstantBuffer> Buffer = GameEngineConstantBuffer::CreateAndFind(sizeof(TransformData), "TransformData", _shader);
//	//if (nullptr != Buffer)
//	//{
//	//	const TransformData& Data = DataTransform->GetConstTransformDataRef();
//	//	Buffer->ChangeData(Data);
//	//	Buffer->Setting(0);
//	//}

}

void GameEngineRenderUnit::Render()
{
	ResSetting();
	Draw();
	// ShaderResHelper.AllShaderResourcesReset();
}