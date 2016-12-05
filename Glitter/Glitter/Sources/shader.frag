#version 330 core  

in vec3 outPosition;
out vec4 color;

void main()
{
    color = vec4(outPosition, 1.0f);
}
