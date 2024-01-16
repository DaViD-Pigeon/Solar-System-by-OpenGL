#include<iostream>
#include<math.h>
#ifndef GLUT_DISABLE_ATEXIT_HACK
    #define GLUT_DISABLE_ATEXIT_HACK
#endif 
#include <GL/glut.h>

# define ASCII_ESC 27
# define MATH_PI 3.141592653589f

const int screenWidth = 1600;
const int screenHeight = 900;
// define the color of the sun, planet1, planet2 and satellite1
const float colorOfSun[3] = { 1.0, 0.0, 0.0 };
const float colorOfPlanet1[3] = { 0.8, 0.3, 0.3 };
const float colorOfPlanet2[3] = { 0.3, 0.3, 0.8 };
const float colorOfSatellite1[3] = { 0.8, 0.8, 0.8 };
const float colorOfOrbit[3] = { 1.0, 1.0, 1.0 };
// define the radius of the sun, planet1, planet2 and satellite1
const float sphereOfSun[3] = { 90, 50, 50 };
const float sphereOfPlanet1[3] = { 30, 20, 20 };
const float sphereOfPlanet2[3] = { 45, 40, 40 };
const float sphereOfSatellite1[3] = { 10, 10, 10 };
// define the orbit's radius of the sun, planet1, planet2 and satellite1
const int orbitOfSun = 400;
const int orbitOfPlanet1 = 150;
const int orbitOfPlanet2 = 280;
const int orbitOfSatellite1 = 40;
//define the revolution speed
const float speedOfSun = 360.0 / 750;
const float speedOfPlanet1 = 360.0 / 365;
const float speedOfPlanet2 = 360.0 / 500;
const float speedOfSatellite1 = 360.0 / 29.32;
// define the angle of the self-rotation
const float angleOfSun = 0.0;
const float angleOfPlanet1 = 10.0;
const float angleOfPlanet2 = 25.0;
const float angleOfSatellite1 = 5.0;
// define the eye position
float eyePosition[3] = { 0.0, 200.0, 1000.0 };
// define the camera angle
float camera_angle_v = 0.0;
float camera_angle_h = 0.0;
// define the drag position
int drag_x_origin = 0;
int drag_y_origin = 0;
// set the mouse sensitivity
float mouse_sensitivity = 0.1;

GLint day = 0;
bool rotate = true;
bool isDragging = false;

void draw_circle(int radius);
void draw_orbit(int radius); 
void Set_star(const float *color, const float *sphere);

void display(void); // 显示回调函数(展示图形)
void reshape(int w, int h); // 窗口大小改变回调函数
void onKeyboard(unsigned char key, int x, int y); // 键盘回调函数
void onSpecialKey(int key, int x, int y);// 特殊键盘回调函数
void idle(); // 空闲回调函数
void mouseButton(int button, int state, int x, int y); // 鼠标点击回调函数
void mouseMove(int x, int y); // 鼠标移动回调函数

int main(int argc, char** argv){
    glutInit(&argc, argv);
    // 设置显示模式，包括双缓冲、RGB颜色模式和深度测试
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    // 设置窗口大小
    glutInitWindowSize(screenWidth, screenHeight);
    // 设置窗口位置
    glutInitWindowPosition(0, 0);
    glutCreateWindow("double solar system");
    // 设置背景颜色
    glClearColor(0.0, 0.0, 0.0, 0.0);

    glEnable(GL_DEPTH_TEST); // 启用深度测试
    glEnable(GL_NORMALIZE); // 启用法向量规范化
    glEnable(GL_SMOOTH); // 启用阴影平滑
    
    // registering callbacks function
    glutKeyboardFunc(onKeyboard); // 键盘事件
    glutSpecialFunc(onSpecialKey); // 特殊键盘事件
    glutDisplayFunc(display); // 显示事件
    glutIdleFunc(idle); // 空闲事件
    glutReshapeFunc(reshape); // 窗口大小改变事件 
    glutMouseFunc(mouseButton); // 鼠标点击事件
    glutMotionFunc(mouseMove); // 鼠标移动事件

    // 进入GLUT事件处理循环，等待用户输入和处理相关事件
    glutMainLoop();
    return 0;
}

void draw_circle(int radius) {
    /*
    @ r: 半径
    @ numSegments: 分割的线段数
    */
    int numSegments = 10000;
    glBegin(GL_LINE_LOOP); 

    for (int i = 0; i < numSegments; i++) {
        float theta = 2.0f * MATH_PI * float(i) / float(numSegments);  // 角度
        float x = radius * cosf(theta);  // X坐标
        float y = radius * sinf(theta);  // Y坐标
        glVertex2f(x, y); // 输出顶点
    }
    glEnd();
}

void draw_orbit(int radius){
    glRotatef(-90.0, 1.0, 0.0, 0.0); // restore the coordinate system
    glColor3f(colorOfOrbit[0], colorOfOrbit[1], colorOfOrbit[2]); // set the color of the orbit
    draw_circle(radius); // define the orbit of the sun
    glRotatef(90, 1.0, 0.0, 0.0); // used to draw a circle on the x-y plane     
}

void Set_star(const float *color, const float *sphere) {
    glColor3f(color[0], color[1], color[2]); // set the color of the s
    glutSolidSphere(sphere[0], sphere[1], sphere[2]); //define the star
}

void display(void) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); //清除颜色缓冲区和深度缓冲区
    if (rotate) day += 1; // 旋转
    // the solar system 1
    glLoadIdentity();
    gluLookAt(eyePosition[0], eyePosition[1], eyePosition[2], 0.0, 0.0, 0.0, 0.0, 1.0, 0.0); // set the camera position
    // set the camera angle
    glRotated(camera_angle_v, 1.0, 0.0, 0.0);
    glRotated(camera_angle_h, 0.0, 1.0, 0.0); 
    glPushMatrix();
    {
        //draw the sun 
        glRotatef(day * speedOfSun, 0.0, 1.0, 0.0); // set the revolution of the sun
        draw_orbit(orbitOfSun); // define the orbit of the sun
        glTranslatef(orbitOfSun, 0.0, 0.0); // set the position of the sun
        Set_star(colorOfSun, sphereOfSun); // define the sun1
        //draw the planet1
        glPushMatrix();
        { 
            glRotatef(angleOfPlanet1,0.0,0.0,1.0);
            draw_orbit(orbitOfPlanet1); // define the orbit of the planet1
            glRotatef(day * speedOfPlanet1, 0.0, 1.0, 0.0); // set the revolution of the planet1
            glTranslatef(orbitOfPlanet1, 0.0, 0.0); // set the position of the planet1
            Set_star(colorOfPlanet1, sphereOfPlanet1); // define the planet1
            //draw the satellite1
            glPushMatrix();
            {
                glRotatef(angleOfSatellite1,0.0,0.0,1.0); // set the self-rotation of the satellite1
                draw_orbit(orbitOfSatellite1); // draw the orbit of the satellite1
                glRotatef(day* speedOfSatellite1, 0.0, 1.0, 0.0); // set the revolution of the satellite1
                glTranslatef(orbitOfSatellite1, 0.0, 0.0); // set the position of the satellite1
                Set_star(colorOfSatellite1, sphereOfSatellite1); // define the satellite1
            }
            glPopMatrix();
        }
        glPopMatrix();
        //draw the planet2
        glPushMatrix();
        { 
            glRotatef(angleOfPlanet2,0.0,0.0,1.0);
            draw_orbit(orbitOfPlanet2); // define the orbit of the planet2
            glRotatef(day * speedOfPlanet2, 0.0, 1.0, 0.0); // set the revolution of the planet2
            glTranslatef(orbitOfPlanet2, 0.0, 0.0); // set the position of the planet1
            Set_star(colorOfPlanet2, sphereOfPlanet2); // define the planet2
        }
        glPopMatrix();
    }
    glPopMatrix();

    // the solar system 2
    glPushMatrix();
    {
        //draw the sun 
        glRotatef(day * speedOfSun, 0.0, 1.0, 0.0); // set the revolution of the sun
        glTranslatef(-orbitOfSun, 0.0, 0.0); // set the position of the sun
        Set_star(colorOfSun, sphereOfSun); // define the sun1
        //draw the planet1
        glPushMatrix();
        { 
            glRotatef(angleOfPlanet1,0.0,0.0,1.0);
            draw_orbit(orbitOfPlanet1); // define the orbit of the planet1
            glRotatef(day * speedOfPlanet1, 0.0, 1.0, 0.0); // set the revolution of the planet1
            glTranslatef(-orbitOfPlanet1, 0.0, 0.0); // set the position of the planet1
            Set_star(colorOfPlanet1, sphereOfPlanet1); // define the planet1
            //draw the satellite1
            glPushMatrix();
            {
                glRotatef(angleOfSatellite1,0.0,0.0,1.0); // set the self-rotation of the satellite1
                draw_orbit(orbitOfSatellite1); // draw the orbit of the satellite1
                glRotatef(day* speedOfSatellite1, 0.0, 1.0, 0.0); // set the revolution of the satellite1
                glTranslatef(-orbitOfSatellite1, 0.0, 0.0); // set the position of the satellite1
                Set_star(colorOfSatellite1, sphereOfSatellite1); // define the satellite1
            }
            glPopMatrix();
        }
        glPopMatrix();
        //draw the planet2
        glPushMatrix();
        { 
            glRotatef(angleOfPlanet2,0.0,0.0,1.0);
            draw_orbit(orbitOfPlanet2); // define the orbit of the planet2
            glRotatef(day * speedOfPlanet2, 0.0, 1.0, 0.0); // set the revolution of the planet2
            glTranslatef(-orbitOfPlanet2, 0.0, 0.0); // set the position of the planet1
            Set_star(colorOfPlanet2, sphereOfPlanet2); // define the planet2
        }
        glPopMatrix();
    }
    glPopMatrix();
    Sleep(50); // 延时50ms
    glutSwapBuffers();

}

void reshape(int w, int h) {
    glViewport(0, 0, (GLsizei)w, (GLsizei)h);
    glMatrixMode(GL_PROJECTION); // 设置投影矩阵
    glLoadIdentity();
    gluPerspective(60.0, (GLfloat)w / (GLfloat)h, 0.01f, 2000.0f);
    glMatrixMode(GL_MODELVIEW); // 设置模型视图矩阵
    glLoadIdentity();
    gluLookAt(0.0,200.0,1000.0,0.0,0.0,0.0,0.0,1.0,0.0);
}
void onKeyboard(unsigned char key, int x, int y) {
    switch (key){
        case 'r':
            rotate = !rotate;
            break;
        case ASCII_ESC:
            exit(0);
            break;
        case 'w':
            eyePosition[1] += 10;
            break;
        case 's':
            eyePosition[1] -= 10;
            break;
        case 'a':
            eyePosition[0] -= 10;
            break;
        case 'd':
            eyePosition[0] += 10;
            break;
        default:
            break;
    }
}

void onSpecialKey(int key, int x, int y) {
    switch (key){
        case GLUT_KEY_UP:
            eyePosition[2] -= 10;
            break;
        case GLUT_KEY_DOWN:
            eyePosition[2] += 10;
            break;
        default:
            break;
    }
}
void idle() {
    glutPostRedisplay(); // 重绘
}
void mouseButton(int button, int state, int x, int y){
    if(button == GLUT_LEFT_BUTTON){
        if(state == GLUT_DOWN){
            isDragging = true;
            drag_x_origin = x;
            drag_y_origin = y;
        }
        else{
            isDragging = false;
        }
    }
}

void mouseMove(int x, int y){
    if(isDragging){
        camera_angle_h += (x - drag_x_origin) * mouse_sensitivity;
        camera_angle_v += (y - drag_y_origin) * mouse_sensitivity;
        drag_x_origin = x;
        drag_y_origin = y;
    }
    return;
}