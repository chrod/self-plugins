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


#ifndef SELF_WEXINTENT_H
#define SELF_WEXINTENT_H

#include "blackboard/IIntent.h"

class WEXIntent : public IIntent
{
public:
    RTTI_DECL();

    //! Types
    typedef boost::shared_ptr<WEXIntent>		SP;
    typedef boost::weak_ptr<WEXIntent>			WP;

    //! Construction
    WEXIntent()
    {}

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IIntent interface
    virtual void Create(const Json::Value & a_Intent, const Json::Value & a_Parse);

    void SetText(const std::string & a_Text)
    {
        m_Text = a_Text;
    }

    const std::string & GetText() const
    {
        return m_Text;
    }

    void SetCollection(const std::string & a_Collection)
    {
        m_Collection = a_Collection;
    }

    const std::string & GetCollection() const
    {
        return m_Collection;
    }

    void SetGoalParams(const Json::Value & a_GoalParams)
    {
        m_GoalParams = a_GoalParams;
    }

    const Json::Value & GetGoalParams() const
    {
        return m_GoalParams;
    }

private:
    //! Data
    std::string m_Text;
    std::string m_Collection;
    Json::Value m_GoalParams;
};

#endif //SELF_QUESTIONINTENT_H
