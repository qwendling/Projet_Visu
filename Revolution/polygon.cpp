#include "polygon.h"
#include <cstdint>

PolygonEditor::PolygonEditor()
{

}


void PolygonEditor::gl_init()
{
	// SHADER
	m_shader_color = new ShaderProgramColor();

	//VBO
	glGenBuffers(1, &m_vbo);

	// genere 1 VAO
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glEnableVertexAttribArray(m_shader_color->idOfVertexAttribute);
	glVertexAttribPointer(m_shader_color->idOfVertexAttribute, 3, GL_FLOAT, GL_FALSE, 0, 0);
	glBindVertexArray(0);
}


void PolygonEditor::draw(const Vec3& color)
{
	Mat4 id;

	glBindBuffer(GL_ARRAY_BUFFER, m_vbo);
	glBufferData(GL_ARRAY_BUFFER, m_points.size()*sizeof(Vec3), m_points.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	m_shader_color->startUseProgram();
	m_shader_color->sendViewMatrix(id);
	m_shader_color->sendProjectionMatrix(id);

	glUniform3fv(m_shader_color->idOfColorUniform, 1, glm::value_ptr(color));

	glBindVertexArray(m_vao);
	glPointSize(4.0);
	glDrawArrays(GL_POINTS, 0, m_points.size());
	glDrawArrays(GL_LINE_STRIP, 0, m_points.size());
	glBindVertexArray(0);
	m_shader_color->stopUseProgram();
}


bool intersecte(const Vec3& A, const Vec3& B, const Vec3& C, const Vec3& D)
{
    bool sign_pv= std::signbit(cross(B-A,C-A).z * cross(B-A,D-A).z);
    bool checkpv = std::signbit(cross(D-C,A-C).z * cross(D-C,B-C).z);
    return sign_pv && checkpv;
}

void PolygonEditor::fermetureHaute(){
    Vec3 tmp = Vec3(0,m_points[0].y,0);
    for(int i=1;i<(int)m_points.size()-2;i++){
        if(intersecte(m_points[i],m_points[i+1],tmp,m_points[0])){
            return;
        }
    }
    std::vector<Vec3> tmp_points;
    tmp_points.push_back(tmp);
    for(int i=0;i<(int)m_points.size();i++)
        tmp_points.push_back(m_points[i]);
    m_points.swap(tmp_points);
}


void PolygonEditor::fermeture(){
    fermetureHaute();
    add_vertex(0.,m_points[m_points.size()-1].y);
}


void PolygonEditor::add_vertex(float x, float y)
{
    for(int i=0;i<(int)m_points.size()-2;i++){
        if(intersecte(m_points[i],m_points[i+1],m_points[m_points.size()-1],Vec3(x,y,0))){
            return;
        }
    }
    m_points.push_back(Vec3(x,y,0));
}

void PolygonEditor::remove_last()
{
    m_points.pop_back();
}

void PolygonEditor::clear()
{
    m_points.clear();
}

void PolygonEditor::lisse()
{
    std::vector<Vec3> tmp_points;
    tmp_points.push_back(m_points[0]);
    for(int i=1;i<(int)m_points.size()-1;i++){
        Vec3 tmp1=(m_points[i]-m_points[i-1]);
        tmp1.x = m_points[i-1].x + 0.75*tmp1.x;
        tmp1.y = m_points[i-1].y + 0.75*tmp1.y;
        Vec3 tmp2=(m_points[i]-m_points[i+1]);
        tmp2.x = m_points[i+1].x + 0.75*tmp2.x;
        tmp2.y = m_points[i+1].y + 0.75*tmp2.y;
        tmp_points.push_back(tmp1);
        tmp_points.push_back(tmp2);
    }
    tmp_points.push_back(m_points[m_points.size()-1]);
    m_points.swap(tmp_points);
}

