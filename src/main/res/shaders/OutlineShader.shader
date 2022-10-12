#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uVP;

out vec3 FragPos;

void main()
{
    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    FragPos = aPos;
}
#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;

void main()
{
    FragColor = vec4(0.04, 0.28, 0.26, 1.0);
}
