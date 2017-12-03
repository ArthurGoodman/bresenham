#include <cmath>

#include <algorithm>
#include <iostream>
#include <tuple>
#include <GL/freeglut.h>

// Sizes of a window
int width = 640, height = 480;

// Parameters of the circle
int x_center = 320, y_center = 240;
int mouse_x = x_center, mouse_y = y_center;

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
    return (int)sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

// Handle mouse button press events
void mouse(int button, int state, int x, int y) {
    // Invert y coordinate
    y = height - y;

    // Update circle center
    x_center = x;
    y_center = y;

    // Do coordinate conversion
    if (state)
        computeCoordinates((float)(x - width / 2), (float)(y - height / 2));

    // Redraw window
    glutPostRedisplay();
}

// Handle mouse movement event
void passiveMouse(int x, int y) {
    // Invert y coordinate
    y = height - y;

    mouse_x = x;
    mouse_y = y;

    // Redraw window
    glutPostRedisplay();
}

// Draw 8 symmetrical points on the circle
void drawPoint(int x, int y) {
    glBegin(GL_POINTS);
    glVertex2i(x, y);
    glEnd();
}

// Bresenham's line algorithm itself
void drawLine() {
    int Dx = mouse_x - x_center;
    int Dy = mouse_y - y_center;

    //# Increments
    int Sx = Dx >= 0 ? 1 : -1;
    int Sy = Dy >= 0 ? 1 : -1;

    //# Segment length
    Dx = abs(Dx);
    Dy = abs(Dy);

    int D = std::max(Dx, Dy);

    //# Initial remainder
    double R = D / 2;

    int X = x_center;
    int Y = y_center;
    if (Dx > Dy) {
        //# Main loop
        for (int I = 0; I < D; I++) {
            drawPoint(X, Y);
            //# Update (X, Y) and R
            X += Sx;
            R += Dy; //# Lateral move
            if (R >= Dx) {
                Y += Sy;
                R -= Dx; //# Diagonal move
            }
        }
    } else {
        //# Main loop
        for (int I = 0; I < D; I++) {
            drawPoint(X, Y);
            //# Update (X, Y) and R
            Y += Sy;
            R += Dx; //# Lateral move
            if (R >= Dy) {
                X += Sx;
                R -= Dy; //# Diagonal move
            }
        }
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
    drawLine();

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
    win_id = glutCreateWindow("bresenham_line");

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
