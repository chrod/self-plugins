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


#ifndef WDC_PERSONALITY_INSIGHTS_H
#define WDC_PERSONALITY_INSIGHTS_H

#include "utils/Delegate.h"
#include "utils/DataCache.h"
#include "utils/IService.h"
#include "DataModels.h"

class PersonalityInsights : public IService
{
public:
    RTTI_DECL();

    //! Types
    typedef Delegate<Profile *>	OnMessage;

    //! Construction
    PersonalityInsights();

    //! ISerializable
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();

    /**
     * This function infers individuals' intrinsic personality characteristics based on the email, tweets or blogs.
     * It outputs the person's personality in a list of top traits like anger, hatred, etc.
     */
    void GetProfile( const std::string & a_Text, OnMessage a_Callback );

private:
	//! Data
    std::string m_Version;
};


#endif //SELF_PROFILE_H
