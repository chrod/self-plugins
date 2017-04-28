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


//
// Created by John Andersen on 6/23/16.
//

#ifndef SELF_PTZCAMERA_H
#define SELF_PTZCAMERA_H

#include "utils/IService.h"

class PTZCamera : public IService
{
public:

    RTTI_DECL();

    typedef Delegate<const std::string &> GetImageObject;

    //! Construction
    PTZCamera();

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();

    IService::Request * GetImage(GetImageObject a_Callback);
    IService::Request * SetCameraCoordinates(const std::string & a_Direction, GetImageObject a_Callback);

private:


};

#endif //SELF_PTZCAMERA_H
