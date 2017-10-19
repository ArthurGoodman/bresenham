#include <cmath>
#include <GL/freeglut.h>

const int width = 640, height = 480;

int x_center = 320, y_center = 240;
int radius = 100;

int win_id;

void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27:
        glutDestroyWindow(win_id);
        exit(0);
        break;
    }

    glutPostRedisplay();
}

int length(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

void mouse(int button, int state, int x, int y) {
    y = height - y;

    x_center = x;
    y_center = y;

    radius = length(x_center, y_center, x, y);

    glutPostRedisplay();
}

void passive_mouse(int x, int y) {
    y = height - y;

    radius = length(x_center, y_center, x, y);

    glutPostRedisplay();
}

void draw_point(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x_center + x, y_center + y);
    glVertex2i(x_center + x, y_center - y);
    glVertex2i(x_center + y, y_center + x);
    glVertex2i(x_center + y, y_center - x);
    glVertex2i(x_center - x, y_center - y);
    glVertex2i(x_center - y, y_center - x);
    glVertex2i(x_center - x, y_center + y);
    glVertex2i(x_center - y, y_center + x);
    glEnd();
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT);

    int x, y;
    int p;
    x = 0;
    y = radius;
    draw_point(x, y);

    p = 1 - radius;

    while (x < y) {
        if (p < 0) {
            x += 1;
        } else {
            x += 1;
            y -= 1;
        }

        if (p < 0) {
            p = p + 2 * x + 1;
        } else {
            p = p + 2 * (x - y) + 1;
        }

        draw_point(x, y);
    }

    glFlush();
}

void initialize() {
    glClearColor(1.0, 1.0, 1.0, 0);
    glColor3f(0.0, 0.0, 0.0);
    gluOrtho2D(0, width, 0, height);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
    glutInitWindowPosition(0, 0);
    glutInitWindowSize(width, height);

    win_id = glutCreateWindow("bresenham_circle");

    initialize();

    glutDisplayFunc(display);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passive_mouse);

    glutMainLoop();

    return 0;
}
