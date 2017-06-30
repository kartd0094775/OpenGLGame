// Rendering_FinalProject.cpp : Defines the entry point for the console application.
//

/*************
Project Name: Dinosaur Run
林凡煒: 地圖製作
紀典勳: 角色模型

Starting Date:　6/12


*************/

#include "stdafx.h"
#define PI 3.14159
#define G_ACC 0.014

enum STATE {
	NORMAL_STATE,
	JUMP_STATE,
	CROUCH_STATE,
	DEAD_STATE
};
enum DIRECTION {
	RIGHT, 
	LEFT, 
	FORWARD
};
enum TURN_ANIMATION {
	TURN_RIGHT,
	TURN_LEFT,
	TURN_NORMAL
};

int horizontal_camera = 0;


GLMmodel* grassModel;
GLMmodel* crateModel;
GLMmodel* branchModel;

CMD2Model md2Model;
animState_t animationStateMain;

GLuint texid[5];

float center_x = 0.0;
float center_z = 0.0;

bool genMapDetect = true;
int curMap = 0;
int nextMap = -1;

int life = 3;
int score = 0;
int death_animation = 0;
float run = 0.0;
float jump = 0.0;
float angle = 0.0;

STATE state;
DIRECTION direct;
TURN_ANIMATION turnAnimation;

using namespace std;

void glmLoadModel(GLMmodel* model, char* filename);
void LoadTexture(GLuint* texid, char* filename);

void Road1();
void Road1_withBarrier();
void Road1_withBranch();
void Road2();
void Life();
void GameOverTexture();

void genMap(int counter);
void drawMap(int map);

void printFont(float x, float y, char* str);
void countScore(int counter);

void jump_func(int counter);
void crouch_func(int counter);


bool CollisionDetection() {
	switch (curMap)
	{
	case 2:
		if (state == JUMP_STATE) return false;
		break;
	case 3:
		if (state == CROUCH_STATE) return false;
		break;
	default:
		return false;
		break;
	}
	return true;
}

void display() {

	GLfloat ground_ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat ground_diffuse[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat ground_specular[] = { 0.0, 0.0, 0.0, 1.0 };


	float x1 = 1.0 * sin(PI * horizontal_camera / 180.0);
	float y1 = 0;
	float z1 = 1.0 * cos(PI * horizontal_camera / 180.0);



	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();


	glRotatef(angle, 0.0, 1.0, 0.0);
	gluLookAt(center_x, 1.0, center_z, 0.0, 1.0, -1.0, 0.0, 1.0, 0.0);

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texid[1]);

	// Ground
	glPushMatrix();
	glMaterialfv(GL_FRONT, GL_AMBIENT, ground_ambient);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, ground_diffuse);
	glMaterialfv(GL_FRONT, GL_SPECULAR, ground_specular);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(0, 0); glVertex3f(-500.0, -0.1, -500.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(1, 0); glVertex3f(500.0, -0.1, -500.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(1, 1); glVertex3f(500.0, -0.1, 500.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(0, 1); glVertex3f(-500.0, -0.1, 500.0);
	glEnd();
	glPopMatrix();



	// Sky
	glPushMatrix();
	// Top
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex3f(-500.0, 1000.0, -500.0);
	glTexCoord2i(0, 1); glVertex3f(500.0, 1000.0, -500.0);
	glTexCoord2i(1, 1); glVertex3f(500.0, 1000.0, 500.0);
	glTexCoord2i(1, 0); glVertex3f(-500.0, 1000.0, 500.0);
	glEnd();
	// Forward
	glBindTexture(GL_TEXTURE_2D, texid[0]);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex3f(-500.0, 0.0, -500.0);
	glTexCoord2i(1, 0); glVertex3f(500.0, 0.0, -500.0);
	glTexCoord2i(1, 1); glVertex3f(500.0, 1000.0, -500.0);
	glTexCoord2i(0, 1); glVertex3f(-500.0, 1000.0, -500.0);
	glEnd();
	// Backward
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex3f(-500.0, 0.0, 500.0);
	glTexCoord2i(0, 1); glVertex3f(500.0, 0.0, 500.0);
	glTexCoord2i(1, 1); glVertex3f(500.0, 1000.0, 500.0);
	glTexCoord2i(1, 0); glVertex3f(-500.0, 1000.0, 500.0);
	glEnd();
	// Left
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex3f(-500.0, 0.0, -500.0);
	glTexCoord2i(0, 1); glVertex3f(-500.0, 0.0, 500.0);
	glTexCoord2i(1, 1); glVertex3f(-500.0, 1000.0, 500.0);
	glTexCoord2i(1, 0); glVertex3f(-500.0, 1000.0, -500.0);
	glEnd();
	// Right
	glBegin(GL_QUADS);
	glTexCoord2i(0, 0); glVertex3f(500.0, 0.0, 500.0);
	glTexCoord2i(1, 0); glVertex3f(500.0, 0.0, -500.0);
	glTexCoord2i(1, 1); glVertex3f(500.0, 1000.0, -500.0);
	glTexCoord2i(0, 1); glVertex3f(500.0, 1000.0, 500.0);
	glEnd();
	glPopMatrix();
	glDisable(GL_TEXTURE_2D);

	
	if (life > 0) {
		if (run >= 3.4 && run <= 3.6) {
			if (genMapDetect)
				if (CollisionDetection()) {
					life--;
					genMapDetect = false;
				}
		}
		if (run >= 7.5 && run <= 8.0 && curMap == 1 && direct == FORWARD) {
			life = 0;
		}
	}
	// Character
	if (turnAnimation == TURN_RIGHT) {
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(2.0, 0.6 + jump, -1.0);
		glScalef(0.02, 0.02, 0.02);
		glRotatef(-90.0, 0.0, 1.0, 0.0);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glRotatef(90.0, 0.0, 0.0, 1.0);
		md2Model.RenderModel(&animationStateMain);
		if (score % 7 == 0) md2Model.UpdateAnimation(&animationStateMain, 1.0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);

	}
	else if (turnAnimation == TURN_LEFT) {
		glEnable(GL_TEXTURE_2D);
		glPushMatrix();
		glTranslatef(-2.0, 0.6 + jump, -1.0);
		glScalef(0.02, 0.02, 0.02);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glRotatef(90.0, 0.0, 0.0, 1.0);
		md2Model.RenderModel(&animationStateMain);
		if (score % 7 == 0) md2Model.UpdateAnimation(&animationStateMain, 1.0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
	} else {
		glEnable(GL_TEXTURE_2D);
		//glEnable(GL_SAMPLER_2D);
		glPushMatrix();
		glTranslatef(0.0, 0.6 + jump, -2.0);
		glScalef(0.02, 0.02, 0.02);
		glRotatef(-90.0, 1.0, 0.0, 0.0);
		glRotatef(90.0, 0.0, 0.0, 1.0);
		md2Model.RenderModel(&animationStateMain);
		if (score % 7 == 0) md2Model.UpdateAnimation(&animationStateMain, 1.0);
		glPopMatrix();
		glDisable(GL_TEXTURE_2D);
		//glDisable(GL_SAMPLER_2D);
	}




	// Print the score
	glPushMatrix();
	glDisable(GL_LIGHTING);
	glDisable(GL_DEPTH_TEST);
	glColor3f(1.0, 0.0, 0.0);
	char temp[100];
	_itoa_s(score, temp, 10);
	glTranslatef(0.0, 1.0, -1.0);
	printFont(0.8, 0.5, temp);
	glEnable(GL_LIGHTING);
	glEnable(GL_DEPTH_TEST);
	glPopMatrix();
	
	// Create the map
	glPushMatrix();
	glTranslatef(0.0, 0.0, run);
	glPushMatrix();
	drawMap(curMap);
	glPopMatrix();

	if (curMap == 1) {
		glPushMatrix();
		glTranslatef(0.0, 0.0, -9.0);
		glPushMatrix();
		glTranslatef(15.0, 0.0, 0.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		drawMap(nextMap);
		glPopMatrix();
		glPushMatrix();
		glTranslatef(-5.0, 0.0, 0.0);
		glRotatef(90.0, 0.0, 1.0, 0.0);
		drawMap(nextMap);
		glPopMatrix();
		glPopMatrix();
		
	}
	else {
		glPushMatrix();
		glTranslatef(0.0, 0.0, -10.0);
		drawMap(nextMap);
		glPopMatrix();
	}
	glPopMatrix();
	
	switch (life) {
	case 0:
		state = DEAD_STATE;
		animationStateMain = md2Model.StartAnimation(DEATH_FALLFORWARD);
		life--;
		break;
	case 1:
		glPushMatrix();
		glTranslatef(-4.5, 2.5, -5.0);
		glScalef(0.5, 0.5, 0.5);
		Life();
		glPopMatrix();
		break;
	case 2:
		glPushMatrix();
		glTranslatef(-4.5, 2.5, -5.0);
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		Life();
		glPopMatrix();
		glTranslatef(0.0, -1.0, 0.0);
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		Life();
		glPopMatrix();
		glPopMatrix();
		break;
	case 3:
		glPushMatrix();
		glTranslatef(-4.5, 2.5, -5.0);
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		Life();
		glPopMatrix();
		glTranslatef(0.0, -1.0, 0.0);
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		Life();
		glPopMatrix();
		glTranslatef(0.0, -1.0, 0.0);
		glPushMatrix();
		glScalef(0.5, 0.5, 0.5);
		Life();
		glPopMatrix();
		glPopMatrix();
		break;
	default:
		break;
	}
	if (state == DEAD_STATE) {
		glPushMatrix();
		glTranslatef(0.0, 0.0, -5.0);
		glScalef(1.5, 1.5, 1.5);
		GameOverTexture();
		glPopMatrix();
	}
	glutSwapBuffers();
}

void init() {

	GLfloat ambient[] = { 0.3, 0.3, 0.3, 1.0 };
	GLfloat diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat emission[] = { 0.0, 0.0, 0.0, 1.0 };
	GLfloat specular[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat position[] = { 0.0, 1000.0, 0.0, 0.0 };

	state = NORMAL_STATE;
	direct = FORWARD;
	turnAnimation = TURN_NORMAL;

	glClearColor(0.8, 0.8, 0.8, 1.0);
	glClearDepth(1.0);
	glShadeModel(GL_SMOOTH);
	glEnable(GL_DEPTH_TEST);

	glLightfv(GL_LIGHT0, GL_AMBIENT, ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, diffuse);
	glLightfv(GL_LIGHT0, GL_EMISSION, emission);
	glLightfv(GL_LIGHT0, GL_POSITION, position);
	glLightfv(GL_LIGHT0, GL_SPECULAR, specular);
	glLightf(GL_LIGHT0, GL_SHININESS, 1.0);

	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);

	//glEnable(GL_BLEND);


	// Loadin object file
	grassModel = glmReadOBJ("model/Ferns.obj");
	if (!grassModel) exit(0);
	glmUnitize(grassModel);
	glmFacetNormals(grassModel);
	glmVertexNormals(grassModel, 90.0);


	crateModel = glmReadOBJ("model/wood-crate-flat.obj");
	if (!crateModel) exit(0);
	glmUnitize(crateModel);
	glmFacetNormals(crateModel);
	glmVertexNormals(crateModel, 90.0);

	branchModel = glmReadOBJ("model/L1.obj");
	if (!branchModel) exit(0);
	glmUnitize(branchModel);
	glmFacetNormals(branchModel);
	glmVertexNormals(branchModel, 90.0);

	
	md2Model.LoadModel("data/models/blade/Blade.md2");
	animationStateMain = md2Model.StartAnimation(RUN);
	
	
	LoadTexture(&texid[0], "texture/forest.jpg");
	LoadTexture(&texid[1], "texture/design_tech.jpg");
	LoadTexture(&texid[2], "texture/road.jpg");
	LoadTexture(&texid[3], "texture/heart.gif");
	LoadTexture(&texid[4], "texture/gameover.gif");
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);



	thread thread(countScore, 0);
	thread.join();
	genMap(0);
}
void idle() {
	glutPostRedisplay();
}
void keyboard(unsigned char key, int x, int y) {
	if (state == DEAD_STATE) return;
	switch (key) {
	case '1':
		life = 1000;
		break;
	case '2' :
		//life = 0;
		break;
	case ' ':
		jump_func(0);
		break;
	case 'C':
	case 'c':
		crouch_func(0);
		break;
	case 'A':
	case'a':
		if (run >= 2.0 && run <= 6.0 && curMap == 1) direct = LEFT;
		break;
	case 'D':
	case 'd':
		if (run >= 2.0 && run <= 6.0 && curMap == 1) direct = RIGHT;
		break;
	default:
		break;
	}
	glutPostRedisplay();
}
void mouseMove(int x, int y) {

	static int lastx = 0.0;
	static int lasty = 0.0;

	lastx = x - lastx;
	lasty = y - lasty;

	if ((abs(lastx) > 10) || (abs(lasty) > 10))
	{
		lastx = x;
		lasty = y;
		return;
	}
	horizontal_camera += 2 * lastx;

	glutPostRedisplay();
}
void reshape(int w, int h) {
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(60.0, (GLfloat)w / h, 1.0, 1000.0);
	glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();
}
int _tmain(int argc, char** argv)
{

	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_MULTISAMPLE);
	glutInitWindowSize(1400, 900);
	glutInit(&argc, argv);
	glutCreateWindow("Dinosaur Run");
	GLenum err = glewInit();
	if (GLEW_OK != err) {
		exit(0);
	}
	init();
	
	//glutPassiveMotionFunc(mouseMove);
	glutKeyboardFunc(keyboard);
	glutReshapeFunc(reshape);
	
	glutDisplayFunc(display);
	//glutIdleFunc(idle);
	glutMainLoop();


	return 0;
}

void printFont(float x, float y, char* str) {
	int len, i;
	glRasterPos2f(x, y);
	len = (int)strlen(str);
	FOR(i, len) {
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18, str[i]);
	}
}

void glmLoadModel(GLMmodel* model, char* filename) {
	model = glmReadOBJ(filename);
	if (!model) exit(0);
	glmUnitize(model);
	glmFacetNormals(model);
	glmVertexNormals(model, 90.0);
}

void GameOverTexture() {
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texid[4]);
	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 0.0); glVertex3f(-2.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 0.0); glVertex3f(2.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 1.0); glVertex3f(2.0, 2.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 1.0); glVertex3f(-2.0, 2.0, 0.0);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}
void Life() {
	glEnable(GL_TEXTURE_2D);
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_CONSTANT_ALPHA);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
	glBindTexture(GL_TEXTURE_2D, texid[3]);
	glBegin(GL_QUADS);
	glColor4f(1.0, 1.0, 1.0, 1.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 2.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 2.0, 0.0);
	glEnd();
	glDisable(GL_BLEND);
	glDisable(GL_TEXTURE_2D);
	glEnable(GL_DEPTH_TEST);
}
void Road1() {

	GLfloat grass[] = { 0.133, 0.545, 0.133, 1.0 };

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texid[2]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 0.1, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.1, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 0.1, -10.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 0.1, -10.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texid[1]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(0, 0); glVertex3f(-10.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(1, 0); glVertex3f(10.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(1, 1); glVertex3f(10.0, 0.0, -10.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(0, 1); glVertex3f(-10.0, 0.0, -10.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glMaterialfv(GL_FRONT, GL_AMBIENT, grass);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grass);
	glPushMatrix();
	glTranslatef(1.5, 0.1, -5.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glmDraw(grassModel, GLM_SMOOTH);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.5, 0.1, -3.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glmDraw(grassModel, GLM_SMOOTH);
	glPopMatrix();


}
void Road1_withBarrier() {

	GLfloat grass[] = { 0.133, 0.545, 0.133, 1.0 };
	GLfloat crate[] = { 0.871, 0.722, 0.529, 1.0 };

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texid[2]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 0.1, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.1, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 0.1, -10.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 0.1, -10.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texid[1]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(0, 0); glVertex3f(-10.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(1, 0); glVertex3f(10.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(1, 1); glVertex3f(10.0, 0.0, -10.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(0, 1); glVertex3f(-10.0, 0.0, -10.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glMaterialfv(GL_FRONT, GL_AMBIENT, grass);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grass);
	glPushMatrix();
	glTranslatef(1.5, 0.1, -5.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glmDraw(grassModel, GLM_SMOOTH);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.5, 0.1, -3.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glmDraw(grassModel, GLM_SMOOTH);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, crate);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, crate);
	glPushMatrix();
	glTranslatef(0.0, 0.5, -6.0);
	glScalef(0.5, 0.3, 0.5);
	glRotatef(90.0, 0.0, 0.0, 1.0);

	glmDraw(crateModel, GLM_SMOOTH);
	glPopMatrix();
}
void Road1_withBranch() {

	GLfloat grass[] = { 0.133, 0.545, 0.133, 1.0 };
	GLfloat branch[] = { 0.545, 0.271, 0.075, 1.0 };

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texid[2]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 0.1, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.1, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 0.1, -10.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 0.1, -10.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texid[1]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(0, 0); glVertex3f(-10.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(1, 0); glVertex3f(10.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(1, 1); glVertex3f(10.0, 0.0, -10.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(0, 1); glVertex3f(-10.0, 0.0, -10.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glMaterialfv(GL_FRONT, GL_AMBIENT, grass);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grass);
	glPushMatrix();
	glTranslatef(1.5, 0.1, -5.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glmDraw(grassModel, GLM_SMOOTH);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-1.5, 0.1, -3.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glmDraw(grassModel, GLM_SMOOTH);
	glPopMatrix();

	glMaterialfv(GL_FRONT, GL_AMBIENT, branch);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, branch);
	glPushMatrix();
	glTranslatef(0.0, 1.0, -6.0);
	glScalef(2.0, 0.5, 0.5);
	glmDraw(branchModel, GLM_SMOOTH | GLM_MATERIAL);
	glPopMatrix();
}
void Road2() {

	GLfloat grass[] = { 0.133, 0.545, 0.133, 1.0 };

	glEnable(GL_TEXTURE_2D);
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	glBindTexture(GL_TEXTURE_2D, texid[2]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 0.0); glVertex3f(-1.0, 0.1, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 0.0); glVertex3f(1.0, 0.1, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 1.0); glVertex3f(1.0, 0.1, -8.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 1.0); glVertex3f(-1.0, 0.1, -8.0);
	glEnd();
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 0.0); glVertex3f(-5.0, 0.1, -8.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 0.0); glVertex3f(5.0, 0.1, -8.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(1.0, 1.0); glVertex3f(5.0, 0.1, -10.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2f(0.0, 1.0); glVertex3f(-5.0, 0.1, -10.0);
	glEnd();
	glBindTexture(GL_TEXTURE_2D, texid[1]);
	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(0, 0); glVertex3f(-10.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(1, 0); glVertex3f(10.0, 0.0, 0.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(1, 1); glVertex3f(10.0, 0.0, -10.0);
	glNormal3f(0.0, 1.0, 0.0); glTexCoord2i(0, 1); glVertex3f(-10.0, 0.0, -10.0);
	glEnd();
	glDisable(GL_TEXTURE_2D);


	glMaterialfv(GL_FRONT, GL_AMBIENT, grass);
	glMaterialfv(GL_FRONT, GL_DIFFUSE, grass);
	glPushMatrix();
	glTranslatef(-1.5, 0.1, -4.0);
	glRotatef(90.0, 0.0, 1.0, 0.0);
	glmDraw(grassModel, GLM_SMOOTH);
	glPopMatrix();
	glPushMatrix();
	glTranslatef(-0.0, 0.1, -11.0);
	glmDraw(grassModel, GLM_SMOOTH);
	glPopMatrix();
}
void genMap(int counter) {

	unsigned seed;


	if (state == DEAD_STATE) {	// 當角色死亡時，產生最後死亡動畫
		if (death_animation < 35) {
			run += 0.01;
			death_animation++;
			score++;	//Score 控制角色的動作
		}

		glutPostRedisplay();
	}
	else {
		if (counter == 0) {

			seed = (unsigned)time(NULL);
			srand(seed);
			nextMap = rand() % 4;	// 隨機產生地圖(4種)
			genMapDetect = true;	// 預防角色重複受傷
			direct = FORWARD;	// 岔路時方向重置
		}
		if (turnAnimation == TURN_NORMAL) run += 0.05;	// 正常地圖Translation
		else run = 8.0;	// 進入轉彎 
		counter++;
		if (curMap == 1) {
			cout << run << ", " << counter << endl;
		}
		if (counter >= 120 && counter <= 150) { // 岔路時轉彎偵測
			if (direct == LEFT) {
				turnAnimation = TURN_LEFT;
				angle -= 3;
			}
			else if (direct == RIGHT) {
				turnAnimation = TURN_RIGHT;
				angle += 3;
			}
		}
		if (counter == 200) {
			turnAnimation = TURN_NORMAL;
			curMap = nextMap;
			counter = 0;
			run = 0;
			angle = 0.0;
		}
	}
	glutPostRedisplay();
	glutTimerFunc(10, genMap, counter);
}
void drawMap(int map) {
	GLfloat road_ambient[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat road_diffuse[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat road_specular[] = { 0.0, 0.0, 0.0, 1.0 };

	switch (map) {
	case 0:
		// Initialization
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, road_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, road_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, road_specular);
		Road1();
		glPopMatrix();
		break;
	case 1:
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, road_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, road_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, road_specular);
		Road2();
		glPopMatrix();
		break;
	case 2:
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, road_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, road_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, road_specular);
		Road1_withBarrier();
		glPopMatrix();
		break;
	case 3:
		glPushMatrix();
		glMaterialfv(GL_FRONT, GL_AMBIENT, road_ambient);
		glMaterialfv(GL_FRONT, GL_DIFFUSE, road_diffuse);
		glMaterialfv(GL_FRONT, GL_SPECULAR, road_specular);
		Road1_withBranch();
		glPopMatrix();
		break;
	default:
		break;
	}
}
void countScore(int counter) {
	score++;
	if (state != DEAD_STATE) {
		score++;
		glutTimerFunc(10, countScore, counter);
	}

}
void LoadTexture(GLuint* texid, char* filename) {


	FIBITMAP* bitmap = FreeImage_Load(FreeImage_GetFileType(filename, 0), filename);
	FIBITMAP *pImage = FreeImage_ConvertTo32Bits(bitmap);
	int nWidth = FreeImage_GetWidth(pImage);
	int nHeight = FreeImage_GetHeight(pImage);

	glGenTextures(1, texid);
	glBindTexture(GL_TEXTURE_2D, *texid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, nWidth, nHeight,
		0, GL_BGRA, GL_UNSIGNED_BYTE, FreeImage_GetBits(pImage));
}
void jump_func(int counter) {
	if (counter == 0) {
		animationStateMain = md2Model.StartAnimation(JUMP);
		state = JUMP_STATE;
		cout << run << endl;
	}
		counter++;
	if (counter <= 10) {
		jump = 0.14 * (float)counter - (G_ACC * pow(counter, 2.0)) / 2.0;
	}
	if (counter > 10) {
		jump = 0.7 - (G_ACC * pow(counter - 10, 2.0)) / 2.0;
	}
	if (counter == 20) {
		counter = 0;
		jump = 0.0;
		animationStateMain = md2Model.StartAnimation(RUN);
		state = NORMAL_STATE;
	}
	else {
		glutPostRedisplay();
		glutTimerFunc(10, jump_func, counter);
	}
}
void crouch_func(int counter) {
	if (counter == 0) {
		animationStateMain = md2Model.StartAnimation(CROUCH_WALK);
		state = CROUCH_STATE;
		cout << run << endl;
	}
		counter++;
	if (counter == 20) {
		animationStateMain = md2Model.StartAnimation(RUN);
		state = NORMAL_STATE;
	}
	else {
		glutPostRedisplay();
		glutTimerFunc(10, crouch_func, counter);
	}
}