#include "SolidSphere.h"
#include <math.h>

SolidSphere::SolidSphere(float r, int sl, int st) {
	center.setPos(0, 0, 0);
	velocity.setPos(0, 0, 0);

	mtl.setEmission(0.1, 0.1, 0.1, 1);
	mtl.setAmbient(0.3, 0.3, 0.3, 1);
	mtl.setDiffuse(0.5, 0.5, 0.5, 1);
	mtl.setSpecular(1.0, 1.0, 1.0, 1);
	mtl.setShininess(10);

	setRadius(r);
	setSlice(sl);
	setStack(st);
}

/* implement: member functions */
void SolidSphere::setRadius(float r) {
	radius = r;
}

float SolidSphere::getRadius() const {
	return radius;
}

void SolidSphere::setSlice(float sl) {
	slice = sl;
}
void SolidSphere::setStack(float st) {
	stack = st;
}

float SolidSphere::getSlice() const {
	return slice;
}
float SolidSphere::getStack() const {
	return stack;
}

void SolidSphere::setCenter(const Vector3f& c) {
	center = c;
}
Vector3f SolidSphere::getCenter() const {
	return center;
}

void SolidSphere::setVelocity(const Vector3f& v) {
	velocity = v;
}
Vector3f SolidSphere::getVelocity() const {
	return velocity;
}

void SolidSphere::setMTL(const Material& m) {
	mtl = m;
}

void SolidSphere::move() {
}

Material SolidSphere::getMTL() const {
	return mtl;
}

void SolidSphere::draw() const {
	glPushMatrix();
	glTranslatef(center[0], center[1], center[2]);
	glutSolidSphere(radius, slice, stack);
	glPopMatrix();
}

SolidSphere::SolidSphere(const SolidSphere& sphere) {
	radius = sphere.getRadius();
	slice = sphere.getSlice();
	stack = sphere.getStack();

	center = sphere.getCenter();
	velocity = sphere.getVelocity();
	mtl = sphere.getMTL();
}