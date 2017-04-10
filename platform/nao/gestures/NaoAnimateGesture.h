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

#ifndef NAO_ANIMATE_GESTURE_H
#define NAO_ANIMATE_GESTURE_H

#include "gestures/AnimateGesture.h"

//! This is the base class for any gesture that moves a single joint on a robot.
class NaoAnimateGesture : public AnimateGesture
{
public:
	RTTI_DECL();

	//! IGesture interface
	virtual bool Execute( GestureDelegate a_Callback, const ParamsMap & a_Params );
	virtual bool Abort();

private:
	void ExecuteAnimation();

	//! Callbacks
	void AnimateThread( std::string a_Anim );
	void AnimateDone();
};


#endif // NAO_ANIMATE_GESTURE_H
