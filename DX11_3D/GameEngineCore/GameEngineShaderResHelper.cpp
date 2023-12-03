#include "PreCompile.h"
#include "GameEngineShaderResHelper.h"
#include "GameEngineShader.h"


void GameEngineConstantBufferSetter::Setting()
{
	if (nullptr == CPUDataPtr)
	{
		MsgBoxAssert(Name + "������۸� ���������� �ʾҽ��ϴ�.");
	}

	ShaderType Type = ParentShader->GetShaderType();

	Res->ChangeData(CPUDataPtr, DataSize);

	switch (Type)
	{
	case ShaderType::Vertex:
		Res->VSSetting(BindPoint);
		break;
	case ShaderType::Pixel:
		Res->PSSetting(BindPoint);
		break;
	case ShaderType::Compute:
		Res->CSSetting(BindPoint);
		break;
	case ShaderType::Geometry:
		Res->GSSetting(BindPoint);
		break;
	default:
		MsgBoxAssert("ó���Ҽ� ���� ���̴� ���� �����Դϴ�.");
		break;
	}

}

void GameEngineConstantBufferSetter::Reset()
{

}

void GameEngineTextureSetter::Setting()
{
	ShaderType Type = ParentShader->GetShaderType();

	switch (Type)
	{
	case ShaderType::Vertex:
		Res->VSSetting(BindPoint);
		break;
	case ShaderType::Pixel:
		Res->PSSetting(BindPoint);
		break;
	case ShaderType::Compute:
		Res->CSSetting(BindPoint);
		break;
	default:
		MsgBoxAssert("ó���Ҽ� ���� ���̴� ���� �����Դϴ�.");
		break;
	}

}

void GameEngineTextureSetter::Reset()
{
	ShaderType Type = ParentShader->GetShaderType();

	switch (Type)
	{
	case ShaderType::Vertex:
		Res->VSReset(BindPoint);
		break;
	case ShaderType::Pixel:
		Res->PSReset(BindPoint);
		break;
	case ShaderType::Compute:
		Res->CSReset(BindPoint);
		break;
	default:
		MsgBoxAssert("ó���Ҽ� ���� ���̴� ���� �����Դϴ�.");
		break;
	}
}

void GameEngineSamplerSetter::Setting()
{
	ShaderType Type = ParentShader->GetShaderType();

	switch (Type)
	{
	case ShaderType::Vertex:
		Res->VSSetting(BindPoint);
		break;
	case ShaderType::Pixel:
		Res->PSSetting(BindPoint);
		break;
	case ShaderType::Compute:
		Res->CSSetting(BindPoint);
		break;
	default:
		MsgBoxAssert("ó���Ҽ� ���� ���̴� ���� �����Դϴ�.");
		break;
	}

}

void GameEngineStructedBufferSetter::Setting()
{
	ShaderType Type = ParentShader->GetShaderType();

	if (Res->GetStructuredBufferType() == StructuredBufferType::SRV_ONLY)
	{
		if (nullptr == CPUDataPtr)
		{
			MsgBoxAssert(Name + "����ȭ ���۸� ���������� �ʾҽ��ϴ�.");
		}

		Res->ChangeData(CPUDataPtr, DataSize * DataCount);
	}

	switch (Type)
	{
	case ShaderType::Vertex:
		Res->VSSetting(BindPoint);
		break;
	case ShaderType::Pixel:
		Res->PSSetting(BindPoint);
		break;
	case ShaderType::Compute:
		Res->CSSetting(BindPoint);
		break;
	case ShaderType::Geometry:
		Res->GSSetting(BindPoint);
		break;
	default:
		MsgBoxAssert("ó���Ҽ� ���� ���̴� ���� �����Դϴ�.");
		break;
	}

}

void GameEngineStructedBufferSetter::Reset()
{
	ShaderType Type = ParentShader->GetShaderType();

	switch (Type)
	{
	case ShaderType::Vertex:
		Res->VSReset(BindPoint);
		break;
	case ShaderType::Pixel:
		Res->VSReset(BindPoint);
		break;
	case ShaderType::Compute:
		Res->CSReset(BindPoint);
		break;
	default:
		MsgBoxAssert("ó���Ҽ� ���� ���̴� ���� �����Դϴ�.");
		break;
	}
}



void GameEngineSamplerSetter::Reset()
{

}


GameEngineShaderResHelper::GameEngineShaderResHelper()
{
}

GameEngineShaderResHelper::~GameEngineShaderResHelper()
{
}



void GameEngineShaderResHelper::ShaderResCheck(std::string _FunctionName, GameEngineShader* _Shader, ID3DBlob* _CompileCode)
{
	if (nullptr == _CompileCode)
	{
		MsgBoxAssert("������ ���� ���� �ڵ�δ� ���̴��� ���ҽ��� �����Ҽ��� �����ϴ�.");
		return;
	}

	ID3DBlob* BinaryCode = _CompileCode;

	// ���÷����� ���ؼ� ���� ���� �ڵ� ����ü�� ������ �ǹ��Ѵ�.
	// Ŭ���� ����
	// �Լ�
	// Ŭ������ Ÿ��
	// ����� �޾Ҵ���? 
	ID3D11ShaderReflection* CompileInfo = nullptr;

	if (S_OK != D3DReflect(BinaryCode->GetBufferPointer(), BinaryCode->GetBufferSize(), IID_ID3D11ShaderReflection, reinterpret_cast<void**>(&CompileInfo)))
	{
		MsgBoxAssert("���̴� ���������� �����߽��ϴ�.");
		return;
	}

	D3D11_SHADER_DESC Info;

	// Info
	// ���� => ����
	// �������� �̹� ������ ���ٰ� + ��尡 �ȴ�. �𸮾� ���� ��쿡�� 
	CompileInfo->GetDesc(&Info);

	D3D11_SHADER_INPUT_BIND_DESC ResDesc;
	// setTexture("MyTexture", "aaaa.png");


	// �ؽ�ó ���̴� ������ �� ���ļ� ���?
	for (UINT i = 0; i < Info.BoundResources; i++)
	{
		// �ؽ�ó�� 10����� 10
		// �ؽ�ó 2�� ������� 3�����. 
		CompileInfo->GetResourceBindingDesc(i, &ResDesc);

		std::string UpperName = GameEngineString::ToUpperReturn(ResDesc.Name);



		D3D_SHADER_INPUT_TYPE Type = ResDesc.Type;
		//D3D_SIT_CBUFFER = 0,
		//D3D_SIT_TBUFFER = (D3D_SIT_CBUFFER + 1),
		//D3D_SIT_TEXTURE = (D3D_SIT_TBUFFER + 1),
		//D3D_SIT_SAMPLER = (D3D_SIT_TEXTURE + 1),
		//D3D_SIT_UAV_RWTYPED = (D3D_SIT_SAMPLER + 1),
		//D3D_SIT_STRUCTURED = (D3D_SIT_UAV_RWTYPED + 1),
		//D3D_SIT_UAV_RWSTRUCTURED = (D3D_SIT_STRUCTURED + 1),
		//D3D_SIT_BYTEADDRESS = (D3D_SIT_UAV_RWSTRUCTURED + 1),
		//D3D_SIT_UAV_RWBYTEADDRESS = (D3D_SIT_BYTEADDRESS + 1),
		//D3D_SIT_UAV_APPEND_STRUCTURED = (D3D_SIT_UAV_RWBYTEADDRESS + 1),
		//D3D_SIT_UAV_CONSUME_STRUCTURED = (D3D_SIT_UAV_APPEND_STRUCTURED + 1),
		//D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER = (D3D_SIT_UAV_CONSUME_STRUCTURED + 1),
		//D3D_SIT_RTACCELERATIONSTRUCTURE = (D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER + 1),
		//D3D_SIT_UAV_FEEDBACKTEXTURE = (D3D_SIT_RTACCELERATIONSTRUCTURE + 1),
		//D3D10_SIT_CBUFFER = D3D_SIT_CBUFFER,
		//D3D10_SIT_TBUFFER = D3D_SIT_TBUFFER,
		//D3D10_SIT_TEXTURE = D3D_SIT_TEXTURE,
		//D3D10_SIT_SAMPLER = D3D_SIT_SAMPLER,
		//D3D11_SIT_UAV_RWTYPED = D3D_SIT_UAV_RWTYPED,
		//D3D11_SIT_STRUCTURED = D3D_SIT_STRUCTURED,
		//D3D11_SIT_UAV_RWSTRUCTURED = D3D_SIT_UAV_RWSTRUCTURED,
		//D3D11_SIT_BYTEADDRESS = D3D_SIT_BYTEADDRESS,
		//D3D11_SIT_UAV_RWBYTEADDRESS = D3D_SIT_UAV_RWBYTEADDRESS,
		//D3D11_SIT_UAV_APPEND_STRUCTURED = D3D_SIT_UAV_APPEND_STRUCTURED,
		//D3D11_SIT_UAV_CONSUME_STRUCTURED = D3D_SIT_UAV_CONSUME_STRUCTURED,
		//D3D11_SIT_UAV_RWSTRUCTURED_WITH_COUNTER = D3D_SIT_UAV_RWSTRUCTURED_WITH_COUNTER

		switch (Type)
		{
		case D3D_SIT_CBUFFER:
		{
			ID3D11ShaderReflectionConstantBuffer* BufferInfo = CompileInfo->GetConstantBufferByName(ResDesc.Name);

			D3D11_SHADER_BUFFER_DESC BufferDesc;
			BufferInfo->GetDesc(&BufferDesc);

			// �翬�� ������۸� ������ �մϴ�.
			std::shared_ptr<GameEngineConstantBuffer> CBuffer = GameEngineConstantBuffer::CreateAndFind(BufferDesc.Size, UpperName, BufferDesc);

			_FunctionName;
			GameEngineConstantBufferSetter NewSetter;
			NewSetter.ParentShader = _Shader;
			NewSetter.Res = CBuffer;
			NewSetter.Name = UpperName;
			NewSetter.BindPoint = ResDesc.BindPoint;
			NewSetter.DataSize = BufferDesc.Size;

			ConstantBufferSetters.insert(std::make_pair(UpperName, NewSetter));

			break;
		}
		case D3D_SIT_TEXTURE:
		{
			std::shared_ptr<GameEngineTexture> Res
				= GameEngineTexture::Find("NSet.png");

			_FunctionName;
			GameEngineTextureSetter NewSetter;
			NewSetter.ParentShader = _Shader;
			NewSetter.Res = Res;
			NewSetter.Name = UpperName;
			NewSetter.BindPoint = ResDesc.BindPoint;

			TextureSetters.insert(std::make_pair(UpperName, NewSetter));
			break;
		}
		case D3D_SIT_SAMPLER:
		{
			//std::shared_ptr<GameEngineSampler> Res
			//	= GameEngineSampler::Find("EngineBaseSampler");

			// �̳༮�� ���� ������ ������ ������.
			_FunctionName;
			GameEngineSamplerSetter NewSetter;
			NewSetter.ParentShader = _Shader;
			NewSetter.Res = nullptr;
			NewSetter.Name = UpperName;
			NewSetter.BindPoint = ResDesc.BindPoint;

			SamplerSetters.insert(std::make_pair(UpperName, NewSetter));
			break;
		}
		case D3D11_SIT_UAV_RWSTRUCTURED:
		case D3D_SIT_STRUCTURED:
		{
			_FunctionName;
			// ��Ʈ��ó�� ���۴� ������ۿʹ� �ٸ���.

			ID3D11ShaderReflectionConstantBuffer* BufferInfo = CompileInfo->GetConstantBufferByName(ResDesc.Name);

			D3D11_SHADER_BUFFER_DESC BufferDesc;
			BufferInfo->GetDesc(&BufferDesc);

			// �翬�� ������۸� ������ �մϴ�.
			std::shared_ptr<GameEngineStructuredBuffer> SBuffer 
				= GameEngineStructuredBuffer::CreateAndFind(BufferDesc.Size, UpperName);

			_FunctionName;
			GameEngineStructedBufferSetter NewSetter;
			NewSetter.ParentShader = _Shader;
			NewSetter.Res = SBuffer;
			NewSetter.Name = UpperName;
			NewSetter.BindPoint = ResDesc.BindPoint;
			NewSetter.DataSize = BufferDesc.Size;

			StructedBufferSetters.insert(std::make_pair(UpperName, NewSetter));
			break;
		}
		default:
		{
			Type;
			MsgBoxAssert("ó���Ҽ� ���� ���ҽ��� ����Ϸ��� �մϴ�");
			break;
		}
		}
	}


}

void GameEngineShaderResHelper::ShaderResCopy(GameEngineShader* _Shader)
{
	if (nullptr == _Shader)
	{
		// std::string 

		// OutputDebugStringA("");
		// MsgBoxAssert("");
		return;
	}

	std::multimap<std::string, GameEngineConstantBufferSetter>& OtherConstantBufferSetters = _Shader->ResHelper.ConstantBufferSetters;
	std::multimap<std::string, GameEngineTextureSetter>& OtherTextureSetters = _Shader->ResHelper.TextureSetters;
	std::multimap<std::string, GameEngineSamplerSetter>& OtherSamplerSetters = _Shader->ResHelper.SamplerSetters;
	std::multimap<std::string, GameEngineStructedBufferSetter>& OtherStructedBufferSetters = _Shader->ResHelper.StructedBufferSetters;

	for (std::pair<const std::string, GameEngineConstantBufferSetter>& Pair : OtherConstantBufferSetters)
	{
		ConstantBufferSetters.insert(std::make_pair(Pair.first, Pair.second));
	}

	for (std::pair<const std::string, GameEngineTextureSetter>& Pair : OtherTextureSetters)
	{
		TextureSetters.insert(std::make_pair(Pair.first, Pair.second));
	}

	for (std::pair<const std::string, GameEngineSamplerSetter>& Pair : OtherSamplerSetters)
	{
		SamplerSetters.insert(std::make_pair(Pair.first, Pair.second));
	}

	for (std::pair<const std::string, GameEngineStructedBufferSetter>& Pair : OtherStructedBufferSetters)
	{
		StructedBufferSetters.insert(std::make_pair(Pair.first, Pair.second));
	}


	// �⺻ ���÷��� ����������� �༮���� �ִ��� Ȯ���Ѵ�.
	for (std::pair<const std::string, GameEngineTextureSetter>& Pair : OtherTextureSetters)
	{
		std::string SamplerName = Pair.first + "SAMPLER";

		if (true == IsSampler(SamplerName))
		{
			std::shared_ptr<GameEngineSampler> Sampler = Pair.second.Res->GetBaseSampler();

			SetSampler(SamplerName, Sampler);
		}
	}
}

void GameEngineShaderResHelper::AllShaderResourcesSetting()
{
	for (std::pair<const std::string, GameEngineConstantBufferSetter>& Pair : ConstantBufferSetters)
	{
		if (nullptr == Pair.second.Res)
		{
			MsgBoxAssert(std::string(Pair.first) + "��� ���÷��� ������ ���� �ʾҽ��ϴ�.");
			return;
		}

		Pair.second.Setting();
	}

	for (std::pair<const std::string, GameEngineTextureSetter>& Pair : TextureSetters)
	{
		if (nullptr == Pair.second.Res)
		{
			MsgBoxAssert(std::string(Pair.first) + "��� ���÷��� ������ ���� �ʾҽ��ϴ�.");
			return;
		}

		Pair.second.Setting();
	}

	for (std::pair<const std::string, GameEngineSamplerSetter>& Pair : SamplerSetters)
	{
		if (nullptr == Pair.second.Res)
		{
			MsgBoxAssert(std::string(Pair.first) + "��� ���÷��� ������ ���� �ʾҽ��ϴ�.");
			return;
		}

		Pair.second.Setting();
	}


	for (std::pair<const std::string, GameEngineStructedBufferSetter>& Pair : StructedBufferSetters)
	{
		if (nullptr == Pair.second.Res)
		{
			MsgBoxAssert(std::string(Pair.first) + "��� ��Ʈ��ó�� ���۰� ������ ���� �ʾҽ��ϴ�.");
			return;
		}

		Pair.second.Setting();
	}

}

void GameEngineShaderResHelper::AllShaderResourcesReset()
{
	//for (std::pair<const std::string, GameEngineConstantBufferSetter>& Pair : ConstantBufferSetters)
	//{
	//	if (nullptr == Pair.second.Res)
	//	{
	//		MsgBoxAssert(std::string(Pair.first) + "��� ���÷��� ������ ���� �ʾҽ��ϴ�.");
	//		return;
	//	}

	//	Pair.second.Reset();
	//}

	for (std::pair<const std::string, GameEngineTextureSetter>& Pair : TextureSetters)
	{
		if (nullptr == Pair.second.Res)
		{
			MsgBoxAssert(std::string(Pair.first) + "��� ���÷��� ������ ���� �ʾҽ��ϴ�.");
			return;
		}

		Pair.second.Reset();
	}

	for (std::pair<const std::string, GameEngineStructedBufferSetter>& Pair : StructedBufferSetters)
	{
		if (nullptr == Pair.second.Res)
		{
			MsgBoxAssert(std::string(Pair.first) + "��� ���÷��� ������ ���� �ʾҽ��ϴ�.");
			return;
		}

		Pair.second.Reset();
	}


	//for (std::pair<const std::string, GameEngineSamplerSetter>& Pair : SamplerSetters)
	//{
	//	if (nullptr == Pair.second.Res)
	//	{
	//		MsgBoxAssert(std::string(Pair.first) + "��� ���÷��� ������ ���� �ʾҽ��ϴ�.");
	//		return;
	//	}

	//	Pair.second.Reset();
	//}

}

void GameEngineShaderResHelper::SetConstantBufferLink(std::string_view _Name, const void* _Data, int _Size)
{
	if (false == IsConstantBuffer(_Name))
	{
		MsgBoxAssert(std::string(_Name) + "�������� �ʴ� ������ۿ� ��ũ�� �ɷ��� �߽��ϴ�.");
		return;
	}

	std::string UpperString = GameEngineString::ToUpperReturn(_Name);

	// �ߺ��Ǵ� �̸��� ���� ���ͷ����Ϳ� �� ���ͷ����͸� ã�¹�
	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameStariter
		= ConstantBufferSetters.lower_bound(UpperString);
	std::multimap<std::string, GameEngineConstantBufferSetter>::iterator NameEnditer
		= ConstantBufferSetters.upper_bound(UpperString);

	for (; NameStariter != NameEnditer; ++NameStariter)
	{
		GameEngineConstantBufferSetter& Setter = NameStariter->second;
		if (Setter.DataSize != _Size)
		{
			MsgBoxAssert(NameStariter->first + "������ۿ� ũ�Ⱑ �ٸ� �����͸� �����Ϸ��� �߽��ϴ�.");
		}

		Setter.CPUDataPtr = _Data;
	}

}


void GameEngineShaderResHelper::SetTexture(std::string_view _Name, std::string_view _TextureName, bool _SamplerCheck /*= true*/)
{
	std::shared_ptr<GameEngineTexture> Tex = GameEngineTexture::Find(_TextureName);
	if (nullptr == Tex)
	{
		MsgBoxAssert("�������� �ʴ� �ؽ�ó �Դϴ�.");
	}

	SetTexture(_Name, Tex);
}

void GameEngineShaderResHelper::SetTexture(std::string_view _Name, std::shared_ptr<GameEngineTexture> _Texture, bool _SamplerCheck /*= true*/)
{
	if (false == IsTexture(_Name))
	{
		MsgBoxAssert(std::string(_Name) + "�������� �ʴ� �ؽ��� ���ÿ� ��ũ�� �ɷ��� �߽��ϴ�.");
		return;
	}

	std::string UpperString = GameEngineString::ToUpperReturn(_Name);

	// �ߺ��Ǵ� �̸��� ���� ���ͷ����Ϳ� �� ���ͷ����͸� ã�¹�
	std::multimap<std::string, GameEngineTextureSetter>::iterator NameStariter
		= TextureSetters.lower_bound(UpperString);
	std::multimap<std::string, GameEngineTextureSetter>::iterator NameEnditer
		= TextureSetters.upper_bound(UpperString);

	std::string SamplerName = NameStariter->first + "SAMPLER";

	for (; NameStariter != NameEnditer; ++NameStariter)
	{
		GameEngineTextureSetter& Setter = NameStariter->second;

		Setter.Res = _Texture;

		if (true == _SamplerCheck && true == IsSampler(SamplerName))
		{
			std::shared_ptr<GameEngineSampler> Sampler = Setter.Res->GetBaseSampler();

			if (nullptr != Sampler)
			{
				SetSampler(SamplerName, Sampler);
			}
		}
	}
}

void GameEngineShaderResHelper::SetSampler(std::string_view _Name, std::string_view _SamplerName)
{
	std::shared_ptr<GameEngineSampler> Samper = GameEngineSampler::Find(_SamplerName);

	SetSampler(_Name, Samper);
}

void GameEngineShaderResHelper::SetSampler(std::string_view _Name, std::shared_ptr<GameEngineSampler> _TextureSampler)
{
	if (false == IsSampler(_Name))
	{
		MsgBoxAssert("�������� �ʴ� ������ۿ� ��ũ�� �ɷ��� �߽��ϴ�.");
		return;
	}

	std::string UpperString = GameEngineString::ToUpperReturn(_Name);

	// �ߺ��Ǵ� �̸��� ���� ���ͷ����Ϳ� �� ���ͷ����͸� ã�¹�
	std::multimap<std::string, GameEngineSamplerSetter>::iterator NameStariter
		= SamplerSetters.lower_bound(UpperString);
	std::multimap<std::string, GameEngineSamplerSetter>::iterator NameEnditer
		= SamplerSetters.upper_bound(UpperString);

	for (; NameStariter != NameEnditer; ++NameStariter)
	{
		GameEngineSamplerSetter& Setter = NameStariter->second;

		Setter.Res = _TextureSampler;
	}
}

std::list<std::shared_ptr<GameEngineStructuredBuffer>> GameEngineShaderResHelper::SetStructedNew(std::string_view _Name, StructuredBufferType _Type, const void* _Data, int _Size, int _Count)
{
	std::list<std::shared_ptr<GameEngineStructuredBuffer>> ReturnList;

	if (false == IsStructedBuffer(_Name))
	{
		MsgBoxAssert(std::string(_Name) + "�������� �ʴ� ��Ʈ��ó�� ���ۿ� ��ũ�� �ɷ��� �߽��ϴ�.");
		return ReturnList;
	}

	if (0 == _Count)
	{
		MsgBoxAssert(std::string(_Name) + "������ 0���� �����͸� ��Ʈ��ó�� ���ۿ� �����Ϸ��� �߽��ϴ�.");
	}

	std::string UpperString = GameEngineString::ToUpperReturn(_Name);

	// �ߺ��Ǵ� �̸��� ���� ���ͷ����Ϳ� �� ���ͷ����͸� ã�¹�
	std::multimap<std::string, GameEngineStructedBufferSetter>::iterator NameStariter
		= StructedBufferSetters.lower_bound(UpperString);
	std::multimap<std::string, GameEngineStructedBufferSetter>::iterator NameEnditer
		= StructedBufferSetters.upper_bound(UpperString);


	for (; NameStariter != NameEnditer; ++NameStariter)
	{
		GameEngineStructedBufferSetter& Setter = NameStariter->second;
		if (Setter.DataSize != _Size)
		{
			MsgBoxAssert(NameStariter->first + "����ȭ ���ۿ� ũ�Ⱑ �ٸ� �����͸� �����Ϸ��� �߽��ϴ�.");
		}

		// ���ο� ��Ʈ��ó�� ���۸� �����.
		// ������ ���ο� ��Ʈ��ó�� ���۸� ���Ӱ� �����.
		Setter.Res = GameEngineStructuredBuffer::CreateAndFind(Setter.Res->GetDataSize(), nullptr);
		Setter.DataCount = _Count;
		Setter.Res->CreateResize(_Size, _Count, _Type, _Data);
		// �������� ���������.
		ReturnList.push_back(Setter.Res);
	}

	return ReturnList;
}

void GameEngineShaderResHelper::SetStructedBufferLink(std::string_view _Name, const void* _Data, int _Size, int _Count)
{
	if (false == IsStructedBuffer(_Name))
	{
		MsgBoxAssert(std::string(_Name) + "�������� �ʴ� ��Ʈ��ó�� ���ۿ� ��ũ�� �ɷ��� �߽��ϴ�.");
		return;
	}

	if (0 == _Count)
	{
		MsgBoxAssert(std::string(_Name) + "������ 0���� �����͸� ��Ʈ��ó�� ���ۿ� �����Ϸ��� �߽��ϴ�.");
	}

	std::string UpperString = GameEngineString::ToUpperReturn(_Name);

	// �ߺ��Ǵ� �̸��� ���� ���ͷ����Ϳ� �� ���ͷ����͸� ã�¹�
	std::multimap<std::string, GameEngineStructedBufferSetter>::iterator NameStariter
		= StructedBufferSetters.lower_bound(UpperString);
	std::multimap<std::string, GameEngineStructedBufferSetter>::iterator NameEnditer
		= StructedBufferSetters.upper_bound(UpperString);

	for (; NameStariter != NameEnditer; ++NameStariter)
	{
		GameEngineStructedBufferSetter& Setter = NameStariter->second;
		if (Setter.DataSize != _Size)
		{
			MsgBoxAssert(NameStariter->first + "����ȭ ���ۿ� ũ�Ⱑ �ٸ� �����͸� �����Ϸ��� �߽��ϴ�.");
		}

		Setter.CPUDataPtr = _Data;
		Setter.DataCount = _Count;

		// �̰� ������ �Ǹ� 
		Setter.Res->CreateResize(_Size, _Count, StructuredBufferType::SRV_ONLY, _Data);
		// �������� ���������.
	}
}

void GameEngineShaderResHelper::SetStructedBufferChange(std::string_view _Name, std::shared_ptr<GameEngineStructuredBuffer> _Buffer)
{
	if (false == IsStructedBuffer(_Name))
	{
		MsgBoxAssert(std::string(_Name) + "�������� �ʴ� ��Ʈ��ó�� ���ۿ� ��ũ�� �ɷ��� �߽��ϴ�.");
		return;
	}

	std::string UpperString = GameEngineString::ToUpperReturn(_Name);

	// �ߺ��Ǵ� �̸��� ���� ���ͷ����Ϳ� �� ���ͷ����͸� ã�¹�
	std::multimap<std::string, GameEngineStructedBufferSetter>::iterator NameStariter
		= StructedBufferSetters.lower_bound(UpperString);
	std::multimap<std::string, GameEngineStructedBufferSetter>::iterator NameEnditer
		= StructedBufferSetters.upper_bound(UpperString);

	for (; NameStariter != NameEnditer; ++NameStariter)
	{
		GameEngineStructedBufferSetter& Setter = NameStariter->second;
		Setter.Res = _Buffer;
	}
}

std::shared_ptr<GameEngineStructuredBuffer> GameEngineShaderResHelper::GetStructedBuffer(std::string_view _Name, ShaderType Type)
{
	if (false == IsStructedBuffer(_Name))
	{
		return nullptr;
	}

	std::string UpperString = GameEngineString::ToUpperReturn(_Name);

	// �ߺ��Ǵ� �̸��� ���� ���ͷ����Ϳ� �� ���ͷ����͸� ã�¹�
	std::multimap<std::string, GameEngineStructedBufferSetter>::iterator NameStariter
		= StructedBufferSetters.lower_bound(UpperString);
	std::multimap<std::string, GameEngineStructedBufferSetter>::iterator NameEnditer
		= StructedBufferSetters.upper_bound(UpperString);

	for (; NameStariter != NameEnditer; ++NameStariter)
	{
		GameEngineStructedBufferSetter& Setter = NameStariter->second;

		if (Setter.ParentShader->GetShaderType() == Type)
		{
			return Setter.Res;
		}
	}

	return nullptr;
}

void GameEngineShaderResHelper::ResClear()
{
	ConstantBufferSetters.clear();
	TextureSetters.clear();
	SamplerSetters.clear();
}