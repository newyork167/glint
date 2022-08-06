#version 330 core

out vec4 FragColor;

in vec3 vertexColor;
uniform vec3 color;
uniform float iTime;

void main()
{		
	FragColor = vec4(color * vertexColor * sin(iTime), 1.0);
}