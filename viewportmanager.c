#include <tgmath.h>			//for sin and others
#include "globaldefs.h"
#include "matrixlib.h"
#include "viewportmanager.h"
#include "mathlib.h"			//for all the dist and dot products



IDLIST_INTERNAL_NOHASH(viewport, viewport_t);

int viewport_init(void){
	IDLIST_INIT_NOHASH(viewport, viewport_t);
	return TRUE;
}


vec3_t stockv_forward =	{0.0, 0.0, 1.0};
vec3_t stockv_up =	{0.0, 1.0, 0.0};
vec3_t stockv_right =	{1.0, 0.0, 0.0};

const inline void recalcViewMatrix(viewport_t *v){
	Matrix4x4_CreateRotate(&v->view, v->angle[2], 0.0f, 0.0f, 1.0f);
	Matrix4x4_ConcatRotate(&v->view, v->angle[0], 1.0f, 0.0f, 0.0f);
	Matrix4x4_CreateRotate(&v->view, v->angle[1], 0.0f, 1.0f, 0.0f);
	//recalc the v_ vectors
	//TODO simd
	//TODO shouldn't this be generated from the CAM matrix?
	Matrix4x4_Transform(&v->view, stockv_forward, v->v_forward);
	Matrix4x4_Transform(&v->view, stockv_up, v->v_up);
	Matrix4x4_Transform(&v->view, stockv_right, v->v_right);
	Matrix4x4_ConcatTranslate(&v->view, -v->pos[0], -v->pos[1], -v->pos[2]);
}

const inline void recalcProjectionMatrix(viewport_t *v){
	double sine, cotangent, deltaZ;
	double radians = v->fov / 2.0 * M_PI / 180.0;
	deltaZ = v->far - v->near;
	sine = sin(radians);
	if((deltaZ == 0) || (sine == 0) || (v->aspect == 0)){
		return; //we got some nasty zeros
	}
	cotangent = cos(radians) / sine;
	v->projection.m[0][0] = cotangent/v->aspect;
	v->projection.m[1][1] = cotangent;
	v->projection.m[2][2] = -(v->far + v->near) / deltaZ;
	v->projection.m[2][3] = -1.0;
	v->projection.m[3][2] = (-2.0 * v->near * v->far) / deltaZ;
	v->projection.m[3][3] = 0.0;
}
//todo make this a generic dist from plane and then inline/macro it
vec_t distPointFromNear(viewport_t *v, vec_t *p){
	vec_t *n = v->frustum[5].norm;
	vec_t dist = vec3dot(n, p) + v->frustum[5].d;
	return dist;
}

int testPointInFrustum(viewport_t *v, vec_t *p){
	int i;
	vec_t *n;
	for(i = 0; i < 6; i++){
		n = v->frustum[i].norm;
		float dist = vec3dot(n,p) + v->frustum[i].d;
		if(dist < 0.0) return FALSE;
	}
	return TRUE;
}


int testSphereInFrustum(viewport_t *v, vec_t *p, float size){
	int i;
	vec_t *n;
	float nsize = -size;
	for(i = 0; i < 6; i++){
		n = v->frustum[i].norm;
		float dist = vec3dot(n,p) + v->frustum[i].d;
		if(dist < nsize) return FALSE;
	}
	return TRUE;
}



IDLIST_CODE_NOHASH(viewport, viewport_t);
