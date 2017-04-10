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


#ifndef CLICKER_H
#define CLICKER_H

#include "SelfInstance.h"
#include "sensors/ISensor.h"
#include "utils/ThreadPool.h"
#include "utils/Time.h"
#include "ClickerData.h"

//! SMK-Link Clicker implementation
class Clicker : public ISensor
{
public:
    RTTI_DECL();

    Clicker() : ISensor( "Click" ), m_StopThread(false)
    {}

    //! ISensor interface
    virtual const char * GetDataType()
    {
        return "ClickerData";
    }

    //! ISensor interface
    virtual bool OnStart();
    virtual bool OnStop();
    virtual void OnPause();
    virtual void OnResume();

private:
    bool					m_StopThread;
    bool					m_ThreadStopped;

    void 					StreamingThread(void * args);
    void					SendingData(ClickerData * a_pData);
};

#endif // CLICKER_H
