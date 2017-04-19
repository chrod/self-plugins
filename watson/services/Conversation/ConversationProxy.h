/* ***************************************************************** */
/*                                                                   */
/* IBM Confidential                                                  */
/* OCO Source Materials                                              */
/*                                                                   */
/* (C) Copyright IBM Corp. 2001, 2014                                */
/*                                                                   */
/* The source code for this program is not published or otherwise    */
/* divested of its trade secrets, irrespective of what has been      */
/* deposited with the U.S. Copyright Office.                         */
/*                                                                   */
/* ***************************************************************** */

#ifndef WDC_CONVERSATION_PROXY_H
#define WDC_CONVERSATION_PROXY_H

#include "classifiers/TextClassifier.h"
#include "blackboard/Text.h"

class Conversation;
struct ConversationResponse;

class ConversationProxy : public ITextClassifierProxy
{
public:
	RTTI_DECL();

	//! Construction
	ConversationProxy();
	virtual ~ConversationProxy();

	//! ISerializable interface
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! ITextClassifierProxy interface
	virtual void Start();
	virtual void Stop();

	virtual void ClassifyText( Text::SP a_spText, Delegate<ClassifyResult *> a_Callback );
	void		 OnEmotionalState(const ThingEvent & a_ThingEvent);
	void		 OnEntity(const ThingEvent & a_ThingEvent);

private:
	//! Data
	std::string						m_ServiceId;					// ID for the conversation service to use
	std::string						m_WorkspaceKey;					// ID of the key in the configuration for our workspace ID
	std::string						m_WorkspaceId;					// ID of the workspace to use
	std::string						m_IntentOverride;				// A tag to use to override the intent from Conversation
	std::string						m_PreCacheFile;					//!< filename of phrases to pre-cache
	Json::Value						m_Context;						// Custom context data
	Json::Value						m_MergedContext;
	Json::Value						m_RecognizedObjects;
	std::string						m_EntityParentGUID;
	bool 							m_bUseCache;

	Conversation *					m_pConversation;
	float							m_EmotionalState;
	std::string						m_PreviousIntent;

	//! Helper request object
	class Request
	{
	public:
		Request( ConversationProxy * a_pConversationProxy, Text::SP a_spText, Delegate<ClassifyResult *> a_Callback );

		void OnTextClassified( ConversationResponse * a_pResponse );

	private:
		ConversationProxy *                 m_pProxy;
		Text::SP                         	m_spText;
		Delegate<ClassifyResult *>     		m_Callback;
		bool								m_bTextClassified;
	};
};

#endif // WDC_CONVERSATION_PROXY_H