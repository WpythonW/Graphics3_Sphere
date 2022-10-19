#define _USE_MATH_DEFINES
#include <GL/freeglut.h>
#include <GL/glu.h>
#include <stdio.h>
#include <cmath>
#include <vector>
#include <iostream>

using namespace std;

void display();
void reshape(int, int);
void timer(int);
void processNormalKeys(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);

float rx = 0;
float ry = 0;
float rz = -5;
float angle_x = 0;
float angle_y = 0;
float angle_z = 0;
float sc = 1;
int N;

struct vec3 {
    double x;
    double y;
    double z;
};

float colors[12];

/*float points[] =
{
    //ZX
    0, 0, 1,  //z
    1, 0, 0,  //x
    0, 0, 0,

    //YX
    0, 0, 0,
    0, 1, 0,  //y
    1, 0, 0,  //x

    //ZY
    0, 0, 1,  //z
    0, 1, 0,  //y
    0, 0, 0,

    //XYZ
    0, 1, 0,  //y
    1, 0, 0,  //x
    0, 0, 1,  //z

};*/


vector<vec3> points;

void colgener()
{
    double a, b, c;
    for (int i = 2; i < 12; i++) {
        a = (rand() % 255);
        a = a / 255;
        b = (rand() % 255);
        b = b / 255;
        c = (rand() % 255);
        c = c / 255;
        colors[i - 2] = a;
        colors[i - 1] = b;
        colors[i] = c;
    }
    ;
}

void filler()
{

    double ang = M_PI / (2 * N + 2);
    double alpha = ang, betha = ang;


    /*
    for (int i = 0; i < N; i++)
    {
        for (int j = 0; i < N; j++)
        {
            angle_c += ang;
            //points.push_back({});
        }
    }
    */
    for (int i = 0; i < N + 1; i++)
    {
        if (alpha == M_PI / 2) {
            points.push_back({ sin(alpha), 0, 0 });
            for (int j = 1; j < N + 2; j++)
            {
                if (betha == M_PI / 2)
                {
                    points.push_back({ 0, 0, sin(alpha) * sin(betha) });
                }
                else points.push_back({ sin(alpha) * cos(betha), 0, sin(alpha) * sin(betha) });

                betha += ang;
            }
        }
        else {
            points.push_back({ sin(alpha), cos(alpha), 0 });    ///// 

            for (int j = 1; j < N + 2; j++)
            {

                if (betha == M_PI / 2)
                {
                    points.push_back({ 0, cos(alpha), sin(alpha) * sin(betha) });
                }
                else points.push_back({ sin(alpha) * cos(betha), cos(alpha), sin(alpha) * sin(betha) });   /////

                betha += ang;
            }
        }

        alpha += ang;
        betha = ang;
    }
    ;

}

void init()
{
    glClearColor(0, 0, 0, 1.0); // очищаем экран в цвет, установленый параметрами R G B A 
    glEnable(GL_DEPTH_TEST);

}

void init_l() {
    GLfloat mat_specular[] = { 1.0, 1.0, 1.0, 1.0 };
    GLfloat mat_shininess[] = { 50.0 };
    GLfloat light_position[] = { -2.0, -2.0, -1.0, 0.0 };

    glShadeModel(GL_DIFFUSE);

    glMaterialfv(GL_FRONT, GL_SPECULAR, mat_specular);
    glMaterialfv(GL_FRONT, GL_SHININESS, mat_shininess);
    glLightfv(GL_LIGHT0, GL_POSITION, light_position);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0); // разрешаем использовать light0 

}

int main(int argc, char** argv)
{
    cout << "Input accuracy: ";
    cin >> N;
    filler();
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);

    glutInitWindowPosition(200, 200);
    glutInitWindowSize(900, 900);

    glutCreateWindow("win-1");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutKeyboardFunc(processNormalKeys);
    glutMouseFunc(mouse);

    init();
    init_l();

    glutMainLoop();
    return 0;
}



void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();
    glTranslatef(rx, ry, rz);
    glRotatef(angle_x, 1, 0, 0);
    glRotatef(angle_y, 0, 1, 0);
    glRotatef(angle_z, 0, 0, 1);
    glScalef(sc, sc, sc);
    //glPointSize(6.0);
    //draw


    glBegin(GL_LINES);

    glColor3f(1, 0, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(5, 0, 0);//X

    glColor3f(0, 1, 0);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 5, 0);//Y

    glColor3f(0, 0, 1);
    glVertex3f(0, 0, 0);
    glVertex3f(0, 0, 5);//Y

    glEnd();

    //hat
    int first_row = (points.size()) / (N + 1);
    int index = first_row;
    for (int i = 1; i < first_row; i++)
    {
        glBegin(GL_POLYGON);
        glColor3f(1, 1, 1);

        glVertex3f(0, 1, 0);
        glVertex3f(points[i - 1].x, points[i - 1].y, points[i - 1].z);
        glVertex3f(points[i].x, points[i].y, points[i].z);
        glEnd();
    }

    //the rest
    for (int i = 1; i < points.size() - index; i++)
    {
        glBegin(GL_POLYGON);
        glColor3f(1, 1, 1);

        glVertex3f(points[i - 1].x, points[i - 1].y, points[i - 1].z);
        glVertex3f(points[i].x, points[i].y, points[i].z);
        glVertex3f(points[i + first_row].x, points[i + first_row].y, points[i + first_row].z);
        glVertex3f(points[i + first_row - 1].x, points[i + first_row - 1].y, points[i + first_row - 1].z);
        glEnd();
    }


    /*glPolygonMode(GL_FRONT, GL_POLYGON);
    //glColor3f(0.3, 0, 0.9);
    glEnableClientState(GL_VERTEX_ARRAY);
    glVertexPointer(3, GL_FLOAT, 0, points);

    glEnableClientState(GL_COLOR_ARRAY);
    glColorPointer(3, GL_FLOAT, 0, colors);

    glDrawArrays(GL_TRIANGLES, 0, 12);   // 1..4 2..10 3..18 4..28      4 * (N * (N + 3) + 3)
    //glDrawArrays(GL_LINE_LOOP, 0, 4 * (N * (N + 3) + 3));   // 1..4 2..10 3..18 4..28

    glDisableClientState(GL_VERTEX_ARRAY); */
    glutSwapBuffers();
}

void reshape(int w, int h)
{
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(60, 1, 2, 50);
    glMatrixMode(GL_MODELVIEW);

}
void timer(int k)
{
    glutPostRedisplay();
    glutTimerFunc(1000 / 60, timer, 0);
}
void mouse(int button, int state, int x, int y)
{
    if (button == 3)
    {
        rz += 0.7;
    }
    else if (button == 4)
    {
        rz -= 0.7;
    }

    /*else if (button == GLUT_RIGHT_BUTTON)
    {
        leftMouseButtonDown = (state == GLUT_DOWN);
        zMovement -= 0.1f;
    }*/

    // Save the mouse position
    ///mouseXPos = x;
    ///mouseYPos = y;
    glutPostRedisplay();
}
void processNormalKeys(unsigned char key, int x, int y) {
    switch (key)
    {
    case 's': angle_x -= 2; break;
    case 'w': angle_x += 2; break;
    case 'e': angle_z += 2; break;
    case 'q': angle_z -= 2; break;
    case 'd': angle_y -= 2; break;
    case 'a': angle_y += 2; break;
    default: exit(0);
    }
    glutPostRedisplay();
}