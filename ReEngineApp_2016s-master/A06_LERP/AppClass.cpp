#include "AppClass.h"
void AppClass::InitWindow(String a_sWindowName)
{
	super::InitWindow("Assignment  06 - LERP"); // Window Name
}

void AppClass::InitVariables(void)
{
	m_pCameraMngr->SetPositionTargetAndView(vector3(0.0f, 0.0f, 15.0f), ZERO_V3, REAXISY);

	// Color of the screen
	m_v4ClearColor = vector4(REBLACK, 1); // Set the clear color to black

	m_pMeshMngr->LoadModel("Sorted\\WallEye.bto", "WallEye");

	fDuration = 1.0f;
}

void AppClass::Update(void)
{
#pragma region Does not change anything here
	//Update the system's time
	m_pSystem->UpdateTime();

	//Update the mesh manager's time without updating for collision detection
	m_pMeshMngr->Update();
#pragma region

#pragma region Does not need changes but feel free to change anything here
	//Lets us know how much time has passed since the last call
	double fTimeSpan = m_pSystem->LapClock(); //Delta time (between frame calls)

											  //cumulative time
	static float fRunTime = 0.0f; //How much time has passed since the program started
	fRunTime += fTimeSpan;
#pragma endregion

#pragma region Your Code goes here
	//m_pMeshMngr->SetModelMatrix(IDENTITY_M4, "WallEye");
#pragma endregion

#pragma region Does not need changes but feel free to change anything here
	//Adds all loaded instance to the render list
	m_pMeshMngr->AddInstanceToRenderList("ALL");
	vector3 positions[] = { vector3(-4.0f,-2.0f, 5.0f), // array of the positions for the enemy to loop through
	
	vector3(1.0f,-2.0f, 5.0f),

	vector3(-3.0f,-1.0f, 3.0f),

	vector3(2.0f,-1.0f, 3.0f),

	vector3(-2.0f, 0.0f, 0.0f),

	vector3(3.0f, 0.0f, 0.0f),

	vector3(-1.0f, 1.0f,-3.0f),

	vector3(4.0f, 1.0f,-3.0f),

	vector3(0.0f, 2.0f,-5.0f),

	vector3(5.0f, 2.0f,-5.0f),

	vector3(1.0f, 3.0f,-5.0f) };
	static int current = 0; // the current index of the array, starting at 0
	static int next = 1; // the next index of the array, starting at 1
	static int loops = 0; // the amount of times the timer will have looped from 0 to 1, starting at 0
	

	//Indicate the FPS
	int nFPS = m_pSystem->GetFPS();

	//Print info on the screen
	m_pMeshMngr->PrintLine("");
	m_pMeshMngr->PrintLine(m_pSystem->GetAppName(), REYELLOW); // print app name
	static DWORD startTimeSystem = GetTickCount();
	DWORD timeApplication = GetTickCount() - startTimeSystem; 
	float timer = fRunTime - loops; // create a timer based on getting from the current point to the next point
	m_pMeshMngr->PrintLine("Time is: " + std::to_string(fRunTime)); // write out the run time
	matrix4 m4WallEye; // matrix for the position of the enemy
	float timerMapped = MapValue(timer, 0.0f, fDuration, 0.0f, 1.0f); // map the time from 0 to 1 to use for the lerping
	if (timerMapped > 1.0f) // timer cant go greater than 100% of its possible value
		timerMapped = 1.0f;
	vector3 v3lerp = glm::lerp(positions[current], positions[next], timerMapped); // perform the lerp to put the enemy in the proper position
	m4WallEye = glm::translate(v3lerp); // translate the enemy using the vector 3 created by the lerp
	m_pMeshMngr->SetModelMatrix(m4WallEye, "WallEye"); // set the model matrix for the enemy
	for (int i = 0; i < 11; i++) { // create the small red spheres for every position the enemy will travel to
		matrix4 m4Sphere1;
		m4Sphere1 = glm::translate(positions[i]) * glm::scale(vector3(0.1f));
		m_pMeshMngr->AddSphereToRenderList(m4Sphere1, RERED, SOLID);
	}
	
	m_pMeshMngr->Print("FPS:");
	m_pMeshMngr->Print(std::to_string(nFPS), RERED);
	//m_pMeshMngr->PrintLine("Timermap: " + std::to_string(timerMapped));
	if (timer > fDuration) { // every time the timer reaches the time it takes to move from one position to the next
		current++; // update the current position index
		next++; // update the next position index
		loops++; // update the amount of times the timer has looped from 0 to 1
	}
	if (current > 10) current = 0; // if it reaches the maximum index, put it back to zero
	if (next > 10) next = 0; // if it reaches the maximum index, put it back to zero

#pragma endregion
}

void AppClass::Display(void)
{
	//clear the screen
	ClearScreen();
	//Render the grid based on the camera's mode:
	m_pMeshMngr->AddGridToRenderListBasedOnCamera(m_pCameraMngr->GetCameraMode());
	m_pMeshMngr->Render(); //renders the render list
	m_pMeshMngr->ClearRenderList(); //Reset the Render list after render
	m_pGLSystem->GLSwapBuffers(); //Swaps the OpenGL buffers
}

void AppClass::Release(void)
{
	super::Release(); //release the memory of the inherited fields
}