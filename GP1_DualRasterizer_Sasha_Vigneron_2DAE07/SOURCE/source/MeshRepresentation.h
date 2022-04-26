#pragma once
#include "pch.h"
#include <vector>
#include "EMath.h"

#include "ShadingEffect.h"
#include "TransparentEffect.h"

#include "structs.h"
#include "Triangle.h"

class MeshRepresentation final
{
public:
	MeshRepresentation(ID3D11Device* pDevice, const std::string& filePath, const Elite::FVector3 position, bool isTransparent = false);
	~MeshRepresentation();

	MeshRepresentation(const MeshRepresentation&) = delete;
	MeshRepresentation(MeshRepresentation&&) noexcept = delete;
	MeshRepresentation& operator=(const MeshRepresentation&) = delete;
	MeshRepresentation& operator=(MeshRepresentation&&) noexcept = delete;

	Elite::FVector3 GetPosition() const { return m_Position; }

	#pragma region DirectX	
	void DoRender(ID3D11DeviceContext*& pDeviceContext);

	void SetWorldViewMatrix(const float* pData);
	void SetWorldMatrix(const float* pData);
	void SetViewInverseMatrix(const float* pData);


	void SetDiffuseMap(ID3D11ShaderResourceView* pResourceView);
	void SetNormalMap(ID3D11ShaderResourceView* pResourceView);
	void SetSpecularMap(ID3D11ShaderResourceView* pResourceView);
	void SetGlossinessMap(ID3D11ShaderResourceView* pResourceView);
	void SetTextureSamplingState(TextureSamplingState renderTechnique);

	void SetCullMode(CullMode cullMode);
	#pragma endregion

	#pragma region SoftwareRasterizer
	const std::vector<uint32_t>& GetIndexBuffer() const;
	const std::vector<Vertex_Input>& GetVertexBuffer() const;
	Triangle* GetTriangle() const;
	void SetLocalVertices(const std::vector<Vertex_Input>& localVertices);
	#pragma endregion

private:

	Elite::FVector3 m_Position;

	#pragma region DirectX
	EffectClass* m_pEffect;
	ID3D11InputLayout* m_pVertexLayout;
	ID3D11Buffer* m_pVertexBuffer;
	ID3D11Buffer* m_pIndexBuffer;
	uint32_t m_AmountIndices;

	ID3DX11EffectMatrixVariable* m_pMatWorldViewProjVariable;
	ID3DX11EffectMatrixVariable* m_pMatWorldVariable;
	ID3DX11EffectMatrixVariable* m_pMatViewInverseVariable;
	ID3DX11EffectShaderResourceVariable* m_pDiffuseMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pNormalMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pSpecularMapVariable;
	ID3DX11EffectShaderResourceVariable* m_pGlossinessMapVariable;

	CullMode m_CullMode = CullMode::BACKFACE;
	#pragma endregion

	#pragma region SoftwareRasterizer
	Triangle* m_pTriangle;
	
	std::vector<unsigned int> m_vIndexBuffer;
	std::vector<Vertex_Input> m_vVertexBuffer;
	#pragma endregion


};

