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


#ifndef WDC_CONVERSATION_H
#define WDC_CONVERSATION_H

#include "utils/Delegate.h"
#include "utils/DataCache.h"
#include "utils/IService.h"
#include "DataModels.h"

class Conversation : public IService
{
public:
    RTTI_DECL();

    //! Types
    typedef Delegate<ConversationResponse *>	OnMessage;

    //! Construction
    Conversation();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();

    //! Message text -- sends text to the Conversation Service
    void Message( 
		const std::string & a_WorkspaceId, 
		const Json::Value & a_Context,
		const std::string & a_Text,
		const std::string & a_IntentOverrideTag,
		OnMessage a_Callback,
		bool a_bUseCache = true );

private:
	//! Types
	class MessageReq
	{
	public:
		MessageReq(Conversation * a_pConversation, 
			const std::string & a_WorkspaceId,
			const Json::Value & a_Context,
			const std::string & a_Text,
			const std::string & a_IntentOverrideTag,
			OnMessage a_Callback,
			bool a_bUseCache );

	private:
		//! Callback
		void OnResponse( ConversationResponse * a_pResponse );

		//! Data
		Conversation *	m_pConversation;
		std::string		m_WorkspaceId;
		OnMessage		m_Callback;
		bool			m_bUseCache;
		std::string		m_InputHash;
	};

	//! Data
	std::string		m_APIVersion;
	std::string		m_CacheTag;
};

#endif
