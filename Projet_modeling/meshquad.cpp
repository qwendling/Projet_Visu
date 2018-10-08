#include "meshquad.h"
#include "matrices.h"

MeshQuad::MeshQuad():
	m_nb_ind_edges(0)
{

}


void MeshQuad::gl_init()
{
	m_shader_flat = new ShaderProgramFlat();
	m_shader_color = new ShaderProgramColor();

	//VBO
	glGenBuffers(1, &m_vbo);

	//VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_flat->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_flat->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	glGenVertexArrays(1, &m_vao2);
	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_color->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_color->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);


	//EBO indices
	glGenBuffers(1, &m_ebo);
	glGenBuffers(1, &m_ebo2);
}

void MeshQuad::gl_update()
{
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_points.size() * sizeof(GLfloat), &(m_points[0][0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);


	std::vector<int> tri_indices;
	convert_quads_to_tris(m_quad_indices,tri_indices);

	//EBO indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,tri_indices.size() * sizeof(int), &(tri_indices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);


	std::vector<int> edge_indices;
	convert_quads_to_edges(m_quad_indices,edge_indices);
	m_nb_ind_edges = edge_indices.size();

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo2);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_nb_ind_edges * sizeof(int), &(edge_indices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void MeshQuad::set_matrices(const Mat4& view, const Mat4& projection)
{
	viewMatrix = view;
	projectionMatrix = projection;
}

void MeshQuad::draw(const Vec3& color)
{

	glEnable(GL_POLYGON_OFFSET_FILL);
	glPolygonOffset(1.0f, 1.0f);

	m_shader_flat->startUseProgram();
	m_shader_flat->sendViewMatrix(viewMatrix);
	m_shader_flat->sendProjectionMatrix(projectionMatrix);
	glUniform3fv(m_shader_flat->idOfColorUniform, 1, glm::value_ptr(color));
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glDrawElements(GL_TRIANGLES, 3*m_quad_indices.size()/2,GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	m_shader_flat->stopUseProgram();

	glDisable(GL_POLYGON_OFFSET_FILL);

	m_shader_color->startUseProgram();
	m_shader_color->sendViewMatrix(viewMatrix);
	m_shader_color->sendProjectionMatrix(projectionMatrix);
	glUniform3f(m_shader_color->idOfColorUniform, 0.0f,0.0f,0.0f);
	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo2);
	glDrawElements(GL_LINES, m_nb_ind_edges,GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);
	m_shader_color->stopUseProgram();
}

void MeshQuad::clear()
{
	m_points.clear();
	m_quad_indices.clear();
}

int MeshQuad::add_vertex(const Vec3& P)
{
    m_points.push_back(P);
    return m_points.size()-1;
}


void MeshQuad::add_quad(int i1, int i2, int i3, int i4)
{
    m_quad_indices.push_back(i1);
    m_quad_indices.push_back(i2);
    m_quad_indices.push_back(i3);
    m_quad_indices.push_back(i4);
}

void MeshQuad::convert_quads_to_tris(const std::vector<int>& quads, std::vector<int>& tris)
{
	tris.clear();
	tris.reserve(3*quads.size()/2); // 1 quad = 4 indices -> 2 tris = 6 indices d'ou ce calcul (attention division entiere)

	// Pour chaque quad on genere 2 triangles
	// Attention a respecter l'orientation des triangles

    for(int i=0;i<(int)quads.size();i+=4){
        //premier triangle
        tris.push_back(quads[i]);
        tris.push_back(quads[i+1]);
        tris.push_back(quads[i+2]);
        //deuxieme triangle
        tris.push_back(quads[i+2]);
        tris.push_back(quads[i+3]);
        tris.push_back(quads[i]);
    }
}

void MeshQuad::convert_quads_to_edges(const std::vector<int>& quads, std::vector<int>& edges)
{
	edges.clear();
	edges.reserve(quads.size()); // ( *2 /2 !)

	// Pour chaque quad on genere 4 aretes, 1 arete = 2 indices.
	// Mais chaque arete est commune a 2 quads voisins !
	// Comment n'avoir qu'une seule fois chaque arete ?

    //test si l'arete i1 i2 n'est pas deja dans edges
    auto edges_exist = [&] (int i1, int i2) -> bool {
        for(int i=0;i<(int)edges.size();i+=2){
            if(edges[i]==i2 && edges[i+1]==i1){
                return true;
            }
        }
        return false;
    };

    //Si l'arete i1 i2 n'est pas deja edges la rajoute
    auto add_if_doesnt_exist = [&] (int i1, int i2) -> void{
        if(!edges_exist(i1,i2)){
            edges.push_back(i1);
            edges.push_back(i2);
        }
    };

    for(int i=0;i<(int)quads.size();i+=4){
        add_if_doesnt_exist(quads[i],quads[i+1]);
        add_if_doesnt_exist(quads[i+1],quads[i+2]);
        add_if_doesnt_exist(quads[i+2],quads[i+3]);
        add_if_doesnt_exist(quads[i+3],quads[i]);
    }

}


void MeshQuad::create_cube()
{
	clear();
	// ajouter 8 sommets (-1 +1)
    int s1 = add_vertex(Vec3(0,0,0));
    int s2 = add_vertex(Vec3(0.5,0,0));
    int s3 = add_vertex(Vec3(0.5,0.5,0));
    int s4 = add_vertex(Vec3(0,0.5,0));
    int s5 = add_vertex(Vec3(0,0,0.5));
    int s6 = add_vertex(Vec3(0.5,0,0.5));
    int s7 = add_vertex(Vec3(0.5,0.5,0.5));
    int s8 = add_vertex(Vec3(0,0.5,0.5));

	// ajouter 6 faces (sens trigo)
    add_quad(s1,s4,s3,s2);
    add_quad(s1,s2,s6,s5);
    add_quad(s2,s3,s7,s6);
    add_quad(s1,s5,s8,s4);
    add_quad(s4,s8,s7,s3);
    add_quad(s5,s6,s7,s8);


	gl_update();
}

Vec3 MeshQuad::normal_of_quad(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{
	// Attention a l'ordre des points !
	// le produit vectoriel n'est pas commutatif U ^ V = - V ^ U
	// ne pas oublier de normaliser le resultat.

    Vec3 N1 = cross(B-A,D-A);
    Vec3 N2 = cross(D-C,B-C);
    Vec3 NM = N1+N2;

    NM.x /= 2;
    NM.y /= 2;
    NM.z /= 2;



    return normalize(NM);
}

float MeshQuad::area_of_quad(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{
	// aire du quad - aire tri + aire tri

	// aire du tri = 1/2 aire parallelogramme

	// aire parallelogramme: cf produit vectoriel

    float A1 = length(cross(A-B,C-B))/2;
    float A2 = length(cross(A-D,C-D))/2;

    return A1 + A2;
}


bool MeshQuad::is_points_in_quad(const Vec3& P, const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{
	// On sait que P est dans le plan du quad.

	// P est-il au dessus des 4 plans contenant chacun la normale au quad et une arete AB/BC/CD/DA ?
	// si oui il est dans le quad

    //On calcul la normale moyenne au quad
    Vec3 NQ = normal_of_quad(A,B,C,D);

    //On test si le point est au dessus des 4 plan

    Vec3 NP1 = cross(NQ,B-A);
    Vec3 NP2 = cross(NQ,C-B);
    Vec3 NP3 = cross(NQ,D-C);
    Vec3 NP4 = cross(NQ,A-D);

    bool T1 = dot(P,NP1) > dot(A,NP1);
    bool T2 = dot(P,NP2) > dot(B,NP2);
    bool T3 = dot(P,NP3) > dot(C,NP3);
    bool T4 = dot(P,NP4) > dot(D,NP4);


    return T1 && T2 && T3 && T4;
}

bool MeshQuad::intersect_ray_quad(const Vec3& P, const Vec3& Dir, int q, Vec3& inter)
{
	// recuperation des indices de points
	// recuperation des points

	// calcul de l'equation du plan (N+d)

	// calcul de l'intersection rayon plan
	// I = P + alpha*Dir est dans le plan => calcul de alpha

	// alpha => calcul de I

	// I dans le quad ?

    int ip1 = m_quad_indices[4*q];
    int ip2 = m_quad_indices[4*q+1];
    int ip3 = m_quad_indices[4*q+2];
    int ip4 = m_quad_indices[4*q+3];

    Vec3 p1 = m_points[ip1];
    Vec3 p2 = m_points[ip2];
    Vec3 p3 = m_points[ip3];
    Vec3 p4 = m_points[ip4];

    Vec3 NQ = normal_of_quad(p1,p2,p3,p4);


    float d = -(NQ.x * p1.x + NQ.y * p1.y + NQ.z * p1.z);

    float alpha = ((-d)-(NQ.x*P.x + NQ.y*P.y + NQ.z*P.z))/(NQ.x*Dir.x + NQ.y*Dir.y + NQ.z*Dir.z);
    inter = Dir;
    inter.x *= alpha;
    inter.y *= alpha;
    inter.z *= alpha;

    inter += P;

    return is_points_in_quad(inter,p1,p2,p3,p4);
}


int MeshQuad::intersected_visible(const Vec3& P, const Vec3& Dir)
{
	// on parcours tous les quads
	// on teste si il y a intersection avec le rayon
	// on garde le plus proche (de P)

    int inter = -1;
    Vec3 tmp1;
    Vec3 tmp2;
    if(intersect_ray_quad(P,Dir,0,tmp2)){
        inter=0;
    }
    tmp1=tmp2;
    if(inter==-1)
        tmp1=Vec3(10000,10000,10000);

    for(int i=4,j=1;i<(int)m_quad_indices.size();i+=4,j++){
        if(intersect_ray_quad(P,Dir,j,tmp2) && length(P-tmp1) > length(P-tmp2)){

            inter = j;
            tmp1=tmp2;
        }
    }

	return inter;
}


Mat4 MeshQuad::local_frame(int q)
{
	// Repere locale = Matrice de transfo avec
	// les trois premieres colones: X,Y,Z locaux
	// la derniere colonne l'origine du repere

	// ici Z = N et X = AB
	// Origine le centre de la face
	// longueur des axes : [AB]/2

	// recuperation des indices de points
	// recuperation des points

	// calcul de Z:N puis de X:arete on en deduit Y

	// calcul du centre

	// calcul de la taille

	// calcul de la matrice

    int ip1 = m_quad_indices[4*q];
    int ip2 = m_quad_indices[4*q+1];
    int ip3 = m_quad_indices[4*q+2];
    int ip4 = m_quad_indices[4*q+3];

    Vec3 p1 = m_points[ip1];
    Vec3 p2 = m_points[ip2];
    Vec3 p3 = m_points[ip3];
    Vec3 p4 = m_points[ip4];

    Vec3 Z = normal_of_quad(p1,p2,p3,p4);
    Vec3 X = normalize(p2-p1);
    Vec3 Y = cross(Z,X);
    float taille = length(p2-p1)/2;
    Vec3 Origine = p1+p2+p3+p4;
    Origine.x /= 4;
    Origine.y /= 4;
    Origine.z /= 4;

    Mat4 res = Mat4(Vec4(X,0),Vec4(Y,0),Vec4(Z,0),Vec4(Origine,1));

    return res*scale(taille,taille,taille);
}

void MeshQuad::extrude_quad(int q)
{
	// recuperation des indices de points

	// recuperation des points

	// calcul de la normale

	// calcul de la hauteur

	// calcul et ajout des 4 nouveaux points

	// on remplace le quad initial par le quad du dessu

	// on ajoute les 4 quads des cotes

    int ip1 = m_quad_indices[4*q];
    int ip2 = m_quad_indices[4*q+1];
    int ip3 = m_quad_indices[4*q+2];
    int ip4 = m_quad_indices[4*q+3];

    Vec3 p1 = m_points[ip1];
    Vec3 p2 = m_points[ip2];
    Vec3 p3 = m_points[ip3];
    Vec3 p4 = m_points[ip4];

    Vec3 N = normal_of_quad(p1,p2,p3,p4);

    float A =sqrt(area_of_quad(p1,p2,p3,p4));

    Vec3 Hauteur = Vec3(scale(A,A,A)*Vec4(N,0));

    Vec3 n1 = p1+Hauteur;
    Vec3 n2 = p2+Hauteur;
    Vec3 n3 = p3+Hauteur;
    Vec3 n4 = p4+Hauteur;

    int in1 = add_vertex(n1);
    int in2 = add_vertex(n2);
    int in3 = add_vertex(n3);
    int in4 = add_vertex(n4);

    add_quad(ip1,ip2,ip3,ip4);

    add_quad(ip1,ip2,in2,in1);
    add_quad(ip2,ip3,in3,in2);
    add_quad(ip3,ip4,in4,in3);
    add_quad(ip4,ip1,in1,in4);

    m_quad_indices[4*q] = in1;
    m_quad_indices[4*q+1] = in2;
    m_quad_indices[4*q+2] = in3;
    m_quad_indices[4*q+3] = in4;

	gl_update();
}


void MeshQuad::decale_quad(int q, float d)
{
	// recuperation des indices de points

	// recuperation des (references de) points

	// calcul de la normale

	// modification des points

    int ip1 = m_quad_indices[4*q];
    int ip2 = m_quad_indices[4*q+1];
    int ip3 = m_quad_indices[4*q+2];
    int ip4 = m_quad_indices[4*q+3];

    Vec3* p1 = &m_points[ip1];
    Vec3* p2 = &m_points[ip2];
    Vec3* p3 = &m_points[ip3];
    Vec3* p4 = &m_points[ip4];

    Vec3 N = normal_of_quad(*p1,*p2,*p3,*p4);

    float A =area_of_quad(*p1,*p2,*p3,*p4);

    Vec3 Distance = Vec3(scale(A*d,A*d,A*d)*Vec4(N,0));

    *p1 += Distance;
    *p2 += Distance;
    *p3 += Distance;
    *p4 += Distance;

	gl_update();
}


void MeshQuad::shrink_quad(int q, float s)
{
	// recuperation des indices de points

	// recuperation des (references de) points

	// ici  pas besoin de passer par une matrice
	// calcul du centre

	 // modification des points

    int ip1 = m_quad_indices[4*q];
    int ip2 = m_quad_indices[4*q+1];
    int ip3 = m_quad_indices[4*q+2];
    int ip4 = m_quad_indices[4*q+3];

    Vec3* p1 = &m_points[ip1];
    Vec3* p2 = &m_points[ip2];
    Vec3* p3 = &m_points[ip3];
    Vec3* p4 = &m_points[ip4];

    Vec3 Origine = (*p1)+(*p2)+(*p3)+(*p4);
    Origine.x /= 4;
    Origine.y /= 4;
    Origine.z /= 4;


    *p1 = Origine+ Vec3(scale(s,s,s)*Vec4((*p1)-Origine,0));
    *p2 = Origine+ Vec3(scale(s,s,s)*Vec4((*p2)-Origine,0));
    *p3 = Origine+ Vec3(scale(s,s,s)*Vec4((*p3)-Origine,0));
    *p4 = Origine+ Vec3(scale(s,s,s)*Vec4((*p4)-Origine,0));

	gl_update();
}


void MeshQuad::tourne_quad(int q, float a)
{
	// recuperation des indices de points

	// recuperation des (references de) points

	// generation de la matrice de transfo:
	// tourne autour du Z de la local frame
	// indice utilisation de glm::inverse()

	// Application au 4 points du quad

    int ip1 = m_quad_indices[4*q];
    int ip2 = m_quad_indices[4*q+1];
    int ip3 = m_quad_indices[4*q+2];
    int ip4 = m_quad_indices[4*q+3];

    Vec3* p1 = &m_points[ip1];
    Vec3* p2 = &m_points[ip2];
    Vec3* p3 = &m_points[ip3];
    Vec3* p4 = &m_points[ip4];

    Mat4 LF = local_frame(q);

    Mat4 Transfo = LF * rotateZ(a) * inverse(LF);

    *p1 = Vec3(Transfo*Vec4(*p1,1));
    *p2 = Vec3(Transfo*Vec4(*p2,1));
    *p3 = Vec3(Transfo*Vec4(*p3,1));
    *p4 = Vec3(Transfo*Vec4(*p4,1));


	gl_update();
}

void MeshQuad::create_Spiral()
{

    create_cube();

    float coef_decal=0.15;
    float size_cube=1;
    for(int i=0;i<200;i++){
        extrude_quad(2);
        tourne_quad(this->nb_quads()-3,10);
        decale_quad(this->nb_quads()-3,coef_decal);
        tourne_quad(this->nb_quads()-1,-10);
        decale_quad(this->nb_quads()-1,-coef_decal);
        shrink_quad(2,0.995);
        coef_decal*=1.005;
        size_cube*=1.005;
    }


    gl_update();
}

void MeshQuad::create_Etoile(){
    create_cube();

    const int taille_etoile=10;
    auto Draw_branche = [&] (int q) -> void{
        for(int i=0;i<taille_etoile;i++){
            extrude_quad(q);
            shrink_quad(q,0.8);
            tourne_quad(q,10);
        }
    };

    Draw_branche(0);
    Draw_branche(1);
    Draw_branche(2);
    Draw_branche(3);
    Draw_branche(4);
    Draw_branche(5);

    gl_update();
}

void MeshQuad::create_Recursif(){
    create_cube();

    Recursif_Aux(4,4);

    gl_update();
}

void MeshQuad::Recursif_Aux(int q,int n){
    if(n<=0)
        return;
    extrude_quad(q);
    decale_quad(q,1.5);
    shrink_quad(q,0.6);
    extrude_quad(q);
    decale_quad(q,3);
    extrude_quad(q);

    int nbquads=nb_quads();
    Recursif_Aux(q,n-1);
    Recursif_Aux(nbquads-1,n-1);
    Recursif_Aux(nbquads-2,n-1);
    Recursif_Aux(nbquads-3,n-1);
    Recursif_Aux(nbquads-4,n-1);
}
