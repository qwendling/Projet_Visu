#include "source_facette.h"
#include <cstdlib>
#include <ctime>
#include <random>

Vec3 operator*(const Vec3& v,double x){
    return Vec3(v.x*x,v.y*x,v.z*x);
}

source_facette::source_facette(const Triangle& t)
{
    std::srand(std::time(nullptr));
    Vec3 u1 = t.s_2 - t.s_1;
    Vec3 u2 = t.s_3 - t.s_1;
    this->s_1 = t.s_1;
    this->s_2 = t.s_2;
    this->s_3 = t.s_3;
    double a1;
    double a2;
    for(int i =0;i<ECHANTILLON;i++){
        bool redo = true;
        while (redo) {
            a1 = rand()/(double)RAND_MAX;
            a2 = rand()/(double)RAND_MAX;
            if(a1+a2<1)
                redo = false;
        }
        this->liste_sources.push_back(t.s_1 + (u1*a1)+(u2*a2));
    }
}

void source_facette::draw(GLfloat r, GLfloat g, GLfloat b){
    glColor4f(r,g,b,1) ;
        #if 1
      glBegin(GL_TRIANGLES) ;
      glVertex3f(s_1.x,s_1.y,s_1.z) ;
      glVertex3f(s_2.x,s_2.y,s_2.z) ;
      glVertex3f(s_3.x,s_3.y,s_3.z) ;
      glVertex3f(s_1.x,s_1.y,s_1.z) ;
      glVertex3f(s_3.x,s_3.y,s_3.z) ;
      glVertex3f(s_2.x,s_2.y,s_2.z) ;
#else
    glBegin(GL_POINTS) ;
    for(auto& p:liste_sources){
        glVertex3f(p.x,p.y,p.z) ;
    }
#endif
      glEnd() ;
}


void source_facette::move_facette(const Vec3& v){
    Vec3 deplacement = v-get_cm();
    s_1 = s_1 + deplacement;
    s_2 = s_2 + deplacement;
    s_3 = s_3 + deplacement;
    for(auto& p:liste_sources){
        p +=deplacement;
    }
}
