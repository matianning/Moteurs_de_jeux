#ifndef TRANSFORM_H
#define TRANSFORM_H

#include <QMatrix4x4>


class Transform {

public :
    Transform()=default;
    QMatrix4x4 m;
    QVector4D apply(QVector4D p);


};



/*
class Transform
{
public:

    Transform(
        const glm::vec3 & position = glm::vec3(0.0f),
        const glm::vec3 & rotation = glm::vec3(0.0f),
        const glm::vec3 & scale = glm::vec3(1.0f)) :
        m_position(position),
        m_rotation(rotation),
        m_scale(scale)
    {


    inline glm::mat4 getModel() //const
    {
        glm::mat4 posMatrix = glm::translate(m_position);
        glm::mat4 rotXMatrix = glm::rotate(m_rotation.x, glm::vec3(1.0f, 0.0f, 0.0f));
        glm::mat4 rotYMatrix = glm::rotate(m_rotation.y, glm::vec3(0.0f, 1.0f, 0.0f));
        glm::mat4 rotZMatrix = glm::rotate(m_rotation.z, glm::vec3(0.0f, 0.0f, 1.0f));
        glm::mat4 scaleMatrix = glm::scale(m_scale);

        // Want to write these matrices out in the opposite order
        // in which you want them to be calculated
        // ie the following will evaluate X * Y * Z
        glm::mat4 rotMatrix = rotZMatrix * rotYMatrix * rotXMatrix;
        m_normalMatrix = rotMatrix;

        return posMatrix * rotMatrix * scaleMatrix;
    }

    // Getters
    inline glm::vec3 & getPosition() { return m_position; }
    inline glm::vec3 & getRotation() { return m_rotation; }
    inline glm::vec3 & getScale() { return m_scale; }
    inline glm::mat4 & getNormalMatrix() { return m_normalMatrix; }

    // Setters
    inline void setPosition(const glm::vec3 & pos) { m_position = pos; }
    inline void setRotation(const glm::vec3 & rot) { m_rotation = rot; }
    inline void setScale(const glm::vec3 & scale) { m_scale = scale; }

private:
    glm::vec3 m_position, m_rotation, m_scale;
    glm::mat4 m_normalMatrix;

};
*/





#endif // TRANSFORM_H
