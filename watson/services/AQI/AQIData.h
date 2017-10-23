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


#ifndef WDC_AQIDATA_H
#define WDC_AQIDATA_H

#include "services/IAQI.h"
#include "services/ILocation.h"

class AQIData : public IAQI
{
public:
    RTTI_DECL();

    //! Types
    typedef Delegate<const Json::Value &>			SendCallback;

    //! Construction
    AQIData();

    //! ISerializable interface
    virtual void Serialize(Json::Value & json);
    virtual void Deserialize(const Json::Value & json);

    //! IService interface
    virtual bool Start();

	//! IWeather interface
    void GetCurrentConditions( Location * a_Location, SendCallback a_Callback );
    //void GetHourlyForecast( Location * a_Location, SendCallback a_Callback );
    //void GetTenDayForecast( Location * a_Location, SendCallback a_Callback );

private:
	//! Static
	bool VerifyLocation( Location ** a_Location);

	//! Data
    std::string m_Units;
    std::string m_Language;

	std::string m_Location;
	float		m_Long;
	float		m_Lat;
};

#endif
