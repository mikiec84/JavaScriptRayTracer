//From basematerial.js
//     checkboard.js
//     
//This file requires color.c functions

//Basematerial helper functions
float mWrapUp(float t) {
	t = t * 2.f;
	if( t < -1.f)
		t += 2.f;
	if(t >= 1.f)
		t -= 2.f;
	return t;
}

//Checkboard helper functions
float colorEven[3] = { 1.f, 1.f, 1.f };
float colorOdd[3] = { 0.f, 0.f, 0.f };
#define CHECKERBOARD_DENSITY 0.5f

//color structure defined in color.c
void chessGetColor(float u, float v, color *c) {
	float t = mWrapUp(u * CHECKERBOARD_DENSITY) * mWrapUp(v * CHECKERBOARD_DENSITY);
	if(t < 0.f)
		cCopy(&colorEven, c);
	else
		cCopy(&colorOdd, c);
}

//Need: solid.js