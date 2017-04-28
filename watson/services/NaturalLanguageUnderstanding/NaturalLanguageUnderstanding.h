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


#ifndef WDC_NATURALLANGUAGEUNDERSTANDING_H
#define WDC_NATURALLANGUAGEUNDERSTANDING_H

#include "services/ILanguageParser.h"

class NaturalLanguageUnderstanding : public ILanguageParser
{
public:
	RTTI_DECL();

	//! Types
	typedef Delegate<const Json::Value &>	OnClassifyImage;
	typedef Delegate<const Json::Value &>	OnDetectFaces;

	//! Construction
	NaturalLanguageUnderstanding();

	//! ISerializable
	virtual void Serialize(Json::Value & json);
	virtual void Deserialize(const Json::Value & json);

	//! IService interface
	virtual bool Start();

	//! ILanguageParser
	virtual void GetPosTags(const std::string & a_Text, 
		Delegate<const Json::Value &> a_Callback );
	virtual void GetEntities(const std::string & a_Text,
		Delegate<const Json::Value &> a_Callback);


private:
	//!Data
	std::string		m_APIVersion;
	std::string		m_Language;
};

#endif //WDC_NATURALLANGUAGEUNDERSTANDING_H
