#include "PreCompile.h"
#include "ContentsSpriteRenderer.h"


void ContentsFrameAnimation::EventCall(int _Frame)
{
	if (true == FrameEventFunction.contains(Index[_Frame]))
	{
		FrameEventFunction[Index[_Frame]](Parent);
	}
}

void ContentsFrameAnimation::Reset()
{
	CurTime = 0.0f;
	CurIndex = 0;
	IsEnd = false;
	EventCheck = true;
}

SpriteData ContentsFrameAnimation::Update(float _DeltaTime)
{
	if (true == Parent->IsPause)
	{
		return Sprite->GetSpriteData(Index[CurIndex]);
	}

	if (true == Loop)
	{
		IsEnd = false;
	}

	if (true == EventCheck && false == IsEnd)
	{
		EventCall(CurIndex);
		EventCheck = false;
	}

	CurTime += _DeltaTime;

	if (Inter[CurIndex] <= CurTime)
	{
		CurTime -= Inter[CurIndex];
		++CurIndex;
		EventCheck = true;

		if (CurIndex > End - Start)
		{
			if (nullptr != EndEvent && false == IsEnd)
			{
				EndEvent(Parent);
			}

			IsEnd = true;

			if (true == Loop)
			{
				CurIndex = 0;
			} else
			{
				--CurIndex;
			}
		}
	}

	return Sprite->GetSpriteData(Index[CurIndex]);
}

ContentsSpriteRenderer::ContentsSpriteRenderer() 
{
}

ContentsSpriteRenderer::~ContentsSpriteRenderer() 
{
}

void ContentsSpriteRenderer::Start() 
{
	GameEngineRenderer::Start();

	ImageTransform.SetParent(Transform);

	GameEngineRenderer::SetMesh("Rect");
	GameEngineRenderer::SetMaterial("Contents2DTexture");
}

// Update Order에 영향을 받는다.
void ContentsSpriteRenderer::Update(float _Delta)
{
	if (nullptr != CurFrameAnimations)
	{
		Sprite = CurFrameAnimations->Sprite;
		CurSprite = CurFrameAnimations->Update(_Delta);
	}

	if (true == IsImageSize)
	{
		float4 Scale = float4(CurSprite.GetScale());
		Scale.Z = 1.0f;
		Scale.W = 0.0f;
		SetImageScale(Scale * AutoScaleRatio);
	}
}

void ContentsSpriteRenderer::SetImageScale(const float4& _Scale)
{
	ImageTransform.SetLocalScale(_Scale);
}

void ContentsSpriteRenderer::AddImageScale(const float4& _Scale)
{
	ImageTransform.AddLocalScale(_Scale);
}


void ContentsSpriteRenderer::Render(GameEngineCamera* _Camera, float _Delta)
{
	float4 ParentScale = Transform.GetLocalScale();
	float4 Scale = ImageTransform.GetLocalScale();

	float4 CalPivot = Pivot;
	CalPivot.X -= 0.5f;
	CalPivot.Y -= 0.5f;

	float4 PivotPos;
	PivotPos.X = Scale.X * CalPivot.X;
	PivotPos.Y = Scale.Y * CalPivot.Y;

	ImageTransform.SetLocalPosition(PivotPos);

	ImageTransform.TransformUpdate();
	ImageTransform.CalculationViewAndProjection(Transform.GetConstTransformDataRef());

	GetShaderResHelper().SetTexture("DiffuseTex", CurSprite.Texture);


	GameEngineRenderer::Render(_Camera, _Delta);



}

void ContentsSpriteRenderer::SetSprite(std::string_view _Name, unsigned int index /*= 0*/)
{
	CurFrameAnimations = nullptr;

	Sprite = GameEngineSprite::Find(_Name);

	if (nullptr == Sprite)
	{
		MsgBoxAssert("존재하지 않는 스프라이트를 사용하려고 했습니다.");
	}

	CurSprite = Sprite->GetSpriteData(index);
	SetImageScale(CurSprite.GetScale() * AutoScaleRatio);
}

void ContentsSpriteRenderer::CreateAnimation(
	std::string_view _AnimationName,
	std::string_view _SpriteName,
	float _Inter /*= 0.1f*/,
	unsigned int _Start /*= -1*/,
	unsigned int _End /*= -1*/,
	bool _Loop /*= true*/
) 
{
	std::string SpriteName = GameEngineString::ToUpperReturn(_SpriteName);

	std::shared_ptr<GameEngineSprite> Sprite = GameEngineSprite::Find(SpriteName);
	if (nullptr == Sprite)
	{
		MsgBoxAssert("존재하지 않는 스프라이트로 애니메이션을 만들려고 했습니다.");
		return;
	}

	std::string UpperName = GameEngineString::ToUpperReturn(_AnimationName);
	if (true == FrameAnimations.contains(UpperName))
	{
		MsgBoxAssert("이미 존재하는 애니메이션을 또 만들려고 했습니다.");
		return;
	}

	std::shared_ptr<ContentsFrameAnimation> NewAnimation = std::make_shared<ContentsFrameAnimation>();
	FrameAnimations[UpperName] = NewAnimation;
	NewAnimation->AnimationName = _AnimationName;
	NewAnimation->SpriteName = _SpriteName;
	NewAnimation->Sprite = Sprite;
	NewAnimation->Loop = _Loop;

	NewAnimation->Parent = this;

	if (_Start != -1)
	{
		NewAnimation->Start = _Start;
	}
	else 
	{
		NewAnimation->Start = 0;
	}

	if (_End != -1)
	{
		NewAnimation->End = _End;
	}
	else
	{
		NewAnimation->End = Sprite->GetSpriteCount() - 1;
	}


	for (unsigned int i = NewAnimation->Start; i <= NewAnimation->End; i++)
	{
		NewAnimation->Index.push_back(i);
	}

	NewAnimation->Inter.resize(NewAnimation->Index.size());
	for (size_t i = 0; i < NewAnimation->Index.size(); i++)
	{
		NewAnimation->Inter[i] = _Inter;
	}



	NewAnimation->CurIndex = 0;
}

void ContentsSpriteRenderer::ChangeAnimation(std::string_view _AnimationName, bool _Force /*= false*/, unsigned int _FrameIndex /*= 0*/)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_AnimationName);

	std::map<std::string, std::shared_ptr<ContentsFrameAnimation>>::iterator FindIter 
		= FrameAnimations.find(UpperName);

	if (FindIter == FrameAnimations.end())
	{
		MsgBoxAssert("존재하지 않는 애니메이션으로 체인지하려고 했습니다.");
		return;
	}

	if (_Force == false && FindIter->second == CurFrameAnimations)
	{
		return;
	}

	CurFrameAnimations = FrameAnimations[UpperName];
	CurFrameAnimations->Reset();
	CurFrameAnimations->CurIndex = _FrameIndex;
	Sprite = CurFrameAnimations->Sprite;
	CurSprite = CurFrameAnimations->Sprite->GetSpriteData(CurFrameAnimations->CurIndex);
}

void ContentsSpriteRenderer::AutoSpriteSizeOn()
{
	IsImageSize = true;
}

void ContentsSpriteRenderer::AutoSpriteSizeOff()
{
	IsImageSize = false;
}

void ContentsSpriteRenderer::SetFrameEvent(std::string_view _AnimationName, int _Frame, std::function<void(ContentsSpriteRenderer*)> _Function)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_AnimationName);

	std::map<std::string, std::shared_ptr<ContentsFrameAnimation>>::iterator FindIter = FrameAnimations.find(UpperName);

	std::shared_ptr<ContentsFrameAnimation> Animation = FindIter->second;

	if (nullptr == Animation)
	{
		MsgBoxAssert("존재하지 않는 애니메이션에 이벤트를 만들려고 했습니다.");
	}

	Animation->FrameEventFunction[_Frame] = _Function;
}

void ContentsSpriteRenderer::SetStartEvent(std::string_view _AnimationName, std::function<void(ContentsSpriteRenderer*)> _Function)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_AnimationName);

	std::map<std::string, std::shared_ptr<ContentsFrameAnimation>>::iterator FindIter = FrameAnimations.find(UpperName);

	std::shared_ptr<ContentsFrameAnimation> Animation = FindIter->second;

	if (nullptr == Animation)
	{
		MsgBoxAssert("존재하지 않는 애니메이션에 이벤트를 만들려고 했습니다.");
	}

	Animation->FrameEventFunction[Animation->Index[0]] = _Function;
}

void ContentsSpriteRenderer::SetEndEvent(std::string_view _AnimationName, std::function<void(ContentsSpriteRenderer*)> _Function)
{
	std::string UpperName = GameEngineString::ToUpperReturn(_AnimationName);

	std::map<std::string, std::shared_ptr<ContentsFrameAnimation>>::iterator FindIter = FrameAnimations.find(UpperName);

	std::shared_ptr<ContentsFrameAnimation> Animation = FindIter->second;

	if (nullptr == Animation)
	{
		MsgBoxAssert("존재하지 않는 애니메이션에 이벤트를 만들려고 했습니다.");
	}

	Animation->EndEvent = _Function;
}

void ContentsSpriteRenderer::AnimationPauseSwitch()
{
	IsPause = !IsPause;
}

void ContentsSpriteRenderer::AnimationPauseOn()
{
	IsPause = true;
}

void ContentsSpriteRenderer::AnimationPauseOff()
{
	IsPause = false;
}

void ContentsSpriteRenderer::SetPivotType(PivotType _Type)
{
	switch (_Type)
	{
	case PivotType::Center:
		Pivot = {0.5f, 0.5f};
		break;
	case PivotType::Top:
		Pivot = { 0.5f, 0.0f };
		break;
	case PivotType::RightUp:
		Pivot = { 0.0f, 0.0f };
		break;
	case PivotType::Right:
		Pivot = { 0.0f, 0.5f };
		break;
	case PivotType::RightBottom:
		Pivot = { 0.0f, 1.0f };
		break;
	case PivotType::Bottom:
		Pivot = { 0.5f, 1.0f };
		break;
	case PivotType::LeftBottom:
		Pivot = { 1.0f, 1.0f };
		break;
	case PivotType::Left:
		Pivot = { 1.0f, 0.5f };
		break;
	case PivotType::LeftTop:
		Pivot = { 1.0f, 0.0f };
		break;
	default:
		break;
	}
}

void ContentsSpriteRenderer::SetMaterialEvent(std::string_view _Name, int _Index)
{
	const TransformData& Data = ImageTransform.GetConstTransformDataRef();
	GetShaderResHelper().SetConstantBufferLink("TransformData", Data);
	GetShaderResHelper().SetConstantBufferLink("SpriteData", CurSprite.SpritePivot);
	GetShaderResHelper().SetConstantBufferLink("SpriteRendererInfo", SpriteRendererInfoValue);
	GetShaderResHelper().SetConstantBufferLink("ColorData", ColorDataValue);
	SetSprite("NSet.png");
}


void ContentsSpriteRenderer::SetMaskTexture(std::string_view _Texture)
{
	// 이녀석한테 있는
	GameEngineRenderer::SetMaterial("2DTextureMask");

	GetShaderResHelper().SetTexture("MaskTex", _Texture);
}