#include "meshtri.h"
#include "matrices.h"


MeshTri::MeshTri()
{
}



void MeshTri::gl_init()
{
	m_shader_flat = new ShaderProgramFlat();
	m_shader_phong = new ShaderProgramPhong();

	//VBO
	glGenBuffers(1, &m_vbo);
	glGenBuffers(1, &m_vbo2);

	//VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_flat->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_flat->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	//VAO2
	glGenVertexArrays(1, &m_vao2);
	glBindVertexArray(m_vao2);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_phong->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_phong->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glEnableVertexAttribArray(m_shader_phong->idOfNormalAttribute);
	glVertexAttribPointer(m_shader_phong->idOfNormalAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);

	//EBO indices
	glGenBuffers(1, &m_ebo);
}

void MeshTri::gl_update()
{
	//VBO
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_points.size() * sizeof(GLfloat), &(m_points[0][0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo2);
	glBufferData(GL_ARRAY_BUFFER, 3 * m_normals.size() * sizeof(GLfloat), &(m_normals[0][0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	//EBO indices
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER,m_indices.size() * sizeof(int), &(m_indices[0]), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}



void MeshTri::set_matrices(const Mat4& view, const Mat4& projection)
{
	viewMatrix = view;
	projectionMatrix = projection;
}


void MeshTri::draw(const Vec3& color)
{
	m_shader_flat->startUseProgram();

	m_shader_flat->sendViewMatrix(viewMatrix);
	m_shader_flat->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_flat->idOfColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
	glDrawElements(GL_TRIANGLES, m_indices.size(),GL_UNSIGNED_INT,0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
	glBindVertexArray(0);

	m_shader_flat->stopUseProgram();
}


void MeshTri::draw_smooth(const Vec3& color)
{
	m_shader_phong->startUseProgram();

	m_shader_phong->sendViewMatrix(viewMatrix);
    m_shader_phong->sendProjectionMatrix(projectionMatrix);

	glUniform3fv(m_shader_phong->idOfColorUniform, 1, glm::value_ptr(color));

    glBindVertexArray(m_vao2);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,m_ebo);
    glDrawElements(GL_TRIANGLES, m_indices.size(),GL_UNSIGNED_INT,0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
    glBindVertexArray(0);

	m_shader_phong->stopUseProgram();
}


void MeshTri::clear()
{
    m_indices.clear();
    m_points.clear();
    m_normals.clear();

}


int MeshTri::add_vertex(const Vec3& P)
{
    m_points.push_back(P);
    return m_points.size()-1;
}

int MeshTri::add_normal(const Vec3& N)
{
    m_normals.push_back(N);
    return m_normals.size()-1;
}

void MeshTri::add_tri(int i1, int i2, int i3)
{
    m_indices.push_back(i1);
    m_indices.push_back(i2);
    m_indices.push_back(i3);
}

void MeshTri::add_quad(int i1, int i2, int i3, int i4)
{
	// decoupe le quad en 2 triangles: attention a l'ordre
    add_tri(i1,i2,i3);

    add_tri(i3,i4,i1);
}


void MeshTri::create_pyramide()
{
	clear();
    int p1 = this->add_vertex(Vec3(0,0,0));
    int p2 = this->add_vertex(Vec3(2,0,0));
    int p3 = this->add_vertex(Vec3(2,2,0));
    int p4 = this->add_vertex(Vec3(0,2,0));
    int p5 = this->add_vertex(Vec3(1,1,1));
    this->add_quad(p1,p2,p3,p4);
    this->add_tri(p1,p2,p5);
    this->add_tri(p1,p4,p5);
    this->add_tri(p3,p4,p5);
    this->add_tri(p3,p2,p5);

	gl_update();
}

void MeshTri::create_anneau()
{
	clear();
    float pas = 1;
    float taille=1.5;
    for(int i=0;i<360-pas;i+=pas){
        this->add_vertex(Vec3(cos(i*2*M_PI / 360),sin(i*2*M_PI / 360),0));
        this->add_vertex(Vec3(taille*cos(i*2*M_PI / 360),taille*sin(i*2*M_PI / 360),0));
    }
    int length = m_points.size();
    for(int i=0;i<length;i+=2){
        this->add_quad(i,i+1,i+3,i+2);
    }
    this->add_quad(length-2,length-1,1,0);

	gl_update();
}

void MeshTri::create_spirale()
{
	clear();
    float tr=0.2;
    float pas = 1;
    float taille=1.5;
    float dc=1;
    for(int i=0;i<1000-pas;i+=pas,dc+=pas*0.01){
        this->add_vertex(Vec3(tr*dc*cos(i*2*M_PI / 360),tr*dc*sin(i*2*M_PI / 360),tr*(dc-1)));
        this->add_vertex(Vec3(tr*(dc+taille)*cos(i*2*M_PI / 360),tr*(dc+taille)*sin(i*2*M_PI / 360),tr*(dc-1)));
    }
    int length = m_points.size();
    for(int i=0;i<length-2;i+=2){
        this->add_quad(i,i+1,i+3,i+2);
    }
	gl_update();
}



void MeshTri::compute_normals()
{
	// ALGO:
	// init des normale a 0,0,0
	// Pour tous les triangles
	//   calcul de la normale -> N
	//   ajout de N au 3 normales des 3 sommets du triangles
	// Fin_pour
	// Pour toutes les normales
	//   normalisation
	// Fin_pour
    for(int i=0;i<(int)m_points.size();i++){
        add_normal(Vec3(0,0,0));
    }
    Vec3 NF;
    std::cout << "test1" << std::endl;
    for(int i=0;i<(int)m_indices.size()-1;i+=3){
        std::cout << "------------------------------------" << std::endl;
        NF=cross(m_points[m_indices[i]]-m_points[m_indices[i+1]],m_points[m_indices[i+2]]-m_points[m_indices[i+1]]);
        std::cout << m_points[m_indices[i]]-m_points[m_indices[i+1]] << std::endl;
        std::cout << m_points[m_indices[i+2]]-m_points[m_indices[i+1]] << std::endl;
        std::cout << NF << std::endl;
        NF=normalize(NF);
        std::cout << NF << std::endl;
        std::cout << "------------------------------------" << std::endl;
        m_normals[m_indices[i]]=m_normals[m_indices[i]]+NF;
        m_normals[m_indices[i+1]]=m_normals[m_indices[i+1]]+NF;
        m_normals[m_indices[i+2]]=m_normals[m_indices[i+2]]+NF;
    }
    std::cout << "test2" << std::endl;
    for(int i=0;i<(int)m_points.size();i++){
        m_normals[i]=normalize(m_normals[i]);
        std::cout << m_normals[i] << std::endl;
    }
    std::cout << "test3" << std::endl;
    gl_update();

}

List_triangle MeshTri::get_triangle(){
    List_triangle result;
    for(int i=0;i<(int)m_indices.size()-1;i+=3){
        result.push_back(Triangle(m_points[m_indices[i]],m_points[m_indices[i+1]],m_points[m_indices[i+2]]));
    }
    return result;
}
