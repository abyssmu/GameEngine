//Input handler class
//Captures input process from windows messages

#include "Input.h"

//Default constructor
Input::Input()
{}

//Default copy constructor
Input::Input(const Input& other)
{}

//Default destructor
Input::~Input()
{}

//Initialize all key states
void Input::Initialize()
{
	//Initialize key states to false
	for (int i = 0; i < 256; ++i)
	{
		m_keys[i] = false;
	}
}

//Query current key state
bool Input::IsKeyDown(int key)
{
	//Return what state key is in
	return m_keys[key];
}

//Capture down state of key
void Input::KeyDown(int key)
{
	//If a key is down, save state
	m_keys[key] = true;
}

//Capture up state of key
void Input::KeyUp(int key)
{
	//If a key is released, save state
	m_keys[key] = false;
}

//Capture mouse input
void Input::ProcessMouse(float* rot)
{
	//Check cursor position
	if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
	{
		float speed = 0.2f;

		//Rotate about y axis
		if (mouseP.x != prevMouseP.x)
		{
			rot[1] += (mouseP.x - prevMouseP.x) * speed;
		}
		//Rotate about x axis
		if (mouseP.y != prevMouseP.y)
		{
			rot[0] += (mouseP.y - prevMouseP.y) * speed;
		}
	}

	//Save previous mouse state and capture new
	prevMouseP = mouseP;
	GetCursorPos(&mouseP);
}

//Capture movement keys
void Input::ProcessMovement(float& forw, float& lR, float& uD)
{
	float speed = 0.2f;

	//Unit X movement
	if (IsKeyDown(D))
	{
		lR += speed;
	}
	if (IsKeyDown(A))
	{
		lR -= speed;
	}

	//Unit Y movement
	if (IsKeyDown(R))
	{
		uD += speed;
	}
	if (IsKeyDown(F))
	{
		uD -= speed;
	}

	//Unit Z movement
	if (IsKeyDown(W))
	{
		forw += speed;
	}
	if (IsKeyDown(S))
	{
		forw -= speed;
	}
}

//Capture quit key
bool Input::ProcessQuit()
{
	if (IsKeyDown(ESC))
	{
		return false;
	}

	return true;
}