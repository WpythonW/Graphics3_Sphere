#include <GL/freeglut.h>
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
int N, M;

struct vec3 {
    double x;
    double y;
    double z;
};

float colors[12];

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

    double ang = M_PI / (3 * N + 3);
    double ang_xz = 2*M_PI/M;
    double alpha = ang, betha = ang_xz;
    double Sa, Ca, Sb, Cb;
    
    for (int i = 0; i < N; i++)
    {
        Sa = sin(alpha);
        Ca = cos(alpha);
        
        points.push_back({Sa, Ca, 0});
        
        for (int j = 1; j < M; j++)
        {
            Sb = sin(betha);
            Cb = cos(betha);
            
            points.push_back({Sa * Cb, Ca, Sa * Sb});
            betha+= ang_xz;
        }
        
        betha = ang_xz;
        alpha += ang;
    }

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
    cout << "Input accuracy Y: ";
    cin >> N;
    cout << "Input accuracy 2pi: ";
    cin >> M;
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
    for(int i = 1; i < M; i++)
    {
        glBegin(GL_TRIANGLES);
        glColor3f(1, 1, 1);

        glVertex3f(0, 1, 0);
        glVertex3f(points[i - 1].x, points[i - 1].y, points[i - 1].z);
        glVertex3f(points[i].x, points[i].y, points[i].z);
        glEnd();
    }
    
    glBegin(GL_TRIANGLES);
    glColor3f(1, 1, 1);

    glVertex3f(0, 1, 0);
    glVertex3f(points[M - 1].x, points[ M - 1].y, points[M - 1].z);
    glVertex3f(points[0].x, points[0].y, points[0].z);
    glEnd();
    
    for(int i = 1; i < M*N - M + 1; i++)
    {
        glBegin(GL_QUADS);
        glColor3f(1, 1, 1);

        glVertex3f(points[i - 1].x, points[i - 1].y, points[i - 1].z);
        if (i % M == 0)
            glVertex3f(points[i - M].x, points[i - M].y, points[i - M].z);
        else
            glVertex3f(points[i].x, points[i].y, points[i].z);
        
        //2M
        if (i % M == 0)
            glVertex3f(points[i].x, points[i].y, points[i].z);

        else
            glVertex3f(points[i + M].x, points[i + M].y, points[i + M].z);

        glVertex3f(points[i + M - 1].x, points[i + M - 1].y, points[i + M - 1].z);
        
        glEnd();
    }
    
    for (int i = N * M - M + 1; i < N * M; i++)
    {
        glBegin(GL_TRIANGLES);
        glColor3f(1, 1, 1);
        
        glVertex3f(0, 0, 0);
        glVertex3f(points[i-1].x, points[i-1].y, points[i-1].z);
        glVertex3f(points[i].x, points[i].y, points[i].z);
        glEnd();

    }
    
    /*glBegin(GL_QUADS);
    glColor3f(1, 1, 1);
    glVertex3f(points[M * N - M].x, points[M * N - M].y, points[M * N - M].z);
    glVertex3f(points[M * N - 2 * M + 1].x, points[M * N - 2 * M + 1].y, points[M * N - 2 * M + 1].z);
    glVertex3f(points[M * N - M + 1].x, points[M * N - M + 1].y, points[M * N - M + 1].z);
    glVertex3f(points[M * N].x, points[M * N].y, points[M * N].z);
    
    glEnd();*/
    
    
    
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
