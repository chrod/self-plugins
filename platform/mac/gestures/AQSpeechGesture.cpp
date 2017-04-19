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

#include "AQSpeechGesture.h"
#include "sensors/AudioData.h"
#include "skills/SkillManager.h"
#include "utils/ThreadPool.h"
#include "utils/StringUtil.h"
#include "services/ITextToSpeech.h"
#include "SelfInstance.h"

#include <stdlib.h>


REG_OVERRIDE_SERIALIZABLE(SpeechGesture, AQSpeechGesture);
REG_SERIALIZABLE(AQSpeechGesture);
RTTI_IMPL( AQSpeechGesture, SpeechGesture );

bool AQSpeechGesture::Start()
{
	if (! SpeechGesture::Start() )
		return false;

	ITextToSpeech * pTTS = SelfInstance::GetInstance()->FindService<ITextToSpeech>();
	if ( pTTS == NULL )
	{
		Log::Error( "AQSPeechGesture", "ITextToSpeech service is missing." );
		return false;
	}

    pTTS->GetVoices( DELEGATE( AQSpeechGesture, OnVoices, Voices *, this ) );
	return true;
}

void AQSpeechGesture::OnVoices( Voices * a_pVoices )
{
	m_pVoices = a_pVoices;
    if( m_pVoices )
        Log::Debug("AQSpeechGesture", "TTS returned %d available voices", m_pVoices->m_Voices.size());
}

bool AQSpeechGesture::CanExecute(const ParamsMap & a_Params)
{
    if (!a_Params.ValidPath("text"))
		return false;
	return true;
}

bool AQSpeechGesture::Execute(GestureDelegate a_Callback, const ParamsMap & a_Params)
{
    if ( PushRequest( a_Callback, a_Params ) )
        StartSpeech();

    return true;
}

void AQSpeechGesture::StartSpeech()
{
    bool bSuccess = false;
    Request * pReq = ActiveRequest();

    if ( pReq )
    {
        const std::string & text = pReq->m_Params["text"].asString();
        const std::string & gender = pReq->m_Params["gender"].asString();
        const std::string & language = pReq->m_Params["language"].asString();

        std::string voice = "en-US_MichaelVoice";
        if ( m_pVoices )
        {
            for(size_t i=0;i<m_pVoices->m_Voices.size();++i)
            {
                Voice & v = m_pVoices->m_Voices[i];
                if ( v.m_Language == language && v.m_Gender == gender )
                {
                    voice = v.m_Name;
                    break;
                }
            }
        }
        
		ITextToSpeech * pTextToSpeech = SelfInstance::GetInstance()->FindService<ITextToSpeech>();
        if ( pTextToSpeech != NULL )
        {
            // call the service to get the audio data for playing ..
            pTextToSpeech->SetVoice(voice);
            pTextToSpeech->ToSound( text, DELEGATE( AQSpeechGesture, OnSpeechData, Sound *, this ) );
            bSuccess = true;
        }
        else
        {
            Log::Error( "AQSpeechGesture", "No ITextToSpeech service available." );            
        }
    }
    else
    {
        Log::Debug("AQSpeechGesture", "Caught Active Request == NULL");
    }

	if (! bSuccess )
		OnSpeechDone();
}

bool AQSpeechGesture::Abort()
{
    if ( HaveRequests() )
    {
        Log::Debug("AQSpeechGesture", "Abort() invoked.");

        PopAllRequests();

        SelfInstance::GetInstance()->GetSensorManager()->ResumeSensorType(AudioData::GetStaticRTTI().GetName() );
        return true;
    }

    return false;
}

void AQSpeechGesture::OnSpeechData( Sound * a_pSound )
{
    if ( a_pSound )
    {
        SelfInstance::GetInstance()->GetSensorManager()->PauseSensorType(AudioData::GetStaticRTTI().GetName() );
        ThreadPool::Instance()->InvokeOnThread<Sound *>( DELEGATE( AQSpeechGesture, OnPlaySpeech, Sound *, this), a_pSound );
    }
    else
    {
        Log::Error("AQSpeechGesture", "NULL response from TTS");
		OnSpeechDone();
    }
}

void AQSpeechGesture::OnPlaySpeech( Sound * a_pSound )
{
    std::string tmpFile( Config::Instance()->GetInstanceDataPath() + "tmp.wav" );
    a_pSound->SaveToFile( tmpFile );
    delete a_pSound;

    if ( system( StringUtil::Format( "afplay %s", tmpFile.c_str() ).c_str() ) != 0 )
        Log::Error( "AQSpeechGesture", "Failed to play wav file." );

    ThreadPool::Instance()->InvokeOnMain( VOID_DELEGATE( AQSpeechGesture, OnSpeechDone, this ) );
}

void AQSpeechGesture::OnSpeechDone()
{
    SelfInstance::GetInstance()->GetSensorManager()->ResumeSensorType(AudioData::GetStaticRTTI().GetName());

	// start the next speech if we have any..
    if ( PopRequest() )
        StartSpeech();
}

