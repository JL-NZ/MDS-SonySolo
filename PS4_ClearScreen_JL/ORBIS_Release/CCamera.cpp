//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2017 Media Design School
//
// File Name	:	CCamera.cpp
// Description	:	main implementation for CCamera
// Author		:	Jasper Lyons & James Monk
// Mail			:	jjlyonsschool@gmail.com & james.mon7482@mediadesign.school.nz
//

// Library Includes //

// Local Includes //

// This Includes //
#include "CCamera.h"
#include <math.h>

// Static Variables //
CCamera* CCamera::p_Self = nullptr;

// Static Function Prototypes //

// Types //
using namespace std;

// Constructor //
CCamera::CCamera()
{
	m_vec3_CameraPos = Vector3(0.0f, 0.0f, -20.0f);
	//m_vec3_CameraPos = Vector3(0.0f, 0.0f, 0.0f);

	m_vec3_ForwardVector = Vector3(0.0f, 0.0f, 0.0f);
	m_vec3_RightVector = Vector3(0.0f, 0.0f, 0.0f);
	m_fXAngle = 0;
	m_fYAngle = 0;

	mat4_projection = Matrix4::perspective(3.14f / 4.0f, 1920.0f / 1080.0f, 1.0f, 1000.0f);
}

// Destructor //
CCamera::~CCamera()
{
}

// Singleton functions
CCamera * CCamera::GetInstance()
{
	if (!p_Self)
	{
		p_Self = new CCamera();
	}
	return p_Self;
}

void CCamera::Destroy()
{
	delete p_Self;
	p_Self = nullptr;
}

// Regular functions
void CCamera::Process()
{
	// Look at location updating
	{
		// Clamp values		
		m_fYAngle = std::fmin(m_fYAngle, 1.57);
		m_fYAngle = std::fmax(m_fYAngle, -1.57);

		// Set forward vector
		m_vec3_ForwardVector =
			Vector3(
				sin(m_fXAngle), // X
				sin(m_fYAngle), // Y
				cos(m_fXAngle) // Z
			);

		// Set right vector
		m_vec3_RightVector =
			Vector3(sin(m_fXAngle + 1.5708), sin(m_fYAngle), cos(m_fXAngle + 1.5708)); //1.5708

		// Set target position vector
		m_vec3_TargetPos = m_vec3_CameraPos + m_vec3_ForwardVector;
	}

	// View
	mat4_view = Matrix4::lookAt(
		Point3(m_vec3_CameraPos.getX(), m_vec3_CameraPos.getY(), m_vec3_CameraPos.getZ()),
		Point3(m_vec3_TargetPos.getX(), m_vec3_TargetPos.getY(), m_vec3_TargetPos.getZ()),
		Vector3(0.0f, 1.0f, 0.0f));	
}

Matrix4 CCamera::GetView()
{
	return mat4_view;
}

Matrix4 CCamera::GetProjection()
{
	return mat4_projection;
}

Vector3 CCamera::GetForwardVector()
{
	return m_vec3_ForwardVector;
}

Vector3 CCamera::GetRightVector()
{
	return m_vec3_RightVector;
}

