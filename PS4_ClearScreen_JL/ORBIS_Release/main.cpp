#include <gnmx.h>
#include <video_out.h>

#include "common/allocator.h"
#include "api_gnm/toolkit/toolkit.h"

#include "Render.h"
#include "Model.h"
#include "Utils.h"
#include "controller.h"
#include "CCamera.h"

using namespace sce;
using namespace sce::Gnmx;
using namespace std;

ControllerContext g_controllerContext;
SceUserServiceUserId g_userID;

int main()
{
	Model* SphereModel = new Model(ModelType::kSphere, "/app0/mytextures.gnf", Vector3(5.0f, 0.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 1.0f, 0.0f), 0.0f);
	Model* CubeModel = new Model(ModelType::kCube, "/app0/cat.gnf", Vector3(-5.0f, 0.0f, 0.0f), Vector3(2.0f, 2.0f, 2.0f), Vector3(1.0f, 0.0f, 0.0f), 0.0f);
	Model* Model2 = new Model(ModelType::kTriangle, "/app0/normalmap.gnf", Vector3(-10.0f, 0.0f, 0.0f), Vector3(2.0f, 2.0f, 2.0f), Vector3(0.0f, 1.0f, 0.0f), 0.0f);
	Model* Model3 = new Model(ModelType::kQuad, "/app0/kanna.gnf", Vector3(10.0f, 0.0f, 0.0f), Vector3(2.0f, 2.0f, 2.0f), Vector3(0.0f, 1.0f, 0.0f), 0.0f);
	Model* CubeMap = new Model(ModelType::kCube, "/app0/cubemap3.gnf", Vector3(0.0f, 0.0f, 0.0f), Vector3(1000.0f, 1000.0f, 1000.0f), Vector3(0.0f, 0.0f, 1.0f), 0.0f);
	Model* TerrainModel = new Model(ModelType::kTerrain, "/app0/cat.gnf", Vector3(0.0f, -100.0f, 0.0f), Vector3(1.0f, 1.0f, 1.0f), Vector3(0.0f, 0.0f, 1.0f), 0.0f);
	
	SphereModel->genFetchShaderAndOffsetCache("/app0/shader_vv.sb", "/app0/shader_p.sb");
	CubeModel->genFetchShaderAndOffsetCache("/app0/shader_vv.sb", "/app0/shader_p.sb");
	Model2->genFetchShaderAndOffsetCache("/app0/shader_vv.sb", "/app0/shader_p.sb");
	Model3->genFetchShaderAndOffsetCache("/app0/shader_vv.sb", "/app0/shader_p.sb");
	TerrainModel->genFetchShaderAndOffsetCache("/app0/NoTexshader_vv.sb", "/app0/NoTexshader_p.sb");
	CubeMap->genFetchShaderAndOffsetCache("/app0/CMshader_vv.sb", "/app0/CMshader_p.sb");
		
	sceUserServiceInitialize(NULL);
	int ret = sceUserServiceGetInitialUser(&g_userID);
	if (ret < SCE_OK)
	{
		printf("sceUserServiceGetInitialUser failed: 0x%08X\n", ret);
		return ret;
	}
	
	ret = g_controllerContext.initialize(g_userID);
	if (ret < SCE_OK)
	{
		printf("controller initialization failed: 0x%08X\n", ret);
		return ret;
	}	
	
	for (uint32_t frameIndex = 0; frameIndex < 10000; ++frameIndex)
	{	
		// Camera
		{
			// Camera movement
			{
				CCamera* Camera = CCamera::GetInstance();

				// Forward Component
				float fXForward = Camera->GetForwardVector().getX() * -g_controllerContext.LeftStick.y;
				float fYForward = Camera->GetForwardVector().getY() * -g_controllerContext.LeftStick.y;
				float fZForward = Camera->GetForwardVector().getZ() * -g_controllerContext.LeftStick.y;

				// Right component
				float fXRight = Camera->GetRightVector().getX() * -g_controllerContext.LeftStick.x;
				float fYRight = Camera->GetRightVector().getY() * -g_controllerContext.LeftStick.x;
				float fZRight = Camera->GetRightVector().getZ() * -g_controllerContext.LeftStick.x;

				// Combined position difference
				float fXCombined = fXForward + fXRight;
				float fYCombined = fYForward + fYRight;
				float fZCombined = fZForward + fZRight;

				Vector3 vec3_Final = Vector3(fXCombined, fYCombined, fZCombined);

				float fCameraSpeed = 0.1f;
				vec3_Final *= fCameraSpeed;

				// New final position
				CCamera::GetInstance()->m_vec3_CameraPos += vec3_Final;
			}

			// Camera rotation
			{				
				// Up/Down Rotation
				CCamera::GetInstance()->m_fYAngle -= (g_controllerContext.RightStick.y / 40.0f);

				// Left/Right Rotation
				CCamera::GetInstance()->m_fXAngle -= (g_controllerContext.RightStick.x / 40.0f);
			}

			CCamera::GetInstance()->Process();
		}		

		// Object rotation
		{
			// Object one rotation
			if (g_controllerContext.isButtonDown(0, BUTTON_LEFT))
			{
				SphereModel->angle -= 0.01f;
			}
			if (g_controllerContext.isButtonDown(0, BUTTON_RIGHT))
			{
				SphereModel->angle += 0.01f;
			}

			// Object two rotation
			if (g_controllerContext.isButtonDown(0, BUTTON_UP))
			{
				CubeModel->angle -= 0.01f;
			}
			if (g_controllerContext.isButtonDown(0, BUTTON_DOWN))
			{
				CubeModel->angle += 0.01f;
			}
		}
		
		//printf("%d \n", frameIndex);
		g_controllerContext.update();		

		Render::GetInstance()->StartRender();
		Render::GetInstance()->SetPipelineState();
			
		SphereModel->Draw();
		CubeModel->Draw();			
		Model2->Draw();
		Model3->Draw();
		CubeMap->Draw();
		TerrainModel->Draw();

		Render::GetInstance()->EndRender();
	}
	
	// Cleanup
	CCamera::GetInstance()->Destroy();
	Render::GetInstance()->Destroy();

	return 0;
}