//What the hell is strength?
//Its pulled out of no where
//in the flog code
//(See getLightIntensity)

#include "Objects.h"

#define STRENGTH 1.f

//Where does 'strength' come from?!
//This function is never called in the javascript anyway
float getLightIntensity(Light * light, float distance) {
	if(distance >= light->intensity)
		return 0;
	return pow((light->intensity - distance) / STRENGTH, 0.2f);
}

