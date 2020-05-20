#pragma once

#include "Material.h"
#include "Vector3f.h"

class SolidSphere
{
public:
	SolidSphere(float r, int sl, int st);
	SolidSphere(const SolidSphere& sphere);
	void setRadius(float r);
	float getRadius() const;
	void setSlice(float sl);
	void setStack(float st);

	float getSlice() const;
	float getStack() const;


	void setCenter(const Vector3f& c);
	Vector3f getCenter() const;
	void setVelocity(const Vector3f& v);
	Vector3f getVelocity() const;
	void move();
	void setMTL(const Material& m);
	Material getMTL() const;

	void draw() const;

private:
	float radius;
	int slice;
	int stack;

	Vector3f center;
	Vector3f velocity;
	Material mtl;
};

