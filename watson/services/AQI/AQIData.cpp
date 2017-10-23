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


#include "AQIData.h"
//#include "AQILocation.h"
#include "services/ILocation.h"
#include "utils/JsonHelpers.h"
#include "utils/Config.h"

REG_SERIALIZABLE( AQIData );
REG_OVERRIDE_SERIALIZABLE( IAQI, AQIData );
RTTI_IMPL(AQIData, IAQI);

AQIData::AQIData() : 
	IAQI("AQIDataV1"), 
	m_Language("en-US"),
	m_Location("Austin"),
	m_Long(-97.44f),
	m_Lat(30.16f)
{}

void AQIData::Serialize(Json::Value & json)
{
	Log::Status("AQIData", "Serialize()");
	IAQI::Serialize(json);

	json["m_Units"] = m_Units;
	json["m_Language"] = m_Language;
	json["m_Location"] = m_Location;
	json["m_Long"] = m_Long;
	json["m_Lat"] = m_Lat;
	Log::Status("AQIData", "Serialize()-done");
}

void AQIData::Deserialize(const Json::Value & json)
{
	Log::Status("AQIData", "DeSerialize()");
	IAQI::Deserialize(json);

	if (json["m_Units"].isString() )
		m_Units = json["m_Units"].asString();
	if (json["m_Language"].isString() )
		m_Language = json["m_Language"].asString();
	if (json["m_Location"].isString() )
		m_Location = json["m_Location"].asString();
	if (json["m_Long"].isNumeric() )
		m_Long = json["m_Long"].asFloat();
	if (json["m_Lat"].isNumeric() )
		m_Lat = json["m_Lat"].asFloat();
	Log::Status("AQIData", "DeSerialize()-done");
}

bool AQIData::Start()
{
	Log::Status("AQIData", "Starting AQIData!");
	if (!IAQI::Start())
		return false;

	if (!StringUtil::EndsWith(m_pConfig->m_URL, "/posts"))
	{
		Log::Error("AQIData", "Configured URL not ended with /posts");
		return false;
	}

	if (Config::Instance()->FindService<ILocation>() == NULL)
		//Config::Instance()->GetService<AQILocation>();
		Config::Instance()->GetService<ILocation>();

	return true;
}

void AQIData::GetCurrentConditions(Location * a_Location, SendCallback a_Callback)
{
	VerifyLocation(&a_Location);

	/*std::string parameters = "/v1";
	parameters += "/geocode/" + StringUtil::Format("%f", a_Location->GetLatitude()) + "/"
				  + StringUtil::Format("%f", a_Location->GetLongitude());
	parameters += "/forecast/hourly/48hour.json";
	parameters += "?units=" + m_Units;
	parameters += "&language=" + m_Language;*/

    std::string parameters = "/1";

	new RequestJson(this, parameters, "GET", m_Headers, EMPTY_STRING, a_Callback);
}

/*void AQIData::GetHourlyForecast(Location * a_Location, SendCallback a_Callback)
{
	VerifyLocation(&a_Location);

	std::string parameters = "/v1";
	parameters += "/geocode/" + StringUtil::Format("%f", a_Location->GetLatitude()) + "/"
				  + StringUtil::Format("%f", a_Location->GetLongitude());
	parameters += "/forecast/hourly/24hour.json";
	parameters += "?units=" + m_Units;
	parameters += "&language=" + m_Language;

	new RequestJson(this, parameters, "GET", m_Headers, EMPTY_STRING, a_Callback);
}
*/
/*void AQIData::GetTenDayForecast(Location * a_Location, SendCallback a_Callback)
{
	VerifyLocation(&a_Location);

	std::string parameters = "/v1";
	parameters += "/geocode/" + StringUtil::Format("%f", a_Location->GetLatitude()) + "/"
				  + StringUtil::Format("%f", a_Location->GetLongitude());
	parameters += "/forecast/daily/10day.json";
	parameters += "?units=" + m_Units;
	parameters += "&language=" + m_Language;

	new RequestJson(this, parameters, "GET", m_Headers, EMPTY_STRING, a_Callback);
}
*/
bool AQIData::VerifyLocation(Location ** a_Location)
{
	Log::Status("AQIData", "VerifyLocation()");
	if ( *a_Location == NULL )
		*a_Location = new Location( m_Location, m_Lat, m_Long );
	return true;
}

