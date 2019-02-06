#include <GL/glut.h>
 
// статическая переменная для хранения угла поворота полигона
GLfloat rotateAngle = 0.0;

/**
 * Калбэк-функция, для настройки перспективы и корректной отрисовки после 
 * изменения размера окна.
**/
void resizeWindow(GLsizei width, GLsizei height)
{
    if(!height) height = 1;
    // сбсрасываем зону видимости
    glViewport(0, 0, width, height);

    // сообщаем что следующие функции обращены к матрице проекций
    glMatrixMode(GL_PROJECTION);
    // сбрасиываем ее
    glLoadIdentity();
    // ставим туда перспективу. Тут выставляется угол обзора, соотношения ширины и длины, 
    // а также минимальная и максимальная глубина, на которой объекты отрисовываются
    gluPerspective(45.f, (GLfloat)width / (GLfloat)height, 0.1f, 100.f);

    // еще какую-то матрицу сбрасывает, как-нибудь узнаю зачем и что это такое
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
}

void renderScene(void) {
    // Отчистим два буфера
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    // Сбросим коордианты, теперь начало координат в центре окна
    glLoadIdentity();
    // передвинем центр координат на указаный вектор
    glTranslatef(0.f, 0.f, -3.f);

    // Устанавливаем угол вращения для фигуры
    glRotatef(rotateAngle, 1.0f, 1.0f, 0.0f);
    // происходит отрисовка треугольников
    // Кормим им цвет + точку. Походу отрисовки мы можем переопределить цвет.
	glBegin(GL_TRIANGLES);  
        glColor3f(1.0f, 0.0f, 0.0f);
		glVertex3f(-0.5f, -0.5f, 0.0f);

        glColor3f(0.0f, 1.0f, 0.0f);
		glVertex3f(0.0f, 0.5f, 0.0f);

        glColor3f(0.0f, 0.0f, 1.0f);
		glVertex3f(0.5f, -0.5f, 0.0f);
	glEnd();

    // пускаем это карттинку на экран. P.S. разобраться с glFlush()
	glutSwapBuffers();

    // Увеличиваем угол вращения
    rotateAngle += 1.f;
}
 
int main(int argc, char **argv) 
{
    // инициализируем глут
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(400,400);
	glutCreateWindow("PLOYGON");

    // вешаем калбэк функции
    // вызывается при изменение размера окна
    glutReshapeFunc(resizeWindow);
    // вызывается при отрисовке
	glutDisplayFunc(renderScene);
    // вызывается периодически
    glutIdleFunc(renderScene);

	glutMainLoop();
	return 0;
}