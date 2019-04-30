//
// Bachelor of Software Engineering
// Media Design School
// Auckland
// New Zealand
//
// (c) 2005 - 2017 Media Design School
//
// File Name	:	CCamera.h
// Description	:	header file outlining the CCamera
// Author		:	Jasper Lyons & James Monk
// Mail			:	jjlyonsschool@gmail.com & james.mon7482@mediadesign.school.nz
//

#pragma once

#ifndef _CCAMERA_H__
#define _CCAMERA_H__

// Library Includes //
#include <iostream>

// Local Includes //
#include "Utils.h"

// Types //

// Constants //

// Prototypes //

class CCamera
{
    // Member Functions //
        public:
            CCamera();
            ~CCamera();

			static CCamera* GetInstance();
			void Destroy();

			void Process();
			Matrix4 GetView();
			Matrix4 GetProjection();

			Vector3 GetForwardVector();
			Vector3 GetRightVector();

        protected:

        private:

    // Member variables //
        public:
			Vector3 m_vec3_CameraPos;
			Vector3 m_vec3_TargetPos;

			float m_fXAngle; // Radians
			float m_fYAngle; // Radians

        protected:

        private:
			Vector3 m_vec3_ForwardVector;
			Vector3 m_vec3_RightVector;

			Matrix4 mat4_view;
			Matrix4 mat4_projection;

			static CCamera* p_Self;
};

#endif // _CCAMERA_H__

