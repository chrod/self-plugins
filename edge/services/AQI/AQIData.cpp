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
	IAQI::Serialize(json);

	json["m_Units"] = m_Units;
	json["m_Language"] = m_Language;
	json["m_Location"] = m_Location;
	json["m_Long"] = m_Long;
	json["m_Lat"] = m_Lat;
}

void AQIData::Deserialize(const Json::Value & json)
{
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
}

bool AQIData::Start()
{
	if (!IAQI::Start())
		return false;
	/* Replace this with error checking - ping the sensor to see if it's up
	if (!StringUtil::EndsWith(m_pConfig->m_URL, "/posts"))
	{
		Log::Error("AQIData", "Configured URL not ended with /posts");
		return false;
	}
	*/

	if (Config::Instance()->FindService<ILocation>() == NULL)
		//Config::Instance()->GetService<AQILocation>();
		Config::Instance()->GetService<ILocation>();
	return true;
}

void AQIData::GetCurrentConditions(Location * a_Location, SendCallback a_Callback)
{	
	// Get current reading from PurpleAir sensor (JSON payload expected)
	/* Example: {"SensorId":"18:fe:34:dd:b9:8","DateTime":"2017/10/31T15:59:42z","Geo":"AirMonitor_b98",
	  "Mem":29624,"Id":10,"Adc":0.00,"lat":33.063023,"lon":-117.246445,"elevation":74.40,"version":"2.49j",
	  "uptime":54,"rssi":-33,"hardwareversion":"2.0","hardwarediscovered":"2.0+PMSX003A+BME280","current_temp_f":77,
	  "current_humidity":50,"current_dewpoint_f":56.94,"pressure":1006.31,"pm1_0_atm":42.00,"pm2_5_atm":42.25,
	  "pm10_0_atm":42.25,"pm1_0_cf_1":36.75,"pm2_5_cf_1":41.50,"pm10_0_cf_1":42.25,
	  "p_0_3_um":0.00,"p_0_5_um":0.00,"p_1_0_um":0.00,"p_2_5_um":0.00,"p_5_0_um":0.00,"p_10_0_um":0.00}*/

    std::string parameters = "/json";
	new RequestJson(this, parameters, "GET", m_Headers, EMPTY_STRING, a_Callback);
}

bool AQIData::VerifyLocation(Location ** a_Location)
{
	if ( *a_Location == NULL )
		*a_Location = new Location( m_Location, m_Lat, m_Long );
	return true;
}