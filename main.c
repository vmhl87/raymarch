#include <stdlib.h>
#include "tgui.h"
#include "tgin.h"
#include "vec.h"

#define ITER 30
#define TOL .01

#define SQ(x) (x)*(x)

// distance from position p to sphere at (x, y, z) with radius r
float sphere(struct vec3 *p, float x, float y, float z, float r){
	// literally just distance function offset by radius
	return sqrtf(SQ(p->x - x) + SQ(p->y - y) + SQ(p->z - z)) - r;
}

float cube(struct vec3 *p, float x, float y, float z, float s){
	return fmax(
				fabs(p->x - x),
				fmax(
					fabs(p->y - y),
					fabs(p->z - z)
				)
			) - s;
}

float box(struct vec3 *p, float x, float y, float s){
	return fmax(
				fabs(p->x - x),
				fabs(p->y - y)
			) - s;
}

float cyl(struct vec3 *p, float x, float y, float r){
	return sqrtf(SQ(p->x - x) + SQ(p->y - y)) - r;
}

// to unite two different distance functions into one, we can literally
// just do min() of both of them - the shortest distance to the shapes
// will just be the smaller of the two distances
float sdf(struct vec3 *p){
	return fmin(
		// distance to a flat horizontal plane at y = 1
		1 - p->y,
		fmin(
			// various spheres
			sphere(p, 1, 1, 4, .3),
			fmin(
				sphere(p, -2, 1, 8, .9),
				fmin(
					sphere(p, 5, -3, 20, 3),
					fmin(
						sphere(p, 7, -5, 18, 1),
						fmin(
							sphere(p, -15, -17, 40, 0.3),
							fmin(
								cyl(p, -2, -0.3, 0.03),
								fmin(
									cyl(p, -0.4, -0.7, 0.02),
									fmin(
										box(p, 10, -7, 0.01),
										fmin(
											cube(p, -19, -13, 40, 1),
											cube(p, 15, -2, 20, 1)
										)
									)
								)
							)
						)
					)
				)
			)
		)
	);
}

// the actual march code!
int raymarch(int x, int y){
	// screen dimensions etc
	int scrw = width-48, scrh = height-48;
	// equalize fov etc etc
	float fov = (float)(scrw < scrh ? scrw : scrh);
	// compute eye vector
	float px = (float)(x-width/2) / fov,
		  py = (float)(y-height/2) / fov;
	float err, dist = 0;
	struct vec3 p = vmk(0, 0, 0),
				d = vmk(px, py, 1),
				d2;
	vnorm(&d);
	int i;
	// iterate some number of times, or until error is small enough
	for(i=0; i<ITER; ++i){
		// determine distance between current position and terrain
		err = sdf(&p);
		// move forward by this distance
		dist += err;
		vcop(&d2, &d);
		vmul(&d2, err);
		vadd(&p, &d2);
		// if error is small enough, treat this as a collision
		if(err < TOL) break;
	}
	// compute some interpolated color based on distance, where white is
	// close and black is far
	// int col = 10 + (int)(powf(1.2, -dist) * 245);
	int col = 10 + (int)(powf(1.2, -dist/i) * 300) - (int)(powf(1.2, -dist) * 300);
	col = fmax(0, fmin(255, col));
	return to_color(col, col, col);
}

// fbgui stuff etc etc etc
int main(){
	curs_set(0);
	system("clear");

	openfb();

	center("Press enter to exit", W_CHARS/2, 0);

	shade(24, 24, width-48, height-48, raymarch);

	char t; scanf("%c", &t);

	blank();
	closefb();

	system("clear");
	curs_set(1);
}
