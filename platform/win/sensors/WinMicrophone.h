/**
* Copyright 2016 IBM Corp. All Rights Reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

#ifndef WIN_MICROPHONE_H
#define WIN_MICROPHONE_H

#include "sensors/Microphone.h"

//! DirectSound Microphone 
class WinMicrophone : public Microphone
{
public:
	RTTI_DECL();

	//! Construction
	WinMicrophone();
	~WinMicrophone();

	//! ISensor interface
	virtual bool OnStart();
	virtual bool OnStop();
	virtual void OnPause();
	virtual void OnResume();

private:
	//! Data
	volatile bool m_CaptureAudio;
	volatile bool m_CaptureStopped;
	bool m_Error;
	void * m_CaptureEvents[2];

	void CaptureAudio( void * );
	void SendAudio( AudioData * a_pData );
};

#endif
