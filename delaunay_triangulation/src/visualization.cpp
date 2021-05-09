//
// Created by daniilsmelskiy on 04.05.21.
//

#include <GL/freeglut.h>

#include "visualization.h"

int Plane::plane_width = 100;
int Plane::plane_height = 100;

void Plane::InitPlane(int argc, char **argv) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE);
    glutInitWindowSize(1000, 1000);
    glutInitWindowPosition(0, 0);

    glutCreateWindow("Nearest neighbours");
    glClearColor(1, 1, 1, 0);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(-kDefaultPadding, plane_width + kDefaultPadding,
            -kDefaultPadding, plane_height + kDefaultPadding,
            -1, 1);
}

void Plane::PutPlane() {
    glutSwapBuffers();
    glutMainLoop();
}

void Plane::AddLine(const Line2D& line) {
    glBegin(GL_LINES);
    glVertex2d(line.fir.x, line.fir.y);
    glVertex2d(line.sec.x, line.sec.y);
    glEnd();
}

void Plane::AddPoint(const Point2D &p) {
    glBegin(GL_POINTS);
    glVertex2f(p.x, p.y);
    glEnd();
}

void Plane::SetColor(int r, int g, int b) {
    glColor3f(static_cast<float>(r) / 255.f,
              static_cast<float>(g) / 255.f,
              static_cast<float>(b) / 255.f);
}

void Plane::SetLineWidth(int width) {
    glLineWidth(width);
}

void Plane::SetPointSize(int size) {
    glPointSize(size);
}

void Plane::SetPlaneSize(int width, int height) {
    plane_width = width;
    plane_height = height;
}