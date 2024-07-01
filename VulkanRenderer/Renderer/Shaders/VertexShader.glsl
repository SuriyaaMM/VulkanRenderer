
#version 450

struct Vertex
{
	vec3 Position;
	vec3 Colour;
};


layout(location = 0) in vec3 VSInPosition;
layout(location = 1) in vec3 VSInColour;

layout(location = 0) out vec3 PSOutColour;

void main()
{
	gl_Position = vec4(VSInPosition,1.0);
	PSOutColour = VSInColour;
}