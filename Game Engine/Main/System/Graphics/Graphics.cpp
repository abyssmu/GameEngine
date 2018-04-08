//Graphics class
//Used as an interface to DirectX
//Combines components of DirectX to render objects

//Current Shaders
	//ColorShader

#include "Graphics.h"

//Constructor
Graphics::Graphics()
{
	//Initialize pointers
	m_dX11 = 0;

	m_colorShader = 0;
}

//Default copy constructor
Graphics::Graphics(const Graphics& other)
{}

//Default destructor
Graphics::~Graphics()
{}

//Initialize components
bool Graphics::Initialize(int screenWidth, int screenHeight,
	HWND hwnd)
{
	//Initialize DirectX
	InitializeDirectX(screenWidth, screenHeight, hwnd);

	//Initialize shaders
	InitializeShaders(hwnd);

	return true;
}

//Reset DirectX if resolution changed
bool Graphics::ResetDX(int& screenWidth, int& screenHeight, HWND hwnd)
{
	if (!m_dX11->Resize(screenWidth, screenHeight, hwnd, 
						SCREEN_DEPTH, SCREEN_NEAR))
	{
		return false;
	}

	return true;
}

//Shutdown components
void Graphics::Shutdown()
{
	//Shutdown color shader
	if (m_colorShader)
	{
		m_colorShader->Shutdown();
		delete m_colorShader;
		m_colorShader = 0;
	}

	//Shutdown dx11
	if (m_dX11)
	{
		m_dX11->Shutdown();
		delete m_dX11;
		m_dX11 = 0;
	}
}

//Graphical frame processing
bool Graphics::Frame(double* bgcolor, DirectX::XMMATRIX viewMatrix,
					AllModelInfo* modelInfo)
{
	bool result = false;

	//Render the scene
	result = Render(bgcolor, viewMatrix, modelInfo);
	if (!result)
	{
		return false;
	}

	return true;
}

//Get DirectX device
ID3D11Device* Graphics::GetDevice()
{
	return m_dX11->GetDevice();
}

/////////////////////////////////////////////////////////
//Private
/////////////////////////////////////////////////////////

//Calculate world matrix
void Graphics::CalculateWorld(DirectX::XMMATRIX& world, AllModelInfo* modelInfo)
{
	DirectX::XMMATRIX translation = DirectX::XMMatrixTranslation((float)modelInfo->position.x,
		(float)modelInfo->position.y,
		(float)modelInfo->position.z);

	//DirectX::XMMATRIX rotation;
	//DirectX::XMMATRIX scale;

	//world = scale * rotation * translation;
	world = translation;
}

//Initialize DirectX
bool Graphics::InitializeDirectX(int screenWidth, int screenHeight, HWND hwnd)
{
	//Create and initialize DirectX11 object
	m_dX11 = new DirectX11;
	if (!m_dX11)
	{
		return false;
	}

	if (!m_dX11->Initialize(screenWidth, screenHeight,
		VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH,
		SCREEN_NEAR))
	{
		MessageBox(hwnd, "Could not initialize DX11", "Error", MB_OK);
		return false;
	}

	return true;
}

//Initialize shaders
bool Graphics::InitializeShaders(HWND hwnd)
{
	//Create and initialize color shader
	m_colorShader = new ColorShader;
	if (!m_colorShader)
	{
		return false;
	}

	if (!m_colorShader->Initialize(m_dX11->GetDevice(), hwnd))
	{
		MessageBox(hwnd, "Could not initialize color shader.", "Error",
			MB_OK);
		return false;
	}

	return true;
}

//Render model to scene and present scene
bool Graphics::Render(double* bgcolor, DirectX::XMMATRIX viewMatrix,
					AllModelInfo* modelInfo)
{
	//Clear buffers to begin scene
	m_dX11->BeginScene(bgcolor);

	//Render model
	RenderModel(modelInfo, viewMatrix);

	//Present rendered scene
	m_dX11->EndScene();

	return true;
}

//Put model information on pipeline
bool Graphics::RenderModel(AllModelInfo* modelInfo, DirectX::XMMATRIX viewMatrix)
{
	DirectX::XMMATRIX world;
	CalculateWorld(world, modelInfo);

	//Render model with color shader
	if (!m_colorShader->Render(m_dX11->GetDeviceContext(),
		modelInfo->subModelInfo->indexCount, world,
		viewMatrix, m_dX11->GetProjectionMatrix()))
	{
		return false;
	}

	//Set vertex buffer to active
	m_dX11->GetDeviceContext()->IASetVertexBuffers(0, 1,
		&modelInfo->subModelInfo->vertexBuffer, &modelInfo->subModelInfo->stride,
		&modelInfo->subModelInfo->offset);

	//Set index buffer to active
	m_dX11->GetDeviceContext()->IASetIndexBuffer(modelInfo->subModelInfo->indexBuffer,
		DXGI_FORMAT_R32_UINT, 0);

	//Set type of primitive
	m_dX11->GetDeviceContext()->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return true;
}