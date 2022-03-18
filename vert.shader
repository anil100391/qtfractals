std::string vshader =

"layout(location = 0) in vec2 position;\n"
"uniform mat4 u_MVP;\n"
"void main()\n"
"{\n"
"    gl_Position = u_MVP * vec4(position, 0.0, 1.0);\n"
"}\n";
