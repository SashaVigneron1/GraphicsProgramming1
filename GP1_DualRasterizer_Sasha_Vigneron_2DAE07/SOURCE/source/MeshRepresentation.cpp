#pragma once
#include "pch.h"
#include "MeshRepresentation.h" 
#include "EObjParser.h"

MeshRepresentation::MeshRepresentation(ID3D11Device* pDevice, const std::string& filePath, const Elite::FVector3 position, bool isTransparent)
	: m_pIndexBuffer{ nullptr }
	, m_pVertexBuffer{ nullptr }
	, m_pVertexLayout{ nullptr }
	, m_pEffect{ nullptr }
	, m_AmountIndices{0}
	, m_pDiffuseMapVariable{ nullptr }
	, m_pNormalMapVariable{ nullptr }
	, m_pSpecularMapVariable{ nullptr }
	, m_pGlossinessMapVariable{ nullptr }
	, m_pMatViewInverseVariable{ nullptr }
	, m_pMatWorldVariable{ nullptr }
	, m_pMatWorldViewProjVariable{ nullptr }
	, m_Position{ position }

	, m_pTriangle{ new Triangle{Elite::FPoint3(position)}}

{
	
	if (!Elite::ParseOBJ(filePath, position, m_vVertexBuffer, m_vIndexBuffer))
	{
		std::cerr << "Something went wrong parsing the following file: " << filePath << std::endl;
		return;
	}

	#pragma region DirectX
	if (!isTransparent) m_pEffect = new ShadingEffect(pDevice, L"Resources/PosCol3D.fx");
	else  m_pEffect = new TransparentEffect(pDevice, L"Resources/FireFX.fx");


	//Create Vertex Layout
	HRESULT result = S_OK;
	static const uint32_t numElements{ 4 };
	D3D11_INPUT_ELEMENT_DESC vertexDesc[numElements]{};

	// Position
	vertexDesc[0].SemanticName = "POSITION";
	vertexDesc[0].Format = DXGI_FORMAT_R32G32B32A32_FLOAT;
	vertexDesc[0].AlignedByteOffset = 0;
	vertexDesc[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	// UV
	vertexDesc[1].SemanticName = "TEXCOORD";
	vertexDesc[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	vertexDesc[1].AlignedByteOffset = 16;
	vertexDesc[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	// Normal
	vertexDesc[2].SemanticName = "NORMAL";
	vertexDesc[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[2].AlignedByteOffset = 24;
	vertexDesc[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	// Tangent
	vertexDesc[3].SemanticName = "TANGENT";
	vertexDesc[3].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	vertexDesc[3].AlignedByteOffset = 36;
	vertexDesc[3].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

	//Create input layout
	D3DX11_PASS_DESC passDesc;
	m_pEffect->GetTechnique()->GetPassByIndex(0)->GetDesc(&passDesc);
	result = pDevice->CreateInputLayout(
		vertexDesc,
		numElements,
		passDesc.pIAInputSignature,
		passDesc.IAInputSignatureSize,
		&m_pVertexLayout);
	if (FAILED(result)) 
	{
		std::cerr << "Something went wrong creating the DX11 Input Layout!\n";
		return;
	}

	//Create vertex buffer
	D3D11_BUFFER_DESC bd = {};
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(Vertex_Input) * (uint32_t)m_vVertexBuffer.size();
	bd.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	D3D11_SUBRESOURCE_DATA initData = { 0 };
	initData.pSysMem = m_vVertexBuffer.data();
	result = pDevice->CreateBuffer(&bd, &initData, &m_pVertexBuffer);
	if (FAILED(result))
	{
		std::cerr << "Something went wrong creating the DX11 Vertex Buffer!\n";
		return;
	}

	//Create index buffer
	m_AmountIndices = (uint32_t)m_vIndexBuffer.size();
	bd.Usage = D3D11_USAGE_IMMUTABLE;
	bd.ByteWidth = sizeof(uint32_t) * m_AmountIndices;
	bd.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bd.CPUAccessFlags = 0;
	bd.MiscFlags = 0;
	initData.pSysMem = m_vIndexBuffer.data();
	result = pDevice->CreateBuffer(&bd, &initData, &m_pIndexBuffer);
	if (FAILED(result))
	{
		std::cerr << "Something went wrong creating the DX11 Index Buffer!\n";
		return;
	}

	m_pMatWorldViewProjVariable = m_pEffect->GetVariableByName("gWorldViewProj")->AsMatrix();
	if (!m_pMatWorldViewProjVariable->IsValid())
		std::cerr << "m_pMatWorldViewProjVariable not valid.\n";

	m_pMatWorldVariable = m_pEffect->GetVariableByName("gWorldMatrix")->AsMatrix();
	if (!m_pMatWorldVariable->IsValid())
		std::cerr << "m_pMatWorldVariable not valid.\n";

	m_pMatViewInverseVariable = m_pEffect->GetVariableByName("gViewInverseMatrix")->AsMatrix();
	if (!m_pMatViewInverseVariable->IsValid())
		std::cerr << "m_pMatViewInverseVariable not valid.\n";

	m_pDiffuseMapVariable = m_pEffect->GetVariableByName("gDiffuseMap")->AsShaderResource();
	if (!m_pDiffuseMapVariable->IsValid())
		std::cerr << "m_pDiffuseMapVariable not valid.\n";

	m_pNormalMapVariable = m_pEffect->GetVariableByName("gNormalMap")->AsShaderResource();
	if (!m_pNormalMapVariable->IsValid())
		std::cerr << "m_pNormalMapVariable not valid.\n";

	m_pSpecularMapVariable = m_pEffect->GetVariableByName("gSpecularMap")->AsShaderResource();
	if (!m_pSpecularMapVariable->IsValid())
		std::cerr << "m_pSpecularMapVariable not valid.\n";

	m_pGlossinessMapVariable = m_pEffect->GetVariableByName("gGlossinessMap")->AsShaderResource();
	if (!m_pGlossinessMapVariable->IsValid())
		std::cerr << "m_pGlossinessMapVariable not valid.\n";

	#pragma endregion
}

MeshRepresentation::~MeshRepresentation()
{
	// SoftwareRasterizer
	delete m_pTriangle;

	// DirectX
	m_pIndexBuffer->Release();
	m_pVertexBuffer->Release();
	m_pVertexLayout->Release();

	m_pMatWorldViewProjVariable->Release();
	m_pDiffuseMapVariable->Release();
	m_pNormalMapVariable->Release();
	m_pSpecularMapVariable->Release();
	m_pGlossinessMapVariable->Release();
	delete m_pEffect;
	m_pEffect = nullptr;
}

#pragma region DirectX
void MeshRepresentation::DoRender(ID3D11DeviceContext*& pDeviceContext)
{
	//Set vertex buffer
	UINT stride = sizeof(Vertex_Input);
	UINT offset = 0;
	pDeviceContext->IASetVertexBuffers(0, 1, &m_pVertexBuffer, &stride, &offset);

	//Set index buffer
	pDeviceContext->IASetIndexBuffer(m_pIndexBuffer, DXGI_FORMAT_R32_UINT, 0);

	//Set the input layout
	pDeviceContext->IASetInputLayout(m_pVertexLayout);

	//Set primitive topology
	pDeviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	//Render a triangle
	m_pEffect->GetTechnique()->GetPassByIndex((int)m_CullMode)->Apply(0, pDeviceContext);
	pDeviceContext->DrawIndexed(m_AmountIndices, 0, 0);

}

void MeshRepresentation::SetWorldViewMatrix(const float* pData)
{
	m_pMatWorldViewProjVariable->SetMatrix(pData);
}

void MeshRepresentation::SetWorldMatrix(const float* pData)
{
	m_pMatWorldVariable->SetMatrix(pData);
}

void MeshRepresentation::SetViewInverseMatrix(const float* pData)
{
	m_pMatViewInverseVariable->SetMatrix(pData);
}

void MeshRepresentation::SetDiffuseMap(ID3D11ShaderResourceView* pResourceView)
{
	if (m_pDiffuseMapVariable->IsValid())
		m_pDiffuseMapVariable->SetResource(pResourceView);
	else std::cerr << "No diffuse map variable specified.\n";
}

void MeshRepresentation::SetNormalMap(ID3D11ShaderResourceView* pResourceView)
{
	if (m_pNormalMapVariable->IsValid())
		m_pNormalMapVariable->SetResource(pResourceView);
	else std::cerr << "No normal map variable specified.\n";
}

void MeshRepresentation::SetSpecularMap(ID3D11ShaderResourceView* pResourceView)
{
	if (m_pSpecularMapVariable->IsValid())
		m_pSpecularMapVariable->SetResource(pResourceView);
	else std::cerr << "No specular map variable specified.\n";
}

void MeshRepresentation::SetGlossinessMap(ID3D11ShaderResourceView* pResourceView)
{
	if (m_pGlossinessMapVariable->IsValid())
		m_pGlossinessMapVariable->SetResource(pResourceView);
	else std::cerr << "No glossiness map variable specified.\n";
}

void MeshRepresentation::SetTextureSamplingState(TextureSamplingState renderTechnique)
{
	m_pEffect->SetTechnique(renderTechnique);
}

void MeshRepresentation::SetCullMode(CullMode cullMode)
{
	m_CullMode = cullMode;
}
#pragma endregion

#pragma region SoftwareRasterizer

const std::vector<uint32_t>& MeshRepresentation::GetIndexBuffer() const
{
	return m_vIndexBuffer;
}

const std::vector<Vertex_Input>& MeshRepresentation::GetVertexBuffer() const
{
	return m_vVertexBuffer;
}

Triangle* MeshRepresentation::GetTriangle() const
{
	return m_pTriangle;
}

void MeshRepresentation::SetLocalVertices(const std::vector<Vertex_Input>& localVertices)
{
	m_pTriangle->SetLocalVertices(localVertices);
}

#pragma endregion