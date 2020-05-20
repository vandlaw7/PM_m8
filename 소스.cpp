#include <vector>
#include <cmath>
#include <algorithm>

#include "SolidSphere.h"
#include "Light.h"

using namespace std;

#define WIDTH 640
#define HEIGHT 640

#define boundaryX WIDTH/2
#define boundaryY HEIGHT/2

vector<SolidSphere> spheres;
Light light(boundaryX, boundaryY, boundaryX / 2, GL_LIGHT0);
enum DRAWMODE { FIRST, SECOND, THIRD, ALL };
int drawMode;




/* implement: display character function, main and sub menu functions */

void init() {
	drawMode = ALL;

	light.setAmbient(0.5, 0.5, 0.5, 1.0);
	light.setDiffuse(0.7, 0.7, 0.7, 1.0);
	light.setSpecular(1.0, 1.0, 1.0, 1.0);

	Material mtl1;
	mtl1.setEmission(0.1, 0.1, 0.1, 1);
	mtl1.setAmbient(0.3, 0.3, 0.1, 1);
	mtl1.setDiffuse(0.5, 0.5, 0.5, 1);
	mtl1.setSpecular(1.0, 1.0, 1.0, 1);
	mtl1.setShininess(10);

	Material mtl2(mtl1), mtl3(mtl1);
	mtl2.setAmbient(0.1, 0.3, 0.3, 1);
	mtl3.setAmbient(0.3, 0.1, 0.3, 1);

	SolidSphere sphere1(50, 100, 100);
	sphere1.setCenter(Vector3f(0, -200, 0));
	sphere1.setVelocity(Vector3f(5, 5, 0));
	sphere1.setMTL(mtl1);
	spheres.push_back(sphere1);

	SolidSphere sphere2(sphere1);
	sphere2.setCenter(Vector3f(100, 200, 0));
	sphere2.setVelocity(Vector3f(-7, 4, 0));
	sphere2.setMTL(mtl2);
	spheres.push_back(sphere2);

	SolidSphere sphere3(sphere1);
	sphere3.setCenter(Vector3f(-100, 0, 0));
	sphere3.setVelocity(Vector3f(10, -10, 0));
	sphere3.setMTL(mtl3);
	spheres.push_back(sphere3);
}

SolidSphere move_recur(SolidSphere sphere, float time = 1.0) {
	SolidSphere sph = SolidSphere(sphere);
	Vector3f cen = sph.getCenter();
	Vector3f vel = sph.getVelocity();
	float rad = sph.getRadius();

	Vector3f cenF = cen + vel;
	// touch
	bool lt = (cenF[0] - rad < -100.0f);
	bool rt = (cenF[0] + rad > 100.0f);
	bool bt = (cenF[1] - rad < -100.0f);
	bool ut = (cenF[1] + rad > 100.0f);

	// length after rebound
	float L_reLt = 0; float L_reRt = 0; float L_reBt = 0; float L_reUt = 0;
	// time ratio before rebound
	float T_reLt = 0; float T_reRt = 0; float T_reBt = 0; float T_reUt = 0;

	if (lt) {
		L_reLt = (cenF[0] - rad) + 100.0f;
		T_reLt = time - L_reLt / vel[0];
	}
	else if (rt) {
		L_reRt = cenF[0] + rad - 100.0f;
		T_reRt = time - L_reRt / vel[0];
	}

	if (bt) {
		L_reBt = cenF[1] - rad + 100.0f;
		T_reBt = time - L_reBt / vel[1];
	}
	else if (ut) {
		L_reUt = cenF[1] + rad - 100.0f;
		T_reUt = time - L_reUt / vel[1];
	}

	//side touch : these two kinds of touch is incompatible.
	bool st = lt || rt;
	//floor touch : these two kinds of touch is incompatible.
	bool ft = bt || ut;


	if (st && !ft) {
		if (lt) {
			cenF[0] = -100.0f - L_reLt;
			vel[0] = -vel[0];
			sph.setCenter(cenF);
			sph.setVelocity(vel);
			return sph;
		}
		else if (rt) {
			cenF[0] = 100.0f - L_reRt;
			vel[0] = -vel[0];
			sph.setCenter(cenF);
			sph.setVelocity(vel);
			return sph;
		}
	}
	else if (!st && ft) {
		if (bt) {
			cenF[1] = -100.0f - L_reBt;
			vel[1] = -vel[1];
			sph.setCenter(cenF);
			sph.setVelocity(vel);
			return sph;
		}
		else if (ut) {
			cenF[1] = 100.0f - L_reUt;
			vel[1] = -vel[1];
			sph.setCenter(cenF);
			sph.setVelocity(vel);
			return sph;
		}
	}
	else if (st && ft) { // 매우 난해하니 주의!
		if (lt && bt) {
			if (T_reLt <= T_reBt) {
				cen[0] = -100 + rad;
				vel[0] = -vel[0];
				sph.setCenter(cenF);
				sph.setVelocity(vel);
				return move_recur(sph, 1 - T_reLt);
			}
			else {
				cen[1] = -100 + rad;
				vel[1] = -vel[1];
				sph.setCenter(cenF);
				sph.setVelocity(vel);
				return move_recur(sph, 1 - T_reBt);
			}
		}
		else if (lt && ut) {
			if (T_reLt <= T_reUt) {
				cen[0] = -100 + rad;
				vel[0] = -vel[0];
				sph.setCenter(cenF);
				sph.setVelocity(vel);
				return move_recur(sph, 1 - T_reLt);
			}
			else {
				cen[1] = 100 - rad;
				vel[1] = -vel[1];
				sph.setCenter(cenF);
				sph.setVelocity(vel);
				return move_recur(sph, 1 - T_reBt);
			}
		}
		else if (rt && bt) {
			if (T_reRt <= T_reBt) {
				cen[0] = 100 - rad;
				vel[0] = -vel[0];
				sph.setCenter(cenF);
				sph.setVelocity(vel);
				return move_recur(sph, 1 - T_reLt);
			}
			else {
				cen[1] = - 100 + rad;
				vel[1] = -vel[1];
				sph.setCenter(cenF);
				sph.setVelocity(vel);
				return move_recur(sph, 1 - T_reBt);
			}
		}
		else if (rt && ut) {
			if (T_reRt <= T_reUt) {
				cen[0] = 100 - rad;
				vel[0] = -vel[0];
				sph.setCenter(cenF);
				sph.setVelocity(vel);
				return move_recur(sph, 1 - T_reLt);
			}
			else {
				cen[1] = 100 - rad;
				vel[1] = -vel[1];
				sph.setCenter(cenF);
				sph.setVelocity(vel);
				return move_recur(sph, 1 - T_reBt);
			}
		}
	}


}

void idle() {
	/* implement: boundary collision handling */
	for (int i = 0; i < spheres.size(); i++) {

		SolidSphere sphere = spheres[i];
		SolidSphere temp = move_recur(sphere);
		spheres[i] = temp;

	}
}

void renderScene() {
	glClearColor(0, 0, 0, 0);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	// 좌표계 바꿔놓음
	glOrtho(-boundaryX, boundaryX, -boundaryY, boundaryY, -100.0, 100.0);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	// Draw 2D
	/* implement: display id and name */

	// Draw 3D
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_LIGHTING);
	glEnable(light.getID());
	light.draw();

	/* implement: draw spheres */
	switch (drawMode)
	{
	case FIRST: spheres[0].draw();
		break;
	case SECOND: spheres[1].draw();
		break;
	case THIRD: spheres[2].draw();
		break;
	case ALL: spheres[0].draw(); spheres[1].draw(); spheres[2].draw();
		break;
	default:
		break;
	}

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_LIGHTING);
	glDisable(light.getID());

	glutSwapBuffers();
}

int main(int argc, char** argv) {
	// init GLUT and create Window
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
	glutInitWindowPosition(650, 300);
	glutInitWindowSize(WIDTH, HEIGHT);
	glutCreateWindow("Draw bouncing spheres");
	init();

	/* implement: popup menu */


	// register callbacks
	glutDisplayFunc(renderScene);
	glutIdleFunc(idle);

	// enter GLUT event processing cycle
	glutMainLoop();

	return 0;
}
