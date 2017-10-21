#include <cmath>

#include <iostream>
#include <tuple>
#include <GL/freeglut.h>

// Sizes of a window
int width = 640, height = 480;

// Parameters of the circle
int x_center = 320, y_center = 240;
int radius = 100;

// ID of a window. Used to close it on Escape keypress
int win_id;

// Type alias to represent a vector of two float values
using Vector2 = std::tuple<float, float>;

// Convert from cartesian to polar coordinates
Vector2 polarToCartesian(const Vector2 &v) {
    float rho = std::get<0>(v);
    float phi = std::get<1>(v);

    return Vector2{ rho * cos(phi), rho * sin(phi) };
}

// Convert from polar to cartesian coordinates
Vector2 cartesianToPolar(const Vector2 &v) {
    float x = std::get<0>(v);
    float y = std::get<1>(v);

    return Vector2{ sqrt(x * x + y * y), atan2f(y, x) };
}

// Print a value of a vector
void printVector(const Vector2 &v) {
    std::cout << "(" << std::get<0>(v) << ", " << std::get<1>(v) << ")";
}

// Do coordinate conversions of a given point: cart -> polar & polar -> cart
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

// Handle keyboard events
void keyboard(unsigned char key, int x, int y) {
    switch (key) {
    case 27: // ESC
        glutDestroyWindow(win_id);
        exit(0);
        break;
    }

    // Redraw window
    glutPostRedisplay();
}

// Calculate the distance between two points: (x1, y1) and (x2, y2)
int length(int x1, int y1, int x2, int y2) {
    return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// Handle mouse button press events
void mouse(int button, int state, int x, int y) {
    // Invert y coordinate
    y = height - y;

    // Update circle center
    x_center = x;
    y_center = y;

    // Compute distance from cursor position to current center of a circle
    radius = length(x_center, y_center, x, y);

    // Do coordinate conversion
    if (state)
        computeCoordinates(x - width / 2, y - height / 2);

    // Redraw window
    glutPostRedisplay();
}

// Handle mouse movement event
void passiveMouse(int x, int y) {
    // Invert y coordinate
    y = height - y;

    // Compute distance from cursor position to current center of a circle
    radius = length(x_center, y_center, x, y);

    // Redraw window
    glutPostRedisplay();
}

// Draw 8 symmetrical points on the circle
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

// Bresenham's circle algorithm itself
void drawCircle() {
    int x = 0;
    int y = radius;

    int p = 3 - 2 * radius;

    while (y >= x) {
        drawPoint(x, y);

        if (p < 0)
            p += 4 * x++ + 6;
        else
            p += 4 * (x++ - y--) + 10;
    }
}

// Handle resize event
void reshape(int w, int h) {
    // Update window size
    width = w;
    height = h;

    // Setup viewport
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glViewport(0, 0, width, height);
    gluOrtho2D(0, width, 0, height);

    glMatrixMode(GL_MODELVIEW);

    // Redraw window
    glutPostRedisplay();
}

// Handle redraw event
void display() {
    // Clear the window
    glClear(GL_COLOR_BUFFER_BIT);

    // Well... As it says, draw a circle
    drawCircle();

    // Swap frame buffers... Double buffering, you know...
    glutSwapBuffers();
}

// Initialize some OpenGL parameters
void initialize() {
    glClearColor(1.0, 1.0, 1.0, 0);
    glColor3f(0.0, 0.0, 0.0);
}

int main(int argc, char **argv) {
    // Initialize GLUT
    glutInit(&argc, argv);

    // Setup GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);

    glutInitWindowPosition(
        (glutGet(GLUT_SCREEN_WIDTH) - width) / 2, (glutGet(GLUT_SCREEN_HEIGHT) - height) / 2);

    glutInitWindowSize(width, height);

    // Create a main window
    win_id = glutCreateWindow("bresenham_circle");

    initialize();

    // Setup event handlers
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutMouseFunc(mouse);
    glutPassiveMotionFunc(passiveMouse);

    // Start main loop of the application
    glutMainLoop();

    return 0;
}
