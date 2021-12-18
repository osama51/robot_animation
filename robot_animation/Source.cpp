#include <stdlib.h>
#include "imageloader.h"
#include "glm.h"
#include <GL/glut.h>
#include <cmath>
#include <iostream>


#define Right 1
#define Left -1
#define PI 3.14

static int shoulder_x[2], shoulder_y[2], shoulder_z[2], hip_x[2], hip_y[2], hip_z[2], knee[2], elbow[2], jump, wside, wfront, hrotate, trock;
int moving; int startx; int starty;

static int f = 9;
static int d = 0;

float frames[3][9][12] = { { { 0,-90,5,-5 },{ 4,-85,10,-15 } ,{ 7,-15,60,-30 },	{ 9,-15,60,-30 },{ 10,-15,60,-30 },{ 10,-15,60,-20 },
						{ 8,45,10,-15 } ,{ 5,0,0,-5 },{ 0,0,0,0 } }, { { 0,0,2,0,0,90,0 },{ 1,30,7,0,5,90,30 } ,{ 0,60,10,6,10,90,60 },
						{ 1,30,10,3,5,90,30 },{ 0,0,10,0,0,90,0 },{ 1,-30,10,-3,-5,90,30 },	{ 0,-60,7,0,-10,90,60 } ,{ 1,-30,2,0,-5,90,30 },{ 0,0,2,0,-1,90,0 } },
						{ { 0,-60,5,-10,0,-90,0,0,0,0 },{ 0,-60,10,-10,0,-90,0,0,-5,45,20 } ,{ 0,-60,60,-10,0,-45,0,0,-10,90,50 },
						{ 0,-60,60,-10,1,0,0,0,-10,45,20 },{ 0,-60,60,-10,3,0,0,0,-10,0,0 },{ 0,-60,60,-10,6,0,0,0,-10,45,20 },
						{ 0,-60,10,-10,8,0,0,2.5,-10,90,50 } ,{ 0,-60,0,-10,9,0,0,6,-10,45,20 },{ 0,-60,0,-10,10,0,0,7,-10,0,0 } } };

GLfloat angle = 0.0;   // in degrees 
GLfloat angle2 = 0.0;  // in degrees 

void arm(GLfloat LR, GLint num);
void leg(GLfloat LR, GLint num);
GLuint _textureId; //The id of the texture
GLuint rabbittexture;
GLuint yetitexture;
GLuint rocktexture;

const char *modelname = "data/rabbit.obj";
const char *rabbit_obj = "data/rabbit.obj";
const char *rocks_obj = "data/lowpolyrock.obj";
const char *rock_obj = "data/SM_Big_Rock_01.obj";
const char *palm_obj = "data/palm.obj";
const char *bench_obj = "data/bench.obj";
const char *trashcan_obj = "data/park-trashcan.obj";
const char *parklight_obj = "data/park-light.obj";


// RGBA
GLfloat light_ambient[] = { 0.0, 0.0, 0.0, 0.0 };
GLfloat light_diffuse[] = { 1, 1, 1, 1.0 };
GLfloat light_specular[] = { 0.6, 0.6, 0.6, 1.0 };
// x , y, z, w
GLfloat light_position[] = { 5, 3.0, 10.0, 1.0 };
GLfloat lightPos1[] = { -0.5, 0.0, -2.0, 1.0 };

// rabbit material parameters
GLfloat mat_amb_diff[] = { 0.753, 0.703, 0.554, 1.0 };
GLfloat mat_specular[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess[] = { 10.0 };

// rock material parameters
GLfloat mat_amb_diff_r[] = { 0.743, 0.4, 0.2, 1.0 };
GLfloat mat_specular_r[] = { 0.0, 0.0, 0.0, 1.0 };
GLfloat shininess_r[] = { 80.0 };


//Setting Camera
double eye[] = { 0.0, 2.5, 20.0 };
double center[] = { 0.0, 0.0, 0.0 };
double up[] = { 0.0, 1.0, 0.0 };


//Makes the image into a texture, and returns the id of the texture
GLuint loadTexture(Image *image)
{
	GLuint textureId;
	glGenTextures(1, &textureId);			  //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId);  //Tell OpenGL which texture to edit
											  //Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,				  //Always GL_TEXTURE_2D
		0,									  //0 for now
		GL_RGB,					              //Format OpenGL uses for image
		image->width, image->height,          //Width and height
		0,							          //The border of the image
		GL_RGB,					              //GL_RGB, because pixels are stored in RGB format
		GL_UNSIGNED_BYTE,			          //GL_UNSIGNED_BYTE, because pixels are stored
									          //as unsigned numbers
		image->pixels);			              //The actual pixel data
	return textureId;						  //Returns the id of the texture
}



void init(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glShadeModel(GL_FLAT);
}

void reset() {
	double e[] = { 0.0,0.0,1.0 };
	double c[] = { 0.0,0.0,0.0 };
	double u[] = { 0.0,1.0,0.0 };
	for (int i = 0; i< 3; i++)
	{
		eye[i] = e[i];
		center[i] = c[i];
		up[i] = u[i];
	}
}

void screan_menu(int value)
{
	if (value == 0) {
		d = 0;
	}
	else if (value == 1) {
		d = 1;
	}
	else if (value == 2) {
		d = 2;
	}
	else if (value == 4) {
		Image* image = loadBMP("images/grass2.bmp");
		_textureId = loadTexture(image);
		delete image;
	}
	else if (value == 5)
	{
		Image* image = loadBMP("images/rockground.bmp");
		_textureId = loadTexture(image);
		delete image;
	}
	else if (value == 6)
	{
		Image* image = loadBMP("images/snow.bmp");
		_textureId = loadTexture(image);
		delete image;
	}
	glutPostRedisplay();
}

void initCarpet(void) {
	Image* image = loadBMP("images/grass2.bmp");
	_textureId = loadTexture(image);
	delete image;
}

void attachMenu() {
	glutCreateMenu(screan_menu);
	glutAddMenuEntry("Jump", 0);
	glutAddMenuEntry("Dance", 1);
	glutAddMenuEntry("Move", 2);
	glutAddMenuEntry("", 3);
	glutAddMenuEntry("Grass", 4);
	glutAddMenuEntry("Rockground", 5);
	glutAddMenuEntry("Snow", 6);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void initRendering()
{
	initCarpet();
	// Turn on the power
	glEnable(GL_LIGHTING);
	// Flip light switch
	glEnable(GL_LIGHT0);
	//glEnable(GL_LIGHT1);
	// assign light parameters
	glLightfv(GL_LIGHT0, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT0, GL_SPECULAR, light_specular);
	glLightfv(GL_LIGHT1, GL_AMBIENT, light_ambient);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, light_diffuse);
	glLightfv(GL_LIGHT1, GL_SPECULAR, light_specular);
	// Material Properties
	GLfloat lightColor1[] = { 1.0f, 1.0f, 1.0f, 1.0f };
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor1);
	// glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glEnable(GL_NORMALIZE);
	//Enable smooth shading
	glShadeModel(GL_SMOOTH);
	// Enable Depth buffer
	glEnable(GL_DEPTH_TEST);
}

void drawmodel(char *filename)
{
	GLMmodel *model = glmReadOBJ(filename);
	glmUnitize(model);
	glmFacetNormals(model);
	glmVertexNormals(model, 90.0);
	glmScale(model, 1.0);
	glmDraw(model, GLM_SMOOTH | GLM_MATERIAL);
}

void trunk()
{
	glPushMatrix();    //stack: [R][I]
	glScalef(2.0, 3.2, 1.0);
	glColor3f(1.0, 0.0, 0.0);
	glutSolidCube(1.0);
	glPopMatrix();    //stack: [I]
}

void head()
{
	glPushMatrix();
	glTranslatef(0, 2.5, 0);
	glPushMatrix();
	glScalef(0.8, 1, 0.8);
	glutSolidSphere(0.8, 20, 20);
	glPopMatrix();
	glPopMatrix();
}

void arm(GLfloat LR, GLint num)
{
	//__UPPER ARM___//
	glPushMatrix();     //stack: [TRUNK][I]
	glTranslatef(LR * 1.0, 1.6, 0.0);
	glRotatef((GLfloat)shoulder_y[num], 0.0, LR * 1.0, 0.0);
	glRotatef((GLfloat)shoulder_z[num], 0.0, 0.0, 1.0);
	glTranslatef(LR * 0.8, 0, 0.0);
	glRotatef((GLfloat)shoulder_x[num], 1.0, 0.0, 0.0);
	glPushMatrix();    //stack: [UPARM][TRUNK][I]
	glScalef(1.3, 0.4, 0.4);
	glutSolidCube(1);
	glPopMatrix();    //stack: [TRUNK][I]

					  //___LOWER ARM___//
	glTranslatef(LR * 0.6, 0.0, 0);
	glRotatef((GLfloat)elbow[num], 0.0, 0.0, 1.0);
	glTranslatef(LR * 0.8, 0.0, 0.0);
	glPushMatrix();    //stack: [LOARM][TRUNK][I]
	glScalef(1.4, 0.4, 0.4);
	glutSolidCube(1);
	glPopMatrix();    //stack: [TRUNK][I]
	glPopMatrix();    //stack: [I]

}

void leg(GLfloat LR, GLint num)
{
	//__UPPER LEG___//
	glPushMatrix();    //stack: [TRUNK][I]
	glTranslatef(LR * 0.4, -1.2, 0);
	glRotatef((GLfloat)hip_x[num], 1.0, 0.0, 0.0);
	glRotatef((GLfloat)hip_y[num], 0.0, LR * 1.0, 0.0);
	glRotatef((GLfloat)hip_z[num], 0.0, 0.0, 1.0);
	glTranslatef(LR * 0.2, -1.3, 0.0);
	glPushMatrix();    //stack: [UPLEG][TRUNK][I]
	glScalef(0.7, 1.7, 0.8);
	glutSolidCube(1);
	glPopMatrix();    //stack: [TRUNK][I]

					  //__LOWER LEG___//
	glTranslatef(LR * -0.2, -0.8, 0.0);
	glRotatef((GLfloat)knee[num], 1.0, 0.0, 0.0);
	glTranslatef(LR * 0.2, -0.9, 0.0);
	glPushMatrix();    //stack: [LOLEG][TRUNK][I]
	glScalef(0.7, 1.7, 0.7);
	glutSolidCube(1);
	glPopMatrix();    //stack: [TRUNK][I]

					  //__FOOT___//
	glTranslatef(0, -1, 0.3);
	glPushMatrix();    //stack: [FOOT][TRUNK][I]
	glScalef(0.7, 0.2, 1.3);
	glutSolidCube(1);
	glPopMatrix();    //stack: [TRUNK][I]
	glPopMatrix();    //stack: [I]
}

void body() {
	trunk();
	head();

	arm(Right, 0);
	arm(Left, 1);

	leg(Right, 0);
	leg(Left, 1);
}

void rabbit() {

	glPushMatrix();    //stack: [TRUNK][I]
	glTranslatef(21.0, -4.5, -5);
	glRotatef(-60.0, 0.0, 1.0, 0.0);
	glPushMatrix();    
    //glScalef(5,5, 5);
	drawmodel((char *)rabbit_obj);

	glPopMatrix();    //stack: [TRUNK][I]
	glPopMatrix();

}

void rock(int x, int y, int z, int s) {
	
	glPushMatrix();    //stack: [TRUNK][I]
	glTranslatef(x, y, z);
	glRotatef(140.0, 0.0, 1.0, 0.0);
	glScalef(s, s, s);
	drawmodel((char *)rock_obj);

	glPopMatrix();
}

void rocks(int x, int y, int z, int s) {

	glPushMatrix();    //stack: [TRUNK][I]
	glTranslatef(x, y, z);
	glRotatef(140.0, 0.0, 1.0, 0.0);
	glScalef(s, s, s);
	drawmodel((char *)rocks_obj);

	glPopMatrix();
}

void tree(GLfloat x, GLfloat z) {
	glPushMatrix();    //stack: [TRUNK][I]
	glTranslatef(x, 25, z);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glPushMatrix();    
	glScalef(30, 30, 30);
	drawmodel((char *)palm_obj);

	glPopMatrix();    //stack: [TRUNK][I]
	glPopMatrix();
}

void bench(GLfloat x, GLfloat z, int r)
{
	glPushMatrix();    //stack: [TRUNK][I]
	glTranslatef(x, -3.5, z);
	glRotatef(r, 0.0, 1.0, 0.0);
	glScalef(6, 6, 5);
	drawmodel((char *)bench_obj);

	glPopMatrix();
}

void trashcan() {
	glPushMatrix();    //stack: [TRUNK][I]
	glTranslatef(-23.0, -4.5, -14);
	glRotatef(-60.0, 0.0, 1.0, 0.0);
	glPushMatrix();
	//glScalef(5,5, 5);
	drawmodel((char *)trashcan_obj);

	glPopMatrix();    //stack: [TRUNK][I]
	glPopMatrix();
}

void parklight() {
	glPushMatrix();    //stack: [TRUNK][I]
	glTranslatef(-26.0, -4.5, -14);
	glRotatef(0.0, 0.0, 1.0, 0.0);
	glScalef(2, 1, 2);
	drawmodel((char *)parklight_obj);

	glPopMatrix();
}

void textures() {
	Image* rabtexture = loadBMP("images/fur-texture.bmp");
	rabbittexture = loadTexture(rabtexture);

	Image* yetexture = loadBMP("images/white-fur-texture.bmp");
	yetitexture = loadTexture(yetexture);

	Image* roktexture = loadBMP("images/rock03.bmp");
	rocktexture = loadTexture(yetexture);
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT );
	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	gluLookAt(eye[0], eye[1], eye[2], center[0], center[1], center[2], up[0], up[1], up[2]);
	
	glPushMatrix();    //stack: [I]
	glRotatef(angle2, 1.0, 0.0, 0.0);
	glRotatef(angle, 0.0, 1.0, 0.0);

	//glPushMatrix();
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glBegin(GL_QUADS);
	glNormal3f(0.0, 1.0, 0.0);
	glTexCoord2f(0.0f, 0.0f); glVertex3f(-40, -5.5, 40);
	glTexCoord2f(3.0f, 0.0f); glVertex3f(40, -5.5, 40);
	glTexCoord2f(3.0f, 3.0f); glVertex3f(40, -5.5, -40);
	glTexCoord2f(0.0f, 3.0f); glVertex3f(-40, -5.5, -40);
	glEnd();
	
	glBindTexture(GL_TEXTURE_2D, rabbittexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	rabbit();

	glBindTexture(GL_TEXTURE_2D, yetitexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPushMatrix();
	glTranslatef(wside, jump, wfront);
	glRotatef(hrotate, 0, 1, 0);
	body();
	glPopMatrix();

	glBindTexture(GL_TEXTURE_2D, rocktexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glPushMatrix();
	glTranslatef(0, 0, trock);
	rock(-10, -4.5, 8, 1);
	glPopMatrix();

	rocks(15, -1, -15, 10);
	glDisable(GL_TEXTURE_2D);

	bench(-10, -14, 180);
	bench(-25, -1, -90);
	bench(-25, 15, -90);
	
	//tree(-17, 5);
	//tree(-13, 4);
	tree(-7, -25);
	trashcan();
	//parklight();

	glPopMatrix();
	glutSwapBuffers();
}

void setFrame(int animation, int frameIndex) {
	jump = frames[animation][frameIndex][0];
	shoulder_z[0] = frames[animation][frameIndex][1];
	shoulder_z[1] = frames[animation][frameIndex][1];
	knee[0] = frames[animation][frameIndex][2];
	knee[1] = frames[animation][frameIndex][2];
	hip_y[0] = frames[animation][frameIndex][3];
	hip_y[1] = frames[animation][frameIndex][3];
	wfront = frames[animation][frameIndex][4];
	hrotate = frames[animation][frameIndex][5];
	hip_z[0] = frames[animation][frameIndex][6];
	hip_z[1] = -1 * frames[animation][frameIndex][6];
	trock = frames[animation][frameIndex][7];
	wside = frames[animation][frameIndex][8];
	hip_x[0] = frames[animation][frameIndex][9];
	hip_x[1] = -frames[animation][frameIndex][9];
	
}

void timer(int value) {
	f = f % 9;
	setFrame(d, f);
	f++;
	glutPostRedisplay();
	glutTimerFunc(190, timer, d);
}

void rotatePoint(double a[], double theta, double p[])
{

	double temp[3];
	temp[0] = p[0];
	temp[1] = p[1];
	temp[2] = p[2];

	temp[0] = -a[2] * p[1] + a[1] * p[2];
	temp[1] = a[2] * p[0] - a[0] * p[2];
	temp[2] = -a[1] * p[0] + a[0] * p[1];

	temp[0] *= sin(theta);
	temp[1] *= sin(theta);
	temp[2] *= sin(theta);

	temp[0] += (1 - cos(theta)) * (a[0] * a[0] * p[0] + a[0] * a[1] * p[1] + a[0] * a[2] * p[2]);
	temp[1] += (1 - cos(theta)) * (a[0] * a[1] * p[0] + a[1] * a[1] * p[1] + a[1] * a[2] * p[2]);
	temp[2] += (1 - cos(theta)) * (a[0] * a[2] * p[0] + a[1] * a[2] * p[1] + a[2] * a[2] * p[2]);

	temp[0] += cos(theta) * p[0];
	temp[1] += cos(theta) * p[1];
	temp[2] += cos(theta) * p[2];

	p[0] = temp[0];
	p[1] = temp[1];
	p[2] = temp[2];
}

void crossProduct(double a[], double b[], double c[])
{
	c[0] = a[1] * b[2] - a[2] * b[1];
	c[1] = a[2] * b[0] - a[0] * b[2];
	c[2] = a[0] * b[1] - a[1] * b[0];
}

void normalize(double a[])
{
	double norm;
	norm = a[0] * a[0] + a[1] * a[1] + a[2] * a[2];
	norm = sqrt(norm);
	a[0] /= norm;
	a[1] /= norm;
	a[2] /= norm;
}

void rotateHorizontal(int flag) {
	rotatePoint(up, flag * 3.14 / 8, eye);
}

void rotateVertical(int flag) {
	double lookVector[3];
	double rotateVector[3];
	for (int i = 0; i < 3; i++) {
		lookVector[i] = center[i] - eye[i];
	}
	normalize(lookVector);
	crossProduct(up, lookVector, rotateVector);
	rotatePoint(rotateVector, flag * 3.14 / 8, eye);
	rotatePoint(rotateVector, flag * 3.14 / 8, up);
}

void zoom(int flag) {
	double step = flag * 0.2;
	double lookVector[3];
	for (int i = 0; i < 3; i++)
		lookVector[i] = center[i] - eye[i];
	normalize(lookVector);
	for (int i = 0; i < 3; i++) {
		eye[i] += lookVector[i] * step;
	}
}

void specialKeys(int key, int x, int y)
{
	switch (key)
	{
	case GLUT_KEY_LEFT:
		rotateHorizontal(-1);
		break;
	case GLUT_KEY_RIGHT:
		rotateHorizontal(1);
		break;
	case GLUT_KEY_UP:
		rotateVertical(1);
		break;
	case GLUT_KEY_DOWN:
		rotateVertical(-1);
		break;
	}
	glutPostRedisplay();
}

void reshape(int w, int h)
{
	glViewport(0, 0, (GLsizei)w, (GLsizei)h);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(85.0, (GLfloat)w / (GLfloat)h, 1.0, 70.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glTranslatef(0.0, 0.0, -5.0);
}

void keyboard(unsigned char key, int x, int y)
{
	switch (key)
	{
	case '1':
		zoom(1);
		glutPostRedisplay();
		break;

	case '2':
		zoom(-1);
		glutPostRedisplay();
		break;
	}
}

static void mouse(int button, int state, int x, int y)
{
	if (button == GLUT_LEFT_BUTTON) {
		if (state == GLUT_DOWN) {
			moving = 1;
			startx = x;
			starty = y;
		}
		if (state == GLUT_UP) {
			moving = 0;
		}
	}
}

static void motion(int x, int y)
{
	if (moving) {
		angle = angle + (x - startx);
		angle2 = angle2 + (y - starty);
		startx = x;
		starty = y;
		glutPostRedisplay();
	}
}


int main(int argc, char** argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(1500, 1500);
	glutInitWindowPosition(10, 10);
	glutCreateWindow(argv[0]);
	textures();
	attachMenu();
	init();
	initRendering();
	glutMouseFunc(mouse);
	glutTimerFunc(190, timer, d);
	glutMotionFunc(motion);
	glutDisplayFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutSpecialFunc(specialKeys);
	glutMainLoop();
	return 0;
}