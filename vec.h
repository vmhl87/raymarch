#include <math.h>

struct vec3{
	float x, y, z;
};

struct vec3 vmk(float x, float y, float z){
	struct vec3 ret;
	ret.x = x;
	ret.y = y;
	ret.z = z;
	return ret;
}

float vmag(struct vec3 *v){
	return sqrtf(
		v->x * v->x +
		v->y * v->y +
		v->z * v->z
	);
}

void vcop(struct vec3 *a, struct vec3 *b){
	a->x = b->x;
	a->y = b->y;
	a->z = b->z;
}

void vadd(struct vec3 *a, struct vec3 *b){
	a->x += b->x;
	a->y += b->y;
	a->z += b->z;
}

void vmul(struct vec3 *v, float f){
	v->x *= f, v->y *= f, v->z *= f;
}

void vnorm(struct vec3 *v){
	float mag = 1/vmag(v);
	vmul(v, mag);
}
