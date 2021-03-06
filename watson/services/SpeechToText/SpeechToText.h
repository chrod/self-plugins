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


#ifndef WDC_SPEECH_TO_TEXT_H
#define WDC_SPEECH_TO_TEXT_H

#include <stdint.h>

#include "utils/IWebClient.h"
#include "services/ISpeechToText.h"

class SpeechToText : public ISpeechToText
{
public:
	RTTI_DECL();

	//! Types
	typedef Delegate<RecognizeResults *>			OnRecognize;
	typedef Delegate<SpeechModels *>				OnGetModels;
	typedef Delegate<const std::string & >			ErrorEvent;
	typedef std::vector<std::string>				ModelList;

	//! Construction
	SpeechToText();
	~SpeechToText();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IService interface
	virtual bool Start();
	virtual bool Stop();
	virtual void GetServiceStatus( ServiceStatusCallback a_Callback );

	//! ISpeechToText interface
	virtual bool IsListening() const;
	virtual bool IsConnected() const;

	virtual void RefreshConnections();
	virtual void SetOnError( ErrorEvent a_OnError );
	virtual void SetRecognizeModels( const ModelList & a_Models );
	virtual void SetEnableTimestamps( bool a_bEnable );
	virtual void SetEnableWordConfidence( bool a_bEnable );
	virtual void SetEnableContinousRecognition( bool a_bEnable );
	virtual void SetEnableInteriumResults( bool a_bEnable );
	virtual void SetEnableDetectSilence( bool a_bEnable, float a_fThreshold = 0.03f );
	virtual bool SetLearningOptOut(bool a_Flag);
	virtual bool StartListening(OnRecognize callback);
	virtual void OnListen(const SpeechAudioData & clip);
	virtual bool StopListening();
	virtual void Disconnected();
	virtual void GetModels(OnGetModels callback);
	virtual void Recognize(const SpeechAudioData & clip, OnRecognize callback, 
		const std::string & a_RecognizeModel = "en-US_BroadbandModel");
	virtual void Recognize(const Sound & clip, OnRecognize callback, 
		const std::string & a_RecognizeModel = "en-US_BroadbandModel");

private:
	//! Types
	typedef std::list<SpeechAudioData>		AudioQueue;

	//! This class is responsible for checking whether the service is available or not
	class ServiceStatusChecker
	{
	public:
		ServiceStatusChecker(SpeechToText* a_pService, ServiceStatusCallback a_Callback);

	private:
		SpeechToText* m_pService;
		IService::ServiceStatusCallback m_Callback;

		void OnCheckService(SpeechModels* a_pSpeechModels);
	};

	struct Connection : public boost::enable_shared_from_this<Connection>
	{
		typedef boost::shared_ptr<Connection>		SP;
		typedef boost::weak_ptr<Connection>			WP;

		Connection( SpeechToText * a_pSTT, const std::string & a_RecognizeModel = "en-US_BroadbandModel" );
		~Connection();

		SpeechToText *	m_pSTT;						
		std::string		m_RecognizeModel;		 // ID of the model to use.
		std::string		m_Language;
		IWebClient::SP	m_spListenSocket;          // use to communicate with the server
		bool			m_ListenActive;
		bool			m_AudioSent;
		bool			m_Connected;
		AudioQueue		m_ListenRecordings;
		TimerPool::ITimer::SP
						m_spKeepAliveTimer;      // ID of the keep alive co-routine
		Time			m_LastKeepAlive;
		Time			m_LastStartSent;
		int				m_RecordingHZ;
		bool			m_bReconnecting;
		TimerPool::ITimer::SP
						m_spReconnectTimer;

		void Start();
		void SendAudio(const SpeechAudioData & clip);

		bool CreateListenConnector();
		void CloseListenConnector();
		void Disconnected();
		void Refresh();

		void SendStart();
		void SendStop();
		void KeepAlive();

		void OnListenMessage( IWebSocket::FrameSP a_spFrame );
		void OnListenState( IWebClient * );
		void OnListenData( IWebClient::RequestData * );
		void OnReconnect();
	};
	typedef std::list<Connection::SP>		Connectionlist;
	typedef std::list<RecognizeResults *>	ResultList;

	//! Data
	ModelList		m_Models;				// recognize models to run
	bool			m_IsListening;
	OnRecognize		m_ListenCallback;       // Callback is set by StartListening()      
	Connectionlist	m_Connections;
	int				m_MaxAlternatives;      // maximum number of alternatives to return.
	bool			m_Timestamps;
	bool			m_WordConfidence;
	bool			m_Continous;
	bool			m_Interium;
	bool			m_DetectSilence;        // If true, then we will try not to record silence.
	float			m_SilenceThreshold;     // If the audio level is below this value, then it's considered silent.
	unsigned int	m_MaxAudioQueueSize;
	int				m_Timeout;
	bool 			m_bLearningOptOut;
	ErrorEvent		m_OnError;
	float			m_fResultDelay;			// how long do we wait for results from all streams before we return the final result
	ResultList		m_ResultList;
	TimerPool::ITimer::SP
					m_spResultTimer;

	void OnReconizeResult( RecognizeResults * a_pResult );
	void OnFinalizeResult();
};

inline bool SpeechToText::IsListening() const	
{ 
	return m_IsListening; 
}

inline bool SpeechToText::IsConnected() const
{
	bool bConnected = false;
	for( Connectionlist::const_iterator iConn = m_Connections.begin();
		iConn != m_Connections.end(); ++iConn )
	{
		bConnected |= (*iConn)->m_Connected;
	}

	return bConnected;
}

inline void SpeechToText::SetOnError( ErrorEvent a_OnError )
{
	m_OnError = a_OnError;
}

inline void SpeechToText::SetRecognizeModels( const ModelList & a_Models )
{
	m_Models = a_Models;
}

inline void SpeechToText::SetEnableTimestamps( bool a_bEnable )
{
	m_Timestamps = a_bEnable;
}

inline void SpeechToText::SetEnableWordConfidence( bool a_bEnable )
{
	m_WordConfidence = a_bEnable;
}

inline void SpeechToText::SetEnableContinousRecognition( bool a_bEnable )
{
	m_Continous = a_bEnable;
}

inline void SpeechToText::SetEnableInteriumResults( bool a_bEnable )
{
	m_Interium = a_bEnable;
}

inline void SpeechToText::SetEnableDetectSilence( bool a_bEnable, float a_fThreshold /*= 0.03f*/ )
{
	m_DetectSilence = a_bEnable;
	m_SilenceThreshold = a_fThreshold;
}

#endif
