#shader vertex
#version 330 core

layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 aColor;

uniform mat4 uVP;

out vec3 VertColor;

void main()
{
    gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    VertColor = aColor;
}
#shader fragment
#version 330 core

out vec4 FragColor;
in vec3 FragPos;
in vec3 Normal;
in vec3 VertColor;

uniform vec4 objectColor;

void main()
{
    vec3 result = objectColor.xyz * VertColor.xyz;
    FragColor = vec4(result, objectColor.w);
}
