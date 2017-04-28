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


#include "WEXIntent.h"

REG_SERIALIZABLE(WEXIntent);
RTTI_IMPL( WEXIntent, IIntent );

void WEXIntent::Serialize(Json::Value &json)
{
    IIntent::Serialize(json);

    json["m_Text"] = m_Text;
    json["m_Collection"] = m_Collection;
}

void WEXIntent::Deserialize(const Json::Value &json)
{
    IIntent::Deserialize(json);

    m_Text = json["m_Text"].asString();
    m_Collection = json["m_Collection"].asString();
}

void WEXIntent::Create(const Json::Value & a_Intent, const Json::Value & a_Parse)
{
    if ( a_Intent.isMember("text") )
        m_Text = a_Intent["text"].asString();
    if ( a_Intent.isMember("goal_params") )
    {
        m_GoalParams = a_Intent["goal_params"];
        m_Collection = a_Intent["goal_params"]["answer"]["response"][0].asString();
    }
}

