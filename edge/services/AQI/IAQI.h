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


#ifndef SELF_IAQI_H
#define SELF_IAQI_H

#include "utils/IService.h"
#include "services/ILocation.h"
#include "SelfLib.h"

//! Service wrapper for weather 
class SELF_API IAQI : public IService
{
public:
	RTTI_DECL();

	//! Types
	typedef Delegate<const Json::Value &>			SendCallback;

	//! Construction
	IAQI( const std::string & a_ServiceId, AuthType a_AuthType = AUTH_BASIC ) : IService( a_ServiceId, a_AuthType )
	{}

	virtual void GetCurrentConditions( Location * a_Location, SendCallback a_Callback) = 0;

	static void CelsiusToFahrenheit( const float & a_Celsius, float & a_Fahrenheit);
	static void ConcentrationToCategory( const float & a_Concentration, std::string & a_Category);
};

#endif
