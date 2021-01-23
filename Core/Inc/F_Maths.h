/*
 * F_Maths.h
 *
 *  Created on: 19 févr. 2020
 *      Author: mario
 */

#ifndef INC_F_MATHS_H_
#define INC_F_MATHS_H_

#define COEF_180_PI	57.29577951308232	// 180 / PI
#define COEF_PI_180 0.017453292519943	// PI  / 180
#define PI			3.141592653589793
#define PI_X2 		6.283185307179586	// 2 * PI

#include <math.h>

struct position {
	float x;
	float y;
	float teta;
};
typedef struct position Position;

float F_Math_toRad(float angle_deg);
float F_Math_toDeg(float angle_rad);
int   F_Math_Sign(float nb);
float F_Math_ModuloPi(float angleRad);
float F_Math_Abs(float nb);
float F_Math_Squared(float nb);
float F_Math_Distance(Position pointA, Position pointB );



#endif /* INC_F_MATHS_H_ */
