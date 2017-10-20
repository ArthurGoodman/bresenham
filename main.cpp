#include <cmath>
#include <iostream>
#include <tuple>
#include <GL/freeglut.h>

int width = 640, height = 480;

int x_center = 320, y_center = 240;
int radius = 100;

int win_id;

using Vector2 = std::tuple<float, float>;

Vector2 polarToCartesian(const Vector2 &v) {
    float rho = std::get<0>(v);
    float phi = std::get<1>(v);

    return Vector2{ rho * cos(phi), rho * sin(phi) };
}

Vector2 cartesianToPolar(const Vector2 &v) {
    float x = std::get<0>(v);
    float y = std::get<1>(v);

    return Vector2{ sqrt(x * x + y * y), atan2f(y, x) };
}

void printVector(const Vector2 &v) {
    std::cout << "(" << std::get<0>(v) << ", " << std::get<1>(v) << ")";
}

void computeCoordinates(float x, float y) {
    std::cout << "Polar: ";
    Vector2 polar = cartesianToPolar(Vector2{ x, y });
    printVector(polar);
    std::cout << "\n";

    std::cout << "Cartesian: ";
    Vector2 cartesian = polarToCartesian(polar);
    printVector(cartesian);
    std::cout << "\n\n";
}

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

    if (state)
        computeCoordinates(x - width / 2, y - height / 2);

    glutPostRedisplay();
}

void passive_mouse(int x, int y) {
    y = height - y;

    radius = length(x_center, y_center, x, y);

    glutPostRedisplay();
}

void drawPoint(int x, int y) {
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

void drawCircle() {
    int x, y;
    int p;
    x = 0;
    y = radius;
    drawPoint(x, y);

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

        drawPoint(x, y);
    }
}

void reshape(int w, int h) {
    width = w;
    height = h;

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);

    glutPostRedisplay();
}

void display() {
    glClear(GL_COLOR_BUFFER_BIT);
    drawCircle();
    glutSwapBuffers();
}

void initialize() {
    glClearColor(1.0, 1.0, 1.0, 0);
    glColor3f(0.0, 0.0, 0.0);
}

int main(int argc, char **argv) {
    glutInit(&argc, argv);

    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowPosition(
        (glutGet(GLUT_SCREEN_WIDTH) - width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2);

    glutInitWindowSize(width, height);

    win_id = glutCreateWindow("bresenham_circle");

    initialize();

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passive_mouse);

    glutMainLoop();

    return 0;
}
