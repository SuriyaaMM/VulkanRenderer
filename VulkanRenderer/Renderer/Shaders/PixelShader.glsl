
#version 450

layout(location = 0) in vec3 VSOutColour;
layout(location = 0) out vec4 OutColour;


void main()
{
	OutColour = vec4(VSOutColour, 1.0);
}