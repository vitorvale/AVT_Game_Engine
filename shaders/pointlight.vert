#version 330

uniform mat4 m_pvm;
uniform mat4 m_viewModel;
uniform mat4 m_Model; 
uniform mat3 m_normal;

uniform int texMode;
uniform vec4 l_pos; // FIXME not here

uniform bool useNormalMap;

in vec4 color; // get color from material??
in vec4 position;
in vec4 normal;    //por causa do gerador de geometria
in vec4 texCoord;
in vec4 tangent;

out Data {
	vec3 normal;
	vec3 eye;
	vec3 lightDir;

	vec4 position;

	vec2 tex_coord;
	vec3 skyboxTexCoord;
} DataOut;

void main () {
	vec3 n, t, b;
	vec3 lightDir, eyeDir;
	vec3 aux;
	vec4 pos = m_viewModel * position;

	n = normalize(m_normal * normal.xyz);
	eyeDir = vec3(-pos);

	DataOut.skyboxTexCoord = vec3(m_Model * position);	//Transformação de modelação do cubo unitário 
	DataOut.skyboxTexCoord.x = - DataOut.skyboxTexCoord.x; //Texturas mapeadas no interior logo negar a coordenada x

	DataOut.tex_coord = texCoord.st;
	lightDir = vec3(l_pos - pos);

	if(texMode == 6 || useNormalMap)  {  //convert eye and light vectors to tangent space

		//Calculate components of TBN basis in eye space
		t = normalize(m_normal * tangent.xyz);  
		b = tangent.w * cross(n, t);

		aux.x = dot(lightDir, t);
		aux.y = dot(lightDir, b);
		aux.z = dot(lightDir, n);
		lightDir = normalize(aux);

		aux.x = dot(eyeDir, t);
		aux.y = dot(eyeDir, b);
		aux.z = dot(eyeDir, n);
		eyeDir = normalize(aux);
	}

	DataOut.normal = n;
	DataOut.lightDir = lightDir;
	DataOut.eye = eyeDir;
	DataOut.position = m_viewModel *  position;
	gl_Position = m_pvm * position;	
}