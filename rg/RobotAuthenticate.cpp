/* ***************************************************************** */
/*                                                                   */
/* IBM Confidential                                                  */
/* OCO Source Materials                                              */
/*                                                                   */
/* (C) Copyright IBM Corp. 2001, 2014                                */
/*                                                                   */
/* The source code for this program is not published or otherwise    */
/* divested of its trade secrets, irrespective of what has been      */
/* deposited with the U.S. Copyright Office.                         */
/*                                                                   */
/* ***************************************************************** */

#include "services/IAuthenticate.h"
#include "RobotGateway.h"
#include "SelfInstance.h"

class RobotAuthenticate : public IAuthenticate
{
public:
	RTTI_DECL();

	RobotAuthenticate() : IAuthenticate( "RobotGatewayV1", AUTH_NONE )
	{}

	//! IAuthenticate interface
	void Authenticate( const std::string & a_EmodimentToken, 
		Delegate<const Json::Value &> a_Callback )
	{
		RobotGateway * pGateway = SelfInstance::GetInstance()->FindService<RobotGateway>();
		if ( pGateway != NULL )
		{
			Headers headers;
			headers["EmbodimentAuthorization"] = a_EmodimentToken;

			new RequestJson( pGateway, "/v1/membership/authenticateEmbodimentsAgainstOrg", "GET",
				headers, EMPTY_STRING, a_Callback );
		}
		else
		{
			Log::Error( "RobotAuthenticate", "RobotGateway service is required." );
			a_Callback( Json::Value() );
		}
	}
};

RTTI_IMPL( RobotAuthenticate, IAuthenticate );
REG_SERIALIZABLE( RobotAuthenticate );

