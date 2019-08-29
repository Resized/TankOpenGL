#include "glut.h"
#include <math.h>

const double PI = 4 * atan(1.0);

double dx = 0, dy = 0; // translation
double sx = 1, sy = 1; // scaling
double angle = 0;
double direction = 1;
double groundFrequancy = 5;
double cannon_angle = 20;
double shell_force = 0.4;
double shell_time = 0;
double shell_Vx;
double shell_Vy;
double shell_x0 = 0;
double shell_xt;
double shell_y0 = 0;
double shell_yt;
double shell_angle = 0;
double shell_acceleration = 0.1;
int shoot_cannon = false;
int setxy = false;
int shell_explode = false;
double explosion_x = 0;
double explosion_y = 0;
double explode_time = 0;


void init()
{
	glClearColor(0, 0, 0.3, 0);// color of window background
	glOrtho(-1, 1, -1, 1, -1, 1); // set the coordinates system

	shell_Vx = shell_force * cos(shell_angle);
	shell_Vy = shell_force * sin(shell_angle);
	shell_xt = shell_x0 + shell_Vx * shell_time;
	shell_yt = shell_y0 + shell_Vy * shell_time + cannon_angle * shell_time * shell_time;
}

void drawWheel()
{
	double alpha, teta = PI / 20;
	double x, y;

	glBegin(GL_LINE_LOOP);
	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cos(alpha);
		y = sin(alpha);
		glVertex2d(x, y);
	}
	glEnd();

	teta = PI / 8;
	glBegin(GL_LINES);
	for (alpha = 0; alpha <= 2 * PI; alpha += teta)
	{
		x = cos(alpha);
		y = sin(alpha);
		glVertex2d(x, y);
		glVertex2d(0, 0);
	}
	glEnd();

}

void drawCannon()
{
	glBegin(GL_LINE_STRIP);
	glVertex2d(0, 0.04);
	glVertex2d(-0.8, 0.04);
	glVertex2d(-0.8, -0.04);
	glVertex2d(0, -0.04);
	glEnd();
}

void drawTank()
{
	glBegin(GL_LINE_LOOP);
	glVertex2d(-1, 0.25);
	glVertex2d(1, 0.25);
	glVertex2d(0.7, -0.25);
	glVertex2d(-0.7, -0.25);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(-0.6, 0.25);
	glVertex2d(0.7, 0.25);
	glVertex2d(0.65, 0.4);
	glVertex2d(-0.4, 0.4);
	glEnd();

	glBegin(GL_LINE_LOOP);
	glVertex2d(-0.3, 0.4);
	glVertex2d(0.6, 0.4);
	glVertex2d(0.55, 0.5);
	glVertex2d(-0.2, 0.5);
	glEnd();

	// right up wheel
	glPushMatrix();
	glTranslated(0.82, 0.15, 0);
	glScaled(0.1, 0.1, 1);
	glRotated(angle, 0, 0, -direction);
	drawWheel();
	glPopMatrix();

	// left up wheel
	glPushMatrix();
	glTranslated(-0.82, 0.15, 0);
	glScaled(0.1, 0.1, 1);
	glRotated(angle, 0, 0, -direction);
	drawWheel();
	glPopMatrix();

	//mid 1
	glPushMatrix();
	glTranslated(-0.4, 0.18, 0);
	glScaled(0.07, 0.07, 1);
	glRotated(angle, 0, 0, -direction);
	drawWheel();
	glPopMatrix();

	//mid 2
	glPushMatrix();
	glTranslated(0.0, 0.18, 0);
	glScaled(0.07, 0.07, 1);
	glRotated(angle, 0, 0, -direction);
	drawWheel();
	glPopMatrix();

	//mid 3
	glPushMatrix();
	glTranslated(0.4, 0.18, 0);
	glScaled(0.07, 0.07, 1);
	glRotated(angle, 0, 0, -direction);
	drawWheel();
	glPopMatrix();

	glPushMatrix();
	{
		glTranslated(0, -0.05, 0);
		glScaled(0.2, 0.2, 1);

		glPushMatrix();
		glTranslated(-3, 0, 0);
		glRotated(angle, 0, 0, direction);
		drawWheel();
		glPopMatrix();

		glPushMatrix();
		glTranslated(-1, 0, 0);
		glRotated(angle, 0, 0, direction);
		drawWheel();
		glPopMatrix();

		glPushMatrix();
		glTranslated(1, 0, 0);
		glRotated(angle, 0, 0, direction);
		drawWheel();
		glPopMatrix();

		glPushMatrix();
		glTranslated(3, 0, 0);
		glRotated(angle, 0, 0, direction);
		drawWheel();
		glPopMatrix();
	}
	glPopMatrix();
}

void drawGround()
{
	double x, y;

	glColor3d(1, 1, 1);//white
	glBegin(GL_LINE_STRIP);
	for (x = -1; x <= 1; x += 0.01)
	{
		y = 0.1*sin(groundFrequancy * x);

		glVertex2d(x, y);
	}
	glEnd();
}

void drawDot(double shell_x0, double shell_y0)
{
	glColor3d(1, 0, 0);
	glBegin(GL_LINE_LOOP);
	glPointSize(5);
	glVertex2d(shell_x0, shell_y0);
	glVertex2d(shell_x0 - 0.01, shell_y0);
	glVertex2d(shell_x0 - 0.01, shell_y0 + 0.01);
	glVertex2d(shell_x0, shell_y0 + 0.01);
	glEnd();
}

void drawShell(double x, double y, double angle)
{
	if (!setxy)
	{
		shell_angle = angle / 2 - cannon_angle * PI / 180;
		shell_Vx = shell_force * cos(shell_angle);
		shell_Vy = shell_force * sin(shell_angle);
		shell_x0 = x;
		shell_y0 = y;
		setxy = true;
	}
	shell_xt = shell_x0 - shell_Vx * shell_time;
	shell_yt = shell_y0 - shell_Vy * shell_time - shell_acceleration * shell_time * shell_time;
	double x_ground = shell_xt;
	double y_ground = 0.1*sin(groundFrequancy * x_ground);
	if (shell_yt < y_ground)
	{
		shoot_cannon = false;
		setxy = false;
		shell_explode = true;
		explosion_x = shell_xt;
		explosion_y = shell_yt;
		return;
	}
	glBegin(GL_LINE_LOOP);
	glVertex2d(shell_xt, shell_yt);
	glVertex2d(shell_xt - 0.02, shell_yt);
	glVertex2d(shell_xt - 0.02, shell_yt + 0.01);
	glVertex2d(shell_xt, shell_yt + 0.01);
	glEnd();
}

void drawExplosion()
{
	glBegin(GL_LINE_LOOP);
	glVertex2d(-0.5, 0);
	glVertex2d(-0.1, 0.1);
	glVertex2d(0, 0.5);
	glVertex2d(0.1, 0.1);
	glVertex2d(0.5, 0);
	glVertex2d(0.1, -0.1);
	glVertex2d(0, -0.5);
	glVertex2d(-0.1, -0.1);
	glEnd();
}

void display()
{
	double y, x, beta;

	glClear(GL_COLOR_BUFFER_BIT); // clean frame buffer
	glLoadIdentity(); // starts the previous transformations to zero

	drawGround();

	// compute height of the ground
	x = 0.6 - (angle*PI / 180)*0.03;
	y = 0.1*sin(groundFrequancy * x);
	beta = atan(cos(groundFrequancy * x)); // in radian

	// tank
	glPushMatrix();
	glTranslated(x, y + 0.052, 0);
	glRotated(beta * 180 / PI / 2, 0, 0, 1);
	glScaled(direction*0.2, 0.2, 1);
	drawTank();
	glPushMatrix();
	glTranslated(-0.25, 0.45, 0);
	glRotated(-cannon_angle, 0, 0, 1);
	drawCannon();
	glPopMatrix();
	glPopMatrix();

	// shell
	if (shoot_cannon)
	{
		glPushMatrix();
		drawShell(x - 0.18 - 0.06 * sin(beta), y + 0.2 - 0.12 * sin(beta), beta);
		glPopMatrix();
	}

	if (shell_explode)
	{
		glTranslated(explosion_x, explosion_y, 0);
		glScaled(explode_time*2, explode_time*2, 1);
		glColor3d(1, 0, 0);
		drawExplosion();
		glColor3d(1, 1, 0);
		glRotated(45, 0, 0, 1);
		drawExplosion();
	}

	glutSwapBuffers(); // show all
}

void idle()
{
	if (shoot_cannon)
	{
		shell_time += 0.01;
	}
	else
	{
		shell_time = 0;
	}
	if (shell_explode)
	{
		explode_time += 0.005;
	}
	if (explode_time > 0.1)
	{
		explode_time = 0;
		shell_explode = false;
	}

	angle += 1.5*direction;
	glutPostRedisplay();
}

void keyboard(unsigned char key, int x, int y)
{
	if (key == 32) // space key ascii
	{
		shoot_cannon = true;
	}
}

void main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE);
	glutInitWindowSize(600, 600);
	glutInitWindowPosition(200, 100);
	glutCreateWindow("Tank");

	glutDisplayFunc(display);
	glutIdleFunc(idle);
	glutKeyboardFunc(keyboard);

	init();

	glutMainLoop();
}