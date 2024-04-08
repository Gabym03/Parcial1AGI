#include "./minerva.h"
#include <GL/glut.h>
#include <GL/glu.h>
#include <GL/gl.h>
#include <GL/freeglut.h>
#include <math.h>
#include <stdlib.h>
int ssd=1;
int sd=2;
int d=3;
int n=4;
int g=5;
int sp=6;
int spg=7;
int spgg=8;

void dibujar_maya();
void dibujar_pelo();
void dibujar_contorno();

void inicializar_minerva(void){
    glClearColor(0.8, 0, 0,1);
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(0,700,0,700);
}

void dibujar_minerva(){
   glClear (GL_COLOR_BUFFER_BIT);
   glColor3f(1, 1, 1);
   dibujar_pelo();
   dibujar_maya(5,2);
   dibujar_contorno();
   glFlush();
}

const char* get_carnet(void){
    return "MG21054";
}

void lineam(float xa, float ya,float xn, float yn,int tmanio){
    float x1;
    float y1;
    float x2;
    float y2;
    if(xa<xn){
        x1 = xa;
        y1 = ya;
        x2 = xn;
        y2 = yn;
    }else{
        x1 = xn;
        y1 = yn;
        x2 = xa;
        y2 = ya;
    }
	float dx = x2 - x1;
    float dy = y2 - y1;
    float m;
    if (dy==0 || dy>0 || dy<0 || dx<0 || dx>0){
        if(abs(dx)<abs(dy) && dx!=0){
            m = dy/dx;
            float b = y1 - m*x1;
            float y;
            glPointSize(tmanio);
            glBegin(GL_POINTS);
            if(y1<y2){
                for (y1; y1 < y2; y1++) {
                    x1 = round((y1 - b) / m);
                    glVertex2i(x1, y1);
                }
            }else{
                
                for (y2; y2 < y1; y2++) {
                    x1 = round((y2 - b) / m);
                    glVertex2i(x1, y2);
                }

            }
            glEnd();
        }else{
            m = dy/dx;
            float b = y1 - m*x1;
            float y;
            glPointSize(tmanio);
            glBegin(GL_POINTS);
            for (x1; x1 < x2; x1++) {
                y = round(m*x1 + b);
                glVertex2i(x1, y);
            }
            glEnd();
        }
    }
    if (dx==0){
        if (ya<yn){
            y1 = ya;
            y2 = yn;
        }else{
            y1 = yn;
            y2 = ya;
        }
        glPointSize(tmanio);
        glBegin(GL_POINTS);
        for (y1; y1 < y2; y1++) {
            glVertex2i(x1, y1);
        }
        glEnd();

    }
}

void dda(float x1, float y1,float x2, float y2,int tamanio){
	float dx = x2 - x1;
    float dy = y2 - y1;
    float pasos, incrementoX, incrementoY, x = x1, y = y1;
    pasos = abs(dx) > abs(dy) ? abs(dx) : abs(dy);
    incrementoX = dx / pasos;
    incrementoY = dy / pasos;
    glPointSize(tamanio);
    glBegin(GL_POINTS);
    for (float i = 0; i < pasos; i++) {
        glVertex2f(x, y);
        x += incrementoX;
        y += incrementoY;
    }
    glEnd();
}
void brezenham(int x1, int y1, int x2, int y2, int size) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int Pk;
    int Xk = x1;
    int Yk = y1;
    glPointSize(size);
    glBegin(GL_POINTS);
    glVertex2i(Xk, Yk);

    if (dx >= dy) {
        int Pk_inc = 2 * dy;
        int Pk_dec = 2 * dy - 2 * dx;
        int inc = y1 < y2 ? 1 : -1;
        Pk = 2 * dy - dx;
        while (Xk != x2) {
            Xk += (x1 < x2 ? 1 : -1);
            if (Pk < 0)
                Pk += Pk_inc;
            else {
                Yk += inc;
                Pk += Pk_dec;
            }
            glVertex2i(Xk, Yk);
        }
    } else {
        int Pk_inc = 2 * dx;
        int Pk_dec = 2 * dx - 2 * dy;
        int inc = x1 < x2 ? 1 : -1;
        Pk = 2 * dx - dy;
        while (Yk != y2) {
            Yk += (y1 < y2 ? 1 : -1);
            if (Pk < 0)
                Pk += Pk_inc;
            else {
                Xk += inc;
                Pk += Pk_dec;
            }
            glVertex2i(Xk, Yk);
        }
    }
    glEnd();
}
void replinea(float x1, float y1,float x2, float y2, float x3, float y3,float x4, float y4, int pix, char algoritmo){
	float dx = x2 - x1;
    float dy = y2 - y1;
    float dx2 = x4 - x3;
    float dy2 = y4 - y3;
    float xIncrementos, yIncrementos, xxIncrementos, yyIncrementos, x = x1, y = y1,  xx = x3, yy = y3, pasos, pasos2, pasosx;
    pasos = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
    pasos2 = fabs(dx2) > fabs(dy2) ? fabs(dx2) : fabs(dy2);
    pasosx = fabs(pasos) > fabs(pasos2) ? fabs(pasos) : fabs(pasos2);
    xIncrementos = dx / pasosx;
    yIncrementos = dy / pasosx;
    xxIncrementos = dx2 / pasosx;
    yyIncrementos = dy2 / pasosx;
    for (float i = 0; i < pasosx; i++) {
        switch (algoritmo) {
            case 'l':
                lineam(x, y, xx, yy, pix);
                break;
            case 'b':
                brezenham(x, y, xx, yy, pix);
                break;
            case 'd':
                dda(x, y, xx, yy, pix);
                break;
            default:
                break;
        }
        x += xIncrementos;
        y += yIncrementos;
        xx += xxIncrementos;
        yy += yyIncrementos;
        
    }
}
void dibujar_contorno(){
   lineam(219,490,224,500,n);
   lineam(224,500,230,510,n);
   lineam(230,510,236,520,g);
   lineam(236,520,243,529,g);
   lineam(243,529,252,542,g);
   lineam(252,542,256,547,g);
   lineam(256,547,267,567,n);
   lineam(267,567,276,585,g);
   lineam(276,585,285,603,g);
   lineam(285,603,290,616,g);
   lineam(290,616,296,626,g);
   lineam(296,626,302,635,g);
   lineam(302,635,310,644,g);
   lineam(310,644,317,649,g);
   lineam(317,649,325,655,g);
   lineam(325,655,335,659,g);
   lineam(335,659,345,660,g);
   lineam(345,660,355,659,g);
   lineam(355,659,365,657,g);
   lineam(365,657,375,655,g);
   lineam(375,655,385,650,g); //z
   lineam(385,650,393,645,g);
   lineam(393,645,400,640,g);
   lineam(400,640,406,634,g);
   lineam(406,634,416,629,g);
   lineam(416,629,426,624,g);
   lineam(426,624,437,618,g);
   lineam(437,618,448,608,g);
   lineam(448,608,457,600,g);
   lineam(457,600,465,591,n);
   lineam(465,591,474,578,n);
   lineam(474,578,478,568,n);
   lineam(478,568,480,560,d);
   lineam(480,560,481,554,d);
   lineam(481,554,482,547,d);
   lineam(482,547,480,540,g);
   lineam(480,540,483,530,g);
   lineam(483,530,490,521,n);
   lineam(490,521,494,513,n);
   lineam(494,513,497,505,n);
   lineam(497,505,497,495,n);
   lineam(497,495,496,486,n);
   lineam(496,486,493,475,n);
   lineam(493,475,488,464,n);
   lineam(488,464,481,452,n);
   lineam(481,452,476,441,n);//z1
   lineam(476,441,471,431,d);
   lineam(471,431,461,410,d);
   //mono
   lineam(461,410,460,371,n);//c2d2
   lineam(460,371,468,375,g);
   lineam(468,375,476,375,g);
   lineam(476,375,484,373,g);
   lineam(484,373,492,369,g);
   lineam(492,369,500,360,g);
   lineam(500,360,505,351,g);
   lineam(505,351,508,340,g);
   lineam(508,340,509,329,n);
   lineam(509,329,509,317,n);
   lineam(509,317,508,307,n);
   lineam(509,317,504,304,n);//linea
   lineam(508,307,506,297,n);
   lineam(506,297,504,293,n);
   lineam(504,293,501,288,g);
   lineam(501,288,496,283,g);
   lineam(496,284,493,280,g);
   lineam(493,280,488,277,g);
   lineam(488,277,480,272,g);
   lineam(480,272,474,269,g);
   lineam(474,269,467,267,g);//Z2
   lineam(467,267,460,265,g);
   lineam(460,265,450,264,sp);
   lineam(450,264,440,265,sp);
   lineam(440,265,432,267,sp);
   lineam(432,267,425,272,sp);
   lineam(425,272,419,277,g);
   lineam(419,277,411,285,g);
   lineam(411,285,408,290,n);
   lineam(408,290,404,295,d);
   lineam(404,295,404,298,2);
   //cuello derecho
   lineam(431,267,432,259,n);
   lineam(432,259,436,244,n);
   lineam(436,244,440,231,n);
   lineam(440,231,445,220,n);
   lineam(445,220,450,212,n);
   lineam(450,212,456,204,g);
   lineam(456,204,468,190,g);
   lineam(468,190,470,188,g);
   lineam(470,188,475,181,sp);//t3, empieza la camisa
   lineam(475,181,476,173,sp);
   lineam(476,173,477,167,sp);
   lineam(477,167,477,158,sp);
   lineam(477,158,476,150,sp);
   lineam(476,150,475,140,sp);//a4
   lineam(475,140,473,130,sp);
   lineam(473,130,465,109,sp);//falta mejorar la linea que sobresale
   lineam(465,109,462,100,g);
   lineam(462,100,456,90,g);
   lineam(456,90,450,80,g);
   lineam(450,80,444,70,g);
   lineam(444,70,436,59,n);
   lineam(436,59,431,52,n);
   lineam(431,52,426,46,d);
   lineam(426,46,421,42,d);
   lineam(421,42,416,39,d);
   lineam(416,39,409,34,d);
   lineam(409,34,400,30,d);
   lineam(400,30,395,28,d);
   lineam(395,28,390,26,d);
   lineam(390,26,386,25,d);
   lineam(386,25,377,25,d);
   lineam(377,25,365,25,n);
   lineam(365,25,340,25,n);
   lineam(340,25,330,25,g);
   lineam(330,25,316,27,g);
   lineam(316,27,303,31,g);
   lineam(303,31,288,37,g);
   lineam(288,37,277,43,g);
   lineam(277,43,265,52,sp);
   lineam(265,52,260,54,n);
   //d5
   lineam(266,49,264,54,sp);
   lineam(264,55,263,59,sp);
   lineam(260,55,265,65,n);
   lineam(263,59,265,65,n);
   lineam(264,65,267,74,g);
   lineam(267,74,269,78,g);
   lineam(269,78,271,81,g); 
   lineam(271,81,275,84,g);
   lineam(275,84,280,86,g);
   lineam(280,86,285,88,n);
   lineam(285,88,290,90,n);
   lineam(290,90,294,93,n);
   lineam(294,93,299,97,n);
   lineam(299,97,305,101,g);
   lineam(305,101,310,106,g);
   lineam(310,106,315,111,g);
   lineam(315,111,319,115,g);
   lineam(319,115,323,119,g);
   lineam(323,119,330,125,g);
   lineam(330,125,340,134,g);
   lineam(340,134,348,140,g);
   lineam(348,140,356,145,n);
   lineam(356,145,365,152,n);//a6
   lineam(365,152,372,156,n);
   lineam(372,156,381,162,n);
   lineam(381,162,391,167,n);
   lineam(391,167,402,171,n);
   lineam(391,167,402,171,d);
   lineam(402,171,414,176,d);
   lineam(414,176,428,180,d);
   lineam(428,180,440,184,d);
   lineam(440,184,450,186,d);
   lineam(450,186,460,187,d);
   lineam(460,187,470,188,n);
   //cuello izquierdo
   lineam(258,64,257,67,n);
   lineam(257,67,258,74,n);
   lineam(258,74,260,82,n);
   lineam(260,82,263,86,n);
   lineam(263,86,268,93,n);
   lineam(268,93,274,100,n);
   lineam(274,100,280,107,n);
   lineam(280,107,286,112,n);
   lineam(286,112,292,120,n);
   lineam(292,120,298,126,n);
   lineam(298,126,303,134,n);//z6
   lineam(303,134,308,142,g);
   lineam(308,142,311,151,g);
   lineam(311,151,315,160,g);
   lineam(315,160,318,170,g);
   lineam(318,170,321,182,g);
   lineam(321,182,323,192,g);
   lineam(323,192,324,204,g);
   lineam(324,204,324,214,g);
   //barbilla
   lineam(377,247,371,242,d);
   lineam(371,242,366,237,d);
   lineam(366,237,361,233,d);
   lineam(361,233,353,228,d);
   lineam(353,228,348,225,n);
   lineam(348,225,342,222,n);
   lineam(342,222,335,218,n);
   lineam(335,218,329,216,n);
   lineam(329,216,324,214,g);
   lineam(324,214,314,212,g);
   lineam(314,212,304,210,g);
   lineam(304,210,297,210,g);
   lineam(297,210,288,210,g);
   lineam(288,210,276,210,g);
   lineam(276,210,270,211,g);//puede ser mas grueso
   lineam(270,211,265,214,g);
   lineam(265,214,260,218,sp);
   lineam(260,218,257,221,sp);
   lineam(257,221,254,226,sp);
   lineam(254,226,253,232,sp);
   lineam(253,232,253,236,sp);
   lineam(253,236,253,242,g);
   lineam(253,242,252,246,g);
   lineam(252,246,249,248,g);
   lineam(250,248,246,250,g);
   lineam(246,250,243,252,sp);//i8
   //labio
   lineam(243,252,238,253,sp);//j8
   lineam(238,253,236,256,sp);
   lineam(236,256,236,260,sp);
   lineam(236,260,241,265,g);
   lineam(236,260,241,265,g);
   lineam(241,265,259,265,g);
   lineam(259,265,260,266,n);
   lineam(260,266,257,270,d);
   lineam(257,270,254,270,n);
   lineam(254,270,251,267,n);
   lineam(251,267,248,267,n);//s8
   lineam(248,267,244,269,n);//T8
   lineam(244,269,242,270,n);
   lineam(242,270,236,266,n);
   //labio superior
   lineam(236,263,230,267,sp);
   lineam(230,267,227,269,sp);
   lineam(227,269,226,270,sp);
   lineam(226,270,225,272,sp);
   lineam(225,272,226,275,g);
   lineam(226,275,227,278,g);
   lineam(227,278,231,280,n);
   lineam(228,280,227,283,g);
   //nariz
   replinea(234,306,239,300,234,306,243,299,d,'l');
   replinea(239,300,243,299,243,295,239,295,d,'l');
   replinea(243,295,243,291,239,295,243,291,d,'l');
   replinea(243,291,237,287,235,292,239,295,d,'l');
   replinea(237,287,231,287,235,292,230,291,d,'l');
   replinea(230,287,226,283,230,291,224,288,d,'l');
   replinea(220,287,224,288,226,283,221,282,d,'l');
   lineam(220,285,217,281,g);
   lineam(217,281,212,282,sp);
   lineam(211,282,209,284,g);
   lineam(209,284,205,287,g);
   lineam(205,287,205,291,g);
   lineam(205,291,206,300,n);
   lineam(206,300,207,308,g);//r11
   lineam(207,308,209,321,g);
   lineam(209,321,211,335,g);
   lineam(211,335,213,351,g);
   lineam(213,351,213,366,sp);
   lineam(213,366,212,371,sp);
   lineam(212,371,209,378,sp);
   lineam(209,378,207,380,g);
   lineam(207,380,207,385,g);
   lineam(207,385,207,411,g);
   lineam(207,411,207,430,g);
   lineam(207,430,207,436,n);
   //ceja
   //linea abajo
   lineam(284,379,250,379,d);
   lineam(280,380,270,380,d);
   lineam(270,381,247,382,d);
   lineam(250,380,244,380,d);
   lineam(244,380,241,379,d);
   lineam(241,379,239,378,d);
   lineam(239,378,238,377,d);
   lineam(238,377,238,376,d);
   lineam(238,376,238,375,d);
   lineam(238,375,239,374,d);
   lineam(239,374,240,373,d);
   lineam(240,373,241,372,d);
   lineam(241,372,243,371,d);
   lineam(243,371,246,370,d);
   //linea arriba
   lineam(257,383,245,383,d);
   lineam(245,383,240,383,n);
   lineam(240,383,237,381,n);
   lineam(237,381,234,380,n);
   lineam(234,380,232,378,n);
   lineam(232,378,230,376,n);
   lineam(230,376,229,374,n);
   lineam(229,374,229,372,n);
   lineam(229,372,231,370,n);
   lineam(231,370,233,369,n);
   lineam(233,369,236,368,n);
   lineam(236,368,245,367,n);
   //relleno ceja
   lineam(238,378,232,374,n);
   lineam(232,374,240,370,n);
   lineam(244,369,236,374,n);
   //parte grande ojo
   replinea(246,370,263,366,243,357,275,359,n,'l');
   //pancita del ojo
   lineam(242,360,240,361,n);
   lineam(240,361,239,363,n);
   lineam(239,363,239,364,n);
   lineam(239,364,239,366,n);
   replinea(243,357,275,359,251,347,271,354,d,'l');
   //lagrimita
   replinea(252,325,257,332,252,325,251,332,d,'l');
   lineam(253,332,253,335,spg);
   lineam(253,335,252,338,spg);
   lineam(252,338,250,340,spg);
   lineam(250,340,248,342,spg);
   lineam(248,342,248,344,spg);
   lineam(248,344,249,346,spg);
   lineam(249,346,253,349,spg);
   //pico lagrima
   lineam(261,343,259,342,sd);
   lineam(259,342,254,342,sd);
   lineam(259,342,256,339,sd);
   //extras
   //replinea(466,175,474,184,466,175,477,176,d,'l');
   lineam(466,175,468,177,d);
   lineam(468,177,471,183,d);
   lineam(468,177,474,174,d);
   lineam(468,177,473,179,d);
   lineam(289,48,287,45,d);
   lineam(287,45,281,42,d);
   lineam(287,45,288,37,d);
   //rojo
   glColor3f(0.8, 0, 0);
   lineam(250,354,253,351,sd);
   lineam(256,364,244,364,sd);
   lineam(252,365,244,365,sd);
   replinea(259,351,263,354,259,351,256,354,d,'l');
   replinea(256,354,255,356,263,354,262,356,d,'l');
   glColor3f(1,1,1);
}
void Circulo(float centerX, float centerY, float radius, int tmanio, char metodo) {
    const int numSegments = 360; 
    const float angleIncrement = 2 * 3.14159265358979323846 / numSegments; // Incremento angular para cada segmento
    float x1, y1, x2, y2;
    for (int i = 0; i < numSegments; ++i) {
        float theta1 = angleIncrement * i;
        float theta2 = angleIncrement * (i + 1);

        x1 = centerX + radius * cos(theta1);
        y1 = centerY + radius * sin(theta1);
        x2 = centerX + radius * cos(theta2);
        y2 = centerY + radius * sin(theta2);
        if (metodo == 'm'){
            lineam(x1, y1, x2, y2, tmanio);
            lineam(centerX, centerY, x1, y1, tmanio);
        }else if(metodo == 'd'){
            dda(x1, y1, x2, y2, tmanio);
            dda(centerX, centerY, x1, y1, tmanio);
        }else if(metodo == 'b'){
            brezenham(x1, y1, x2, y2, tmanio);
            brezenham(centerX, centerY, x1, y1, tmanio);
        }
    }
}

void brezenham_Gotas(int x1, int y1, int x2, int y2, float tmanio, int pix) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int Pk;
    int Xk = x1;
    int Yk = y1;
    int inc;

    Circulo(Xk, Yk, tmanio, pix, 'b');

    int steps = dx >= dy ? dx : dy;
    float sizeIncrement = tmanio / steps;

    if (dx >= dy) {
        int Pk_inc = 2 * dy;
        int Pk_dec = 2 * dy - 2 * dx;
        inc = y1 < y2 ? 1 : -1;
        Pk = 2 * dy - dx;
        for (int i = 0; i < steps; i++) {
            Xk += (x1 < x2 ? 1 : -1);
            if (Pk < 0)
                Pk += Pk_inc;
            else {
                Yk += inc;
                Pk += Pk_dec;
            }
            tmanio -= sizeIncrement;
            Circulo(Xk, Yk, tmanio, pix, 'b');
        }
    } else {
        int Pk_inc = 2 * dx;
        int Pk_dec = 2 * dx - 2 * dy;
        inc = x1 < x2 ? 1 : -1;
        Pk = 2 * dx - dy;
        for (int i = 0; i < steps; i++) {
            Yk += (y1 < y2 ? 1 : -1);
            if (Pk < 0)
                Pk += Pk_inc;
            else {
                Xk += inc;
                Pk += Pk_dec;
            }
            tmanio -= sizeIncrement;
            Circulo(Xk, Yk, tmanio, pix, 'b');
        }
    }
}


void dibujar_maya(){
    //gotita1
    brezenham_Gotas(278,66,270,51,3,1);
    //gotita2
    brezenham_Gotas(293,81,283,70,3,1);
    //gotita3
    brezenham_Gotas(315,96,304,87,3,1);
    //gota4
    brezenham_Gotas(332,112,327,105,3,1);
    brezenham_Gotas(332,112,337,117,2.75,1);
    brezenham_Gotas(332,112,335,117,2.5,1);
    brezenham_Gotas(332,112,337,115,2.5,1);
    //GOTA
    brezenham_Gotas(354,132,347,124,2.75,1);
    brezenham_Gotas(354,132,357,137,2.75,1);
    brezenham_Gotas(354,132,361,138,2.5,1);
    //gota
    brezenham_Gotas(380,150,373,143,2.75,1);
    brezenham_Gotas(380,150,384,156,2.5,1);
    brezenham_Gotas(380,150,386,156,2.5,1);
    brezenham_Gotas(380,150,386,154,2.5,1);
    //gota
    brezenham_Gotas(407,163,399,158,2.75,1);
    brezenham_Gotas(407,163,410,167,2.5,1);
    brezenham_Gotas(407,163,414,167,2.5,1);
    brezenham_Gotas(407,163,413,164,2.5,1);
    //gota
    brezenham_Gotas(429,170,436,173,3,1);
    brezenham_Gotas(429,170,423,166,3,1);
    brezenham_Gotas(429,170,433,173,2.75,1);
    brezenham_Gotas(429,170,436,171,2.75,1);
    //gota
    replinea(445,173,449,179,457,173,460,179,2,'b');
    //gotas
    brezenham_Gotas(466,169,456,162,3.5,1);//aqui sigo o26
    brezenham_Gotas(453,160,458,163,3.5,1);
    brezenham_Gotas(453,160,445,156,3.5,1);
    //gota
    brezenham_Gotas(439,164,430,160,3,1);
    //gota
    brezenham_Gotas(415,155,408,150,3,1);
    brezenham_Gotas(415,155,422,158,2.75,1);
    brezenham_Gotas(415,155,422,157,2.5,1);
    //gota
    brezenham_Gotas(390,145,384,140,2,1);
    brezenham_Gotas(390,145,397,150,2,1);
    brezenham_Gotas(390,145,398,149,2,1);

    brezenham_Gotas(371,133,364,127,3,1);

    brezenham_Gotas(351,115,346,109,3,2);
    brezenham_Gotas(351,115,356,120,3,2);

    brezenham_Gotas(327,94,320,89,3,1);
    brezenham_Gotas(327,94,335,99,3,1);

    brezenham_Gotas(309,81,302,75,2,1);
    brezenham_Gotas(302,75,309,81,2,1);

    brezenham_Gotas(283,56,292,67,2,2);

    brezenham_Gotas(298,58,305,64,2,1);
    brezenham_Gotas(305,64,298,58,2,1);

    brezenham_Gotas(317,71,326,80,3,2);

    brezenham_Gotas(347,95,337,88,2,2);

    brezenham_Gotas(368,116,361,107,2,2);

    brezenham_Gotas(389,132,383,127,2,2);

    brezenham_Gotas(409,143,397,135,3,1);

    brezenham_Gotas(435,153,426,149,3,2);

    brezenham_Gotas(420,137,413,132,3,1);
    brezenham_Gotas(420,137,425,141,3,1);

    brezenham_Gotas(401,127,391,118,3,1);

    brezenham_Gotas(379,109,370,100,2,2);

    brezenham(359,91,348,81,4);

    brezenham_Gotas(335,71,326,63,2,2);

    brezenham_Gotas(322,52,313,42,3,1);

    brezenham_Gotas(301,47,309,56,3,1);

    brezenham_Gotas(332,46,322,35,3,1);

    brezenham_Gotas(348,42,338,31,3,1);

    brezenham_Gotas(349,71,340,62,2,1);

    brezenham_Gotas(354,61,346,52,2,1);

    brezenham_Gotas(362,41,356,34,2,1);

    brezenham_Gotas(378,39,371,31,2,1);

    brezenham_Gotas(364,55,358,49,3,1);
    brezenham_Gotas(364,55,369,59,3,1);

    brezenham_Gotas(380,54,375,48,2,2);
    brezenham_Gotas(380,54,386,59,2,2);

    brezenham_Gotas(401,54,393,46,2,2);

    brezenham_Gotas(395,37,389,32,2,1);
    brezenham_Gotas(395,37,401,41,2,2);

    brezenham_Gotas(409,45,415,51,1,2);

    brezenham_Gotas(433,62,425,56,3,1);

    brezenham_Gotas(442,83,434,77,2,2);

    brezenham_Gotas(441,97,450,104,1,3);

    brezenham_Gotas(449,117,441,110,2,2);

    brezenham_Gotas(457,128,463,132,2,2);
    brezenham(455,136,458,142,3);

    brezenham_Gotas(468,152,475,154,2,3);
    brezenham_Gotas(451,149,444,146,2,1);
    brezenham_Gotas(437,137,430,132,2,1);//O30
    brezenham_Gotas(414,123,404,115,2,1);//Q30
    brezenham_Gotas(391,106,383,97,2,2);//s30
    brezenham_Gotas(364,80,372,89,2,2);//u30
    brezenham_Gotas(378,80,367,69,2,2);//w30
    brezenham_Gotas(390,75,380,65,2,2);//a31
    brezenham_Gotas(397,65,408,75,2,1);//c31

    brezenham_Gotas(420,68,413,62,2,2);
    brezenham_Gotas(420,68,428,74,2,2);//e31

    brezenham_Gotas(417,80,428,88,1,3);//h31

    brezenham_Gotas(432,107,423,100,3,1);//j31

    brezenham_Gotas(408,88,401,83,3,1);//l31
    brezenham_Gotas(408,88,415,95,2,2);

    brezenham_Gotas(395,93,387,87,2,2);//o31
    brezenham_Gotas(395,93,405,101,2,1);
    brezenham_Gotas(421,114,405,101,3,1);//r31

    brezenham_Gotas(442,127,431,121,2,2);








    





}
void ddac(float x1, float y1,float x2, float y2, float tmanio, int pix){

	float dx = x2 - x1;
    float dy = y2 - y1;
    float pasos, xIncrementos, yIncrementos, x = x1, y = y1;
    pasos = fabs(dx) > fabs(dy) ? fabs(dx) : fabs(dy);
    xIncrementos = dx / pasos;
    yIncrementos = dy / pasos;
    for (float i = 0; i < pasos; i++) {
        Circulo(x,y,tmanio, pix, 'd');
        x += xIncrementos;
        y += yIncrementos;
    }
    glEnd();
}
void dibujarLineas(int x[],int y[],int grosor[], int tamano){
    for (int i = 0; i < tamano - 1; i++) {
        dda(x[i], y[i], x[i+1], y[i+1], grosor[i]);
    }
}
void dibujar_pelo(){
    int x[]={204,198,191,185,181,180,185,192,204,216,232,244,253,260,264};
    int y[]={401,401,406,414,423,438,451,463,473,481,482,483,486,490,501};
    int grosor[]={sp,sp,sp,sp,sp,g,g,g,sp,sp,sp,n,d,d};
    int tamano = sizeof(x) / sizeof(x[0]);
    dibujarLineas(x,y,grosor,tamano);
    int x2[]={260,257,258,261,265,272,280,290,300,310,320,334,342,350,364};
    int y2[]={490,482,475,467,458,450,443,437,433,430,430,432,437,443,464};
    int g2[]={n,g,g,g,sp,sp,sp,sp,sp,sp,sp,g,n,n,g};
    int tamano2 = sizeof(x2) / sizeof(x2[0]);
    dibujarLineas(x2,y2,g2,tamano2);
    int x3[]={192,195,200,208,215,224,231,233,226,217,210};
    int y3[]={428,436,443,450,453,453,450,456,457,458,457};
    int g3[]={n,g,g,g,sp,sp,sp,sp,sp,g};
    int t3= sizeof(x3) / sizeof(x3[0]);
    dibujarLineas(x3,y3,g3,t3);
    int x4[]={242,240,237,232,224,219,214,208,202,197,213};
    int y4[]={446,452,456,460,462,463,464,462,456,453,460};
    int g4[]={g,g,sp,sp,sp,sp,sp,g,g,g};
    int t4= sizeof(x4) / sizeof(x4[0]);
    dibujarLineas(x4,y4,g4,t4);
    //cosa como diamante
    int x5[]={212,213,215,218,225,230,235,241,250,260,270,277,273,268,260,254,248,242,235};
    int y5[]={470,469,448,440,430,424,418,414,411,410,410,413,418,421,424,425,426,428,432};
    int g5[]={d,g,g,g,g,sp,sp,sp,spg,spg,sp,g,g,g,g,g,g,n};
    int t5= sizeof(x5) / sizeof(x5[0]);
    dibujarLineas(x5,y5,g5,t5);
    int x6[]={277,288,298,303,311,318,314,309,300};
    int y6[]={413,407,402,390,389,388,395,398,402};
    int g6[]={g,n,g,spg,sp,sp,g,g};
    int t6= sizeof(x6) / sizeof(x6[0]);
    dibujarLineas(x6,y6,g6,t6);
    dda(298,402,317,388,n);
    int x7[]={314,317,321,326,330,336,345,350,360,370,380,397,404,410,414};
    int y7[]={388,374,366,357,350,342,334,330,324,322,322,321,320,316,313};
    int g7[]={g,n,n,n,n,n,g,g,sp,spg,spg,sp,sp,sp};
    int t7= sizeof(x7) / sizeof(x7[0]);
    dibujarLineas(x7,y7,g7,t7);
    replinea(379,312,377,319,379,312,385,318,sd,'d');
    dda(396,318,394,310,d);
    dda(394,310,390,305,d);
    dda(390,305,386,303,g);
    dda(386,303,375,297,sp);
    dibujarLineas(x5,y5,g5,t5);
    int x8[]={410,410,412,417,421,430,434,442,454,464,474,480,488,493,497};
    int y8[]={316,310,305,300,296,291,289,286,285,286,290,294,302,310,321};
    int g8[]={g,g,g,g,g,sp,sp,sp,sp,sp,g,g,n,n};
    int t8= sizeof(x8) / sizeof(x8[0]);
    dibujarLineas(x8,y8,g8,t8);
    dda(467,299,478,306,sp);
    dda(478,306,486,313,g);
    dda(486,313,491,324,n);
    dda(491,324,493,333,d);
    //linea mono arriba
    dda(460,313,472,319,g);
    dda(472,319,478,324,n);
    dda(478,324,483,333,n);
    dda(483,333,484,345,g);
    dda(484,345,482,350,n);
    //linea sola arriba
    dda(329,405,320,415,g);
    dda(320,415,310,418,n);
    dda(310,418,295,422,g);
    dda(295,422,291,425,n);
    int x9[]={461,460,456,450,440,430,420,410,400,392,390,392,383,394,390,383,378,372,370,370,373,379,385,394,400,404,406,405,410,421,428,434,442,459};
    int y9[]={355,352,349,344,338,336,336,340,347,358,370,378,380,384,380,380,383,388,394,400,408,414,418,417,413,408,399,392,400,407,411,410,408,403};
    int g9[]={d,d,n,g,sp,sp,sp,sp,g,n,n,d,d,d,g,sp,sp,sp,sp,g,g,g,g,g,n,n,d,d,g,sp,sp,sp,sp};
    int t9= sizeof(x9) / sizeof(x9[0]);
    dibujarLineas(x9,y9,g9,t9);
    ddac(367,349,361,351,2,g);
    dda(361,351,343,359,g);
    dda(343,359,318,388,n);
    dda(319,383,341,384,g);
    replinea(367,380,360,372,367,380,360,380,sd,'d');
    int x10[]={360,356,347,343,340,338,340,346,356,347};
    int y10[]={377,376,380,384,390,398,405,416,424,441};
    int g10[]={sp,sp,sp,sp,g,n,n,n,sp};
    int t10= sizeof(x10) / sizeof(x10[0]);
    dibujarLineas(x10,y10,g10,t10);
    dda(336,433,344,417,sp);
    int x11[]={379,373,370,371,374,381,387,391,395,399,406,420,430,440,440,440,429,439,477};
    int y11[]={418,420,423,428,433,440,448,456,465,473,480,480,478,478,470,453,437,444,445};
    int g11[]={sp,sp,sp,n,d,n,g,g,d,d,n,g,g,n,n,d,g,spg};
    int t11= sizeof(x11) / sizeof(x11[0]);
    dibujarLineas(x11,y11,g11,t11);
    dda(336,433,344,417,sp);
    int x12[]={391,390,387,383,378,372,369,366,363,361,361,361,361,360,357};
    int y12[]={461,468,476,485,495,505,511,517,523,529,546,553,565,605,616};
    int g12[]={d,d,d,n,n,n,d,d,d,d,d,d,d,d,d};
    int t12= sizeof(x12) / sizeof(x12[0]);
    dibujarLineas(x12,y12,g12,t12);
    //puntita de la delgada
    ddac(360,605,357,616,2,sd);
    dda(387,458,377,459,d);
    dda(377,459,382,452,d);
    dda(382,452,381,445,d);
    int x13[]={385,361,354,351,345,335,324,312,301,289,280,275,276,283,292,305,310,312};
    int y13[]={454,468,475,465,458,452,448,448,453,460,471,484,501,513,522,522,516,506};
    int g13[]={n,g,g,g,g,g,g,g,g,g,sp,sp,g,sp,sp,sp,sp};
    int t13= sizeof(x13) / sizeof(x13[0]);
    dibujarLineas(x13,y13,g13,t13);
    ddac(312,506,304,497,2,n);
    dda(354,475,351,490,n);
    dda(351,490,345,505,n);
    dda(345,505,337,521,g);
    ddac(337,521,331,531,2,n);
    ddac(331,531,323,538,2,n);
}