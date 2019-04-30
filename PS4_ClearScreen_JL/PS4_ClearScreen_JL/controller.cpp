/* SIE CONFIDENTIAL
PlayStation(R)4 Programmer Tool Runtime Library Release 05.008.001
* Copyright (C) 2013 Sony Interactive Entertainment Inc.
* All Rights Reserved.
*/

#include "api_gnm/toolkit/toolkit.h"
#include "common/allocator.h"

#include "controller.h"
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <string.h>
#include <sceerror.h>
#include <sys/event.h>
#include <unistd.h>

using namespace sce;

const float ControllerContext::m_defaultDeadZone 	= 0.25;
const float ControllerContext::m_recipMaxByteAsFloat	= 1.0f / 255.0f;

ControllerContext::ControllerContext(void)
{

}

ControllerContext::~ControllerContext(void)
{

}

int ControllerContext::initialize(SceUserServiceUserId userId)
{	
	m_deadZone = m_defaultDeadZone;

	int ret = scePadInit();
	if( ret != SCE_OK )
	{
		printf("scePadInit:: Controller Initialization Failed : 0x%08X\n", ret);
		return ret;
	}

	m_handle = scePadOpen(userId, SCE_PAD_PORT_TYPE_STANDARD, 0, NULL);
	if(m_handle < 0)
	{
		printf("scePadOpen:: Controller Port Opening Failed : 0x%08X\n", m_handle);
		return m_handle;
	}

	memset(&m_temporaryPadData, 0, sizeof(m_temporaryPadData));
	m_temporaryPadData.leftStick.x = 128;
	m_temporaryPadData.leftStick.y = 128;
	m_temporaryPadData.rightStick.x = 128;
	m_temporaryPadData.rightStick.y = 128;

	memset(m_currentPadData, 0x0, sizeof(Data)*MAX_PAD_NUM);
	for(int i = 0; i< MAX_PAD_NUM; ++i)
	{
		m_currentPadData[i].leftStick.x = 128;
		m_currentPadData[i].leftStick.y = 128;
		m_currentPadData[i].rightStick.x = 128;
		m_currentPadData[i].rightStick.y = 128;
	}

	for(int i = 0; i < MAX_PAD_NUM; i++){
		m_leftStickXY[i].setX(0.0f);
		m_leftStickXY[i].setY(0.0f);
		m_rightStickXY[i].setX(0.0f);
		m_rightStickXY[i].setY(0.0f);
	}

	m_dummyStickXY.setX(0.0f);
	m_dummyStickXY.setY(0.0f);

	return SCE_OK;
}

void ControllerContext::update()
{
	Data previousPadData[MAX_PAD_NUM];

	memcpy(previousPadData, m_currentPadData, sizeof(previousPadData));

	memset(m_currentPadData, 0x0, sizeof(Data)*MAX_PAD_NUM);
	updatePadData();


	float lX, lY, rX, rY;

	for(int i = 0; i < MAX_PAD_NUM; i++){
		// ascertain button pressed / released events from the current & previous state
		m_pressedButtonData[i] = (m_currentPadData[i].buttons & ~previousPadData[i].buttons);		///< pressed button event data
		m_releasedButtonData[i] = (~m_currentPadData[i].buttons & previousPadData[i].buttons);		///< released button event data

		// Get the analogue stick values
		// Remap ranges from 0-255 to -1 > +1
		lX = (float)((int32_t)m_currentPadData[i].leftStick.x * 2 -256) * m_recipMaxByteAsFloat;
		lY = (float)((int32_t)m_currentPadData[i].leftStick.y * 2 -256) * m_recipMaxByteAsFloat;
		rX = (float)((int32_t)m_currentPadData[i].rightStick.x * 2 -256) * m_recipMaxByteAsFloat;
		rY = (float)((int32_t)m_currentPadData[i].rightStick.y * 2 -256) * m_recipMaxByteAsFloat;

		// store stick values adjusting for deadzone
		m_leftStickXY[i].setX((fabsf(lX) < m_deadZone) ? 0.0f : lX);
		m_leftStickXY[i].setY((fabsf(lY) < m_deadZone) ? 0.0f : lY);
		m_rightStickXY[i].setX((fabsf(rX) < m_deadZone) ? 0.0f : rX);
		m_rightStickXY[i].setY((fabsf(rY) < m_deadZone) ? 0.0f : rY);
	}

	LeftStick = { lX, lY };
	RightStick = { rX, rY };

	//if (rX != 0)
	//{
	//	printf("XAxis: %.10f \n", rX);
	//}
	//if (rY != 0)
	//{
	//	printf("YAxis: %.10f \n", rY);
	//}
	
	//dead zones,  touch pad resolution,  pixel density and  whether remote play is being performed by using scePadGetControllerInformation().
	//int ret = scePadGetControllerInformation(m_handle, &m_information);
}

bool ControllerContext::isButtonDown(uint32_t port, uint32_t buttons, ButtonEventPattern pattern) const
{
	if(port >= MAX_PAD_NUM){
		return false;
	}

	if (pattern == PATTERN_ANY)
	{
		if ((m_currentPadData[port].buttons & buttons) != 0)
		{	
			return true;
		} else {
			return false;
		}
	} else if (pattern == PATTERN_ALL) {
		if ((m_currentPadData[port].buttons & buttons) == buttons)
		{
			return true;
		} else {
			return false;
		}
	}
	return false;
}


bool ControllerContext::isButtonUp(uint32_t port, uint32_t buttons, ButtonEventPattern pattern) const
{
	if(port >= MAX_PAD_NUM){
		return true;
	}

	if (pattern == PATTERN_ANY)
	{
		if ((m_currentPadData[port].buttons & buttons) != 0)
		{	
			return false;
		} else {
			return true;
		}
	} else if (pattern == PATTERN_ALL) {
		if ((m_currentPadData[port].buttons & buttons) == buttons)
		{
			return false;
		} else {
			return true;
		}
	}
	return true;
}

bool ControllerContext::isButtonPressed(uint32_t port, uint32_t buttons, ButtonEventPattern pattern) const
{
	if(port >= MAX_PAD_NUM){
		return false;
	}

	if (pattern == PATTERN_ANY)
	{
		if ((m_pressedButtonData[port] & buttons) != 0)
		{	
			return true;
		} else {
			return false;
		}
	} else if (pattern == PATTERN_ALL) {
		if ((m_pressedButtonData[port] & buttons) == buttons)
		{
			return true;
		} else {
			return false;
		}
	}
	return false;
}

bool ControllerContext::isButtonReleased(uint32_t port, uint32_t buttons, ButtonEventPattern pattern) const
{
	if(port >= MAX_PAD_NUM){
		return false;
	}

	if (pattern == PATTERN_ANY)
	{
		if ((m_releasedButtonData[port] & buttons) != 0)
		{	
			return true;
		} else {
			return false;
		}
	} else if (pattern == PATTERN_ALL) {
		if ((m_releasedButtonData[port] & buttons) == buttons)
		{
			return true;
		} else {
			return false;
		}
	}
	return false;
}

const Vector2& ControllerContext::getLeftStick(uint32_t port) const
{
	if(port >= MAX_PAD_NUM){
		return m_dummyStickXY;
	}

	return m_leftStickXY[port];
}

const Vector2& ControllerContext::getRightStick(uint32_t port) const
{
	if(port >= MAX_PAD_NUM){
		return m_dummyStickXY;
	}

	return m_rightStickXY[port];
}

void ControllerContext::setDeadZone(float deadZone)
{
	m_deadZone = deadZone;
}


void ControllerContext::updatePadData(void)
{
	for(int i = 0; i < MAX_PAD_NUM; i++){
		ScePadData data;
		int ret = scePadReadState(m_handle, &data);
		if(ret == SCE_OK){
			m_currentPadData[i].buttons = data.buttons;
			m_currentPadData[i].leftStick.x = data.leftStick.x;
			m_currentPadData[i].leftStick.y = data.leftStick.y;
			m_currentPadData[i].rightStick.x = data.rightStick.x;
			m_currentPadData[i].rightStick.y = data.rightStick.y;
			m_currentPadData[i].analogButtons.l2 = data.analogButtons.l2;
			m_currentPadData[i].analogButtons.r2 = data.analogButtons.r2;
			m_currentPadData[i].connected = data.connected;
		}
	}
}

int ControllerContext::ClosePort()
{
	int ret = scePadClose(m_handle);
	if (ret != 0)
	{
		printf("scePadClose:: Controller Close Failed : 0x%08X\n", ret);
		return ret;
	}

	return 0;
}