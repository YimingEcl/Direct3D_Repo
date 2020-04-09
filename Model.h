#pragma once

#include "BindableHeader.h"
#include "Drawable.h"

class Mesh : public Drawable
{
public:
	Mesh(Graphics& gfx, const aiMesh& mesh);

	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept(!IS_DEBUG);
	void Update(Graphics& gfx, float dt) noexcept override;
	XMMATRIX GetTransformXM() const noexcept override;

private:
	struct PSObjectCBuf
	{
		alignas(16) XMFLOAT3 color = { 0.6f,0.6f,0.8f };
		float specularIntensity = 0.6f;
		float specularPower = 100.0f;
		float padding[2] = { 0.0f, 0.0f };
	} colorConst;

	using objectCBuf = PixelConstantBuffer<PSObjectCBuf>;

private:
	mutable XMFLOAT4X4 transform;
};

class Node
{
	friend class Model;

public:
	Node(std::vector<Mesh*> meshPtrs, const DirectX::XMMATRIX& transform) noexcept(!IS_DEBUG);

	void Draw(Graphics& gfx, DirectX::FXMMATRIX accumulatedTransform) const noexcept(!IS_DEBUG);

private:
	void AddChild(std::unique_ptr<Node> pChild) noexcept(!IS_DEBUG);

private:
	std::vector<std::unique_ptr<Node>> childPtrs;
	std::vector<Mesh*> meshPtrs;
	XMFLOAT4X4 transform;
};

class Model
{
public:
	Model(Graphics& gfx, const std::string fileName);

	void Draw(Graphics& gfx) const noexcept(!IS_DEBUG);
	void SpawnImguiWindow();

private:
	std::unique_ptr<Mesh> ParseMesh(Graphics& gfx, const aiMesh& mesh);
	std::unique_ptr<Node> ParseNode(const aiNode& node);

private:
	std::unique_ptr<Node> pRoot;
	std::vector<std::shared_ptr<Mesh>> meshPtrs;

	XMFLOAT4 position = XMFLOAT4(0.0f, -5.0f, 0.0f, 1.0f);
	XMFLOAT4 rotation = XMFLOAT4(0.0f, PI, 0.0f, 1.0f);
};