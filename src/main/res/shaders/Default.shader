#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;

uniform mat4 uMVP;

void main()
{
    gl_Position = uMVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
}
#shader fragment
#version 330 core

out vec4 FragColor;

uniform vec4 uColor;

void main()
{
    FragColor = uColor;
}