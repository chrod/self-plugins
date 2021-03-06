/**
* Copyright 2017 IBM Corp. All Rights Reserved.
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


#ifndef SOTA_SPEECH_GESTURE_H
#define SOTA_SPEECH_GESTURE_H

#include "gestures/SpeechGesture.h"
#include "utils/Sound.h"

struct Voices;

//! This gesture wraps DirectSound so the local windows box can speak.
class SotaSpeechGesture : public SpeechGesture
{
public:
	RTTI_DECL();

	//! Construction
	SotaSpeechGesture() : m_pVoices( NULL )
	{}

	//! IGesture interface
	virtual bool Start();
	virtual bool Execute(GestureDelegate a_Callback, const ParamsMap & a_Params);
	virtual bool Abort();

private:
	void StartSpeech();
	void OnVoices(Voices * a_pVoices);
	void OnSpeechData(Sound * a_pSound);
	void OnPlaySpeech(Sound * a_pSound);
	void OnSpeechDone();

	Voices *					m_pVoices;
};


#endif //SPEECH_GESTURE_H
