/*
 * F_Maths.c
 *
 *  Created on: 19 févr. 2020
 *      Author: mario
 */


#include "F_Maths.h"

/*
 * F_toRad - Convert angle form degree to radius
 */
float F_Math_toRad( float angle_deg){
	return angle_deg*COEF_PI_180;
}
/*
 * F_toRad - Convert angle form radius to degree
 */
float F_Math_toDeg( float angle_rad){
	return angle_rad*COEF_180_PI;
}
/*
 *
 */
int F_Math_Sign(float nb){
	if(nb>=0){
		return 1;
	}else{
		return -1;
	}
}
/*
 * Return arg % pi
 */
void F_Math_ModuloPi(float *angleRad){
	*angleRad = 2 * atanf(tanf(*angleRad / 2));
	/*
	float angle=*angleRad;

	if(angle >  PI) angle = -( PI_X2 - angle);
	if(angle < -PI) angle =  ( PI_X2 + angle);

	*angleRad=	angle;*/
}
/*
 * Return absolute value
 */
float F_Math_Abs(float nb){
	if(nb>=0){
		return nb;
	}else{
		return -nb;
	}
}
/*
 * Return nb*nb
 */
float F_Math_Squared(float nb){
	return nb*nb;
}
