#include <GL/glut.h>
#include <stdio.h>
#include <time.h>
#include "imageloader.h"

GLuint _textureStage;
GLuint _textureBg;

GLint level = 1;
GLint score = 0;
GLint length = 0;
GLint life = 3;
GLbyte finish = true;
char hs[4];

GLint body[2][150] = {{}};
GLint x = 5;
GLint z = 10;
GLint x1[2] = {};
GLint z1[2] = {};
GLbyte dir = 0;

GLint foodx = 0;
GLint foodz = 0;

GLint up = 150;
GLint down = 0;
GLint right = 150;
GLint left = 0;

static GLfloat head = 90;

GLuint loadTexture(Image* image)
{
    GLuint textureId;
    glGenTextures(1, &textureId); //Make room for our texture
    glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
    //Map the image to the texture
    glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
                 0,                            //0 for now
                 GL_RGB,                       //Format OpenGL uses for image
                 image->width, image->height,  //Width and height
                 0,                            //The border of the image
                 GL_RGB, //GL_RGB, because pixels are stored in RGB format
                 GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
                 //as unsigned numbers
                 image->pixels);               //The actual pixel data
    return textureId; //Returns the id of the texture
}

void lightSettings()
{
    GLfloat ambientLight[] = {0.3f, 0.4f, 0.8f, 1.0f};
    glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientLight);
    GLfloat light0[] = {0.5f, 0.5f, 0.5f, 1.0f};
    GLfloat pos0[] = {4.0f, 0.0f, 8.0f, 1.0f};
    glLightfv(GL_LIGHT0, GL_DIFFUSE, light0);
    glLightfv(GL_LIGHT0, GL_POSITION, pos0);
    GLfloat light1[] = {0.5f, 0.2f, 0.2f, 1.0f};
    GLfloat pos1[] = {-1.0f, 0.5f, 0.5f, 0.0f};
    glLightfv(GL_LIGHT1, GL_DIFFUSE, light1);
    glLightfv(GL_LIGHT1, GL_POSITION, pos1);
}

void initialize()
{
    glEnable(GL_DEPTH_TEST);
    glClearColor(0.0, 0.0, 0.0, 1.0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);
    glEnable(GL_LIGHT1);
    glEnable(GL_NORMALIZE);

    Image* image = loadBMP("C:\\Users\\Rakibul Islam\\Desktop\\Graphics\\HungrySnake\\roof.bmp");
    _textureStage = loadTexture(image);
    image = loadBMP("C:\\Users\\Rakibul Islam\\Desktop\\Graphics\\HungrySnake\\grass.bmp");
    _textureBg = loadTexture(image);
    delete image;
}

void resize (int width, int height)
{
    glViewport(0, 0, width, height);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0, (double)width / (double)height, 1, 800.0);
}

void text(char *string)
{
    while(*string)
        glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, *string++);
}

void angle()
{
    glRotatef(30,1.0,0.0,0.0);
}

void gameReset()
{
    x = 5;
    z = 10;
    dir = 0;
    level = 1;
    score = 0;
    length = 0;
    life = 3;
    finish = false;
}

void highscore()
{
    FILE *fp1;
    fp1 = fopen("C:\\Users\\Rakibul Islam\\Desktop\\Graphics\\HungrySnake\\highscore.txt","r");
    if(fp1==NULL)
    {
        printf("Error occured while opening file\n");
        exit(1);
    }
    fread(hs,sizeof(char),3,fp1);

    fclose(fp1);
    printf("%s",hs);
}

void highscoreWrite()
{
    FILE *fp1;
    char a[3];
    sprintf(a, "%d", score);

    fp1 = fopen("C:\\Users\\Rakibul Islam\\Desktop\\Graphics\\HungrySnake\\highscore.txt","w+");
    if(fp1==NULL)
    {
        printf("Error occured while opening file\n");
        exit(1);
    }
    fwrite(&a,sizeof(char),sizeof(a),fp1);
    fclose(fp1);
}

void startMenu()
{
    highscore();

    glColor3f(1, 0, 0);
    glRasterPos3f(-40, 40, 0);
    text("<<<<<<<<<<        HUNGRY SNAKE        >>>>>>>>>>");

    glColor3f(0, 1, 0);
    glRasterPos3f(35, -110, 0);
    text("Highscore : ");
    glRasterPos3f(80, -110, 0);
    text(hs);

    glColor3f(1, 1, 1);
    glRasterPos3f(25, -130, 0);
    text("... Press S to start ...");

    glColor3f(0, 0.5, 1);
    glRasterPos3f(-75, 0, 0);
    text("Instructions:");
    glColor3f(0, 0, 1);
    glRasterPos3f(-125, -15, 0);
    text("Press UP to move upward");
    glRasterPos3f(-150, -30, 0);
    text("Press DOWN to move downward");
    glRasterPos3f(-148, -45, 0);
    text("Press RIGHT to move rightward");
    glRasterPos3f(-136, -60, 0);
    text("Press LEFT to move leftward");

    glColor3f(0, 0.5, 1);
    glRasterPos3f(150, 0, 0);
    text("Created by:");
    glColor3f(0, 0, 1);
    glRasterPos3f(150, -15, 0);
    text("Md. Rakibul Islam");
    glRasterPos3f(150, -30, 0);
    text("ID: 12.01.04.001");
    glRasterPos3f(150, -45, 0);
    text("Md. Shofiur Rahman");
    glRasterPos3f(150, -60, 0);
    text("ID: 12.01.04.012");

    glTranslated(60, -40, 0);
    glPushMatrix();
    glColor3f(0.0, 0.0, 1.0);
    glutSolidTorus(6,50,10,50);
    glPopMatrix();

    glPushMatrix();
    glColor3f(0.0, 1.0, 1.0);
    glutSolidTorus(5,40,10,50);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 0.0);
    glutSolidTorus(5,30,10,50);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glutSolidTorus(11,18,10,50);
    glPopMatrix();

    glPushMatrix();
    glColor3f(1,0,0);
    glTranslatef(17,0,0);
    glBegin(GL_TRIANGLES);
    glVertex3f(0,0,10);
    glVertex3f(-25,-15,10);
    glVertex3f(-25,15,10);
    glEnd();
    glPopMatrix();
}

void plane()
{
    glPushMatrix();
    glColor3f(1.0, 1.0, 1.0);
    glTranslatef(75.0, -16.0, 75.0);
    glScalef(170,2.0,170);
    glutSolidCube(1);
    glPopMatrix();

    glPushMatrix(); // Plane texture
    glBindTexture(GL_TEXTURE_2D, _textureStage);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glTexCoord3f(0.0,0.0,1);
    glVertex3f(-10,-14,160);
    glTexCoord3f(0.0,3.0,-1);
    glVertex3f(-10,-14,-10);
    glTexCoord3f(3.0,3.0,-1);
    glVertex3f(160,-14,-10);
    glTexCoord3f(3.0,0.0,1);
    glVertex3f(160,-14,160);
    glEnd();
    glBindTexture( GL_TEXTURE_2D, 0 );
    glPopMatrix();

    glPushMatrix(); // Background texture
    glBindTexture(GL_TEXTURE_2D, _textureBg);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBegin(GL_QUADS);
    glTexCoord3f(0.0,0.0,1);
    glVertex3f(-250,-14.1,200);
    glTexCoord3f(0.0,10.0,-1);
    glVertex3f(-250,-14.1,-250);
    glTexCoord3f(10.0,10.0,-1);
    glVertex3f(400,-14.1,-250);
    glTexCoord3f(10.0,0.0,1);
    glVertex3f(400,-14.1,200);
    glEnd();
    glBindTexture( GL_TEXTURE_2D, 0 );
    glPopMatrix();
}

void snake()
{
    glPushMatrix();
    angle();
    plane();

    glColor3f(0,0.7,0);   // Head
    glTranslatef(x,-10.0,z);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(10,20,20);
    glRotatef(head, 0.0, 1.0, 0.0);

    glColor3f(1,1,0);   // Eye
    glTranslatef(-4.0,8.0,0.0);
    glScalef(0.3,0.3,0.3);
    glutSolidSphere(10,20,20);

    glTranslatef(26.0,0.0,0.0);
    glutSolidSphere(10,20,20);
    glPopMatrix();

    int i;
    for(i=0; i<length; i++)
    {
        glPushMatrix();
        angle();
        glTranslatef(body[0][i],-10.0,body[1][i]);
        if(i%2==0) glColor3f(0,0.85,0);
        else glColor3f(0,1,0);
        glScalef(0.65,0.65,0.65);
        glutSolidSphere(7,20,20);
        glPopMatrix();
    }
}

void food()
{
    glPushMatrix();
    angle();
    glTranslatef(foodx,-10.0,foodz);
    if(score%3==0) glColor3f(1, 1, 0);
    else if(score%2==1) glColor3f(1, 0, 0);
    else  glColor3f(1, 0, 1);
    glScalef(0.5,0.5,0.5);
    glutSolidSphere(7,20,20);
    glPopMatrix();
}

void status()
{
    char str[40];
    glColor3f(0, 0, 1);
    glRasterPos2f(-30, 65);
    sprintf(str, "Level: %d", level);
    text(str);
    glColor3f(1, 0, 0);
    glRasterPos2f(65, 65);
    sprintf(str, "Score: %d", score);
    text(str);
    glColor3f(0, 1, 0);
    glRasterPos2f(150, 65);
    sprintf(str, "Life: %d", life);
    text(str);
}

int random(int range1, int range2)
{
    return (rand() % (range1-range2))+range2;
}

void newFood()
{
    time_t sec;
    time(&sec);
    srand((unsigned int) sec);
    foodx = random(right-left, left+20);
    foodz = random(up-down, down+20);
}

bool collide()
{
    int i;
    for(i=0; i<length; i++)
    {
        if(((body[0][i] >= x) && (body[0][i] <= x + 5) && (body[1][i] >= z) && (body[1][i] <= z + 5)) ||
                ((body[0][i] <= x) && (body[0][i] >= x - 5) && (body[1][i] <= z) && (body[1][i] >= z - 5)) ||
                ((body[0][i] <= x) && (body[0][i] >= x - 5) && (body[1][i] >= z) && (body[1][i] <= z + 5)) ||
                ((body[0][i] >= x) && (body[0][i] <= x + 5) && (body[1][i] <= z) && (body[1][i] >= z - 5)))
            return true;
    }
    return false;
}

void moves(int value)
{
    x1[1] = x;
    z1[1] = z;
    switch(dir)
    {
    case 4: // right
        head =90;
        x += 6;
        if(x > right) x = left;
        break;
    case 3: // left
        head =-90;
        x -= 6;
        if(x < 0) x = right;
        break;
    case 1: // up
        head =0;
        z += 6;
        if(z > up) z = down;
        break;
    case 2: // down
        head =180;
        z -= 6;
        if(z < 0) z = up;
        break;
    }

    if(collide())
    {
        life--;
        if(life==0)
        {
            finish = true;
            if(score>=atoi(hs))
            {
                highscoreWrite();
            }
        }
    }

    if(((x >= foodx) && (x <= foodx + 5) && (z >= foodz) && (z <= foodz + 5)) ||
            ((x <= foodx) && (x >= foodx - 5) && (z <= foodz) && (z >= foodz - 5)) ||
            ((x <= foodx) && (x >= foodx - 5) && (z >= foodz) && (z <= foodz + 5)) ||
            ((x >= foodx) && (x <= foodx + 5) && (z <= foodz) && (z >= foodz - 5)))
    {
        score++;
        if(score < 150) length++;
        if(score % 10 == 0) level++;
        newFood();
        if(score == 150)
        {
            finish = true;
            highscoreWrite();
        }
    }

    int i;
    for(i = 0; i<length; i++)
    {
        x1[0] = x1[1];
        z1[0] = z1[1];
        x1[1] = body[0][i];
        z1[1] = body[1][i];
        body[0][i] = x1[0];
        body[1][i] = z1[0];
    }

    glutTimerFunc(105-level*7, moves, 0);
}

void Display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    lightSettings();
    glEnable(GL_TEXTURE_2D);

    glTranslatef (-70.0, 40.0, -300.0);

    if(finish == false)
    {
        status();
        food();
        snake();
    }
    else
    {
        startMenu();
    }

    glutPostRedisplay();
    glutSwapBuffers();
}

void Special(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_RIGHT:
        if(dir != 3) dir = 4;   // left
        break;
    case GLUT_KEY_LEFT:
        if(dir != 4) dir = 3;   // right
        break;
    case GLUT_KEY_UP:
        if(dir != 1) dir = 2;   // down
        break;
    case GLUT_KEY_DOWN:
        if(dir != 2) dir = 1;   // up
        break;
    }
}

void keyboard (unsigned char key, int x, int y)
{
    switch (key)
    {
    case 'S'    :
    case 's'    :
        gameReset() ;
        glutPostRedisplay();
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
}

int main(int argc, char **argv)
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowSize(1350,690);
    glutInitWindowPosition(0,0);
    glutCreateWindow("Hungry Snake");
    glutSpecialFunc(Special);
    glutKeyboardFunc(keyboard);
    glutDisplayFunc(Display);
    glutReshapeFunc(resize);

    newFood();
    moves(0);

    initialize();
    glutMainLoop();
}
