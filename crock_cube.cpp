#include <GL/glut.h>
 
GLfloat rotateAngle = 0.0;

// функция для корректной инициализации
int initGl(void)
{
    // делает все плавнее (в каком месте -- я не заметил)
    glShadeModel(GL_SMOOTH); 
    // выставляет цвет пустого экрана
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f); 
    // очищает бувер глубины (зачем? нафига? как-нибудь узнаю)
    glClearDepth( 1.0f ); 
    // ставит проверку по глубине. Без них 3-мерные фигуры корректно отрисовываться не будут
    glEnable(GL_DEPTH_TEST);
    // ставит метод проверки на глубину
    glDepthFunc(GL_LEQUAL);
}

void resizeWindow(GLsizei width, GLsizei height)
{
    if(!height) height = 1;
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.f, (GLfloat)width / (GLfloat)height, 0.1f, 100.f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void renderScene(void) 
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(0.f, 0.f, -3.f);

    glRotatef(rotateAngle, 2.0f, 1.0f, 2.0f);
	glBegin(GL_QUADS);

    // bot
    glColor3f(0.5f, 0.5f, 0.f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);

    // right
    glColor3f(0.5f, 0.f, 0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);

    // back
    glColor3f(0.f, 0.5f, 0.5f);
    glVertex3f(-0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);

    // left
    glColor3f(0.5f, 0.f, 0.f);
    glVertex3f(0.5f, -0.5f, 0.5f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);

    // front
    glColor3f(0.f, 0.5f, 0.f);
    glVertex3f(0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, -0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(0.5f, 0.5f, -0.5f);

    // top
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, -0.5f);
    glVertex3f(-0.5f, 0.5f, 0.5f);
    glVertex3f(0.5f, 0.5f, 0.5f);

	glEnd();

	glutSwapBuffers();

    rotateAngle += 1.f;
}
 
int main(int argc, char **argv) 
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(400,400);
	glutCreateWindow("CUBE");
    initGl();

    glutReshapeFunc(resizeWindow);
	glutDisplayFunc(renderScene);
    glutIdleFunc(renderScene);

	glutMainLoop();
	return 0;
}