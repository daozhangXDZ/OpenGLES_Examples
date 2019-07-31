﻿
#include <common.h>
using namespace es;

class Example final : public ExampleBase
{
public:
	Mesh* quad;
	Material* material;
	Example()
	{
		title = "quad with texture";
		defaultClearColor = glm::vec4(0.2f, 0.2f, 0.4f, 1.0f);

		shadersDirectory = getResourcesPath(ResourceType::Shader) + "/07.quad_with_texture/";
		texturesDirectory = getResourcesPath(ResourceType::Texture) + "/07.quad_with_texture/";
	}
	~Example()
	{
		delete(quad);
	}
public:
	virtual void prepare() override
	{
		std::vector<GLfloat> vertexAttrs = {
			// positions          // texture coordinates
			0.5f,  0.5f, 0.0f,    1.0f, 1.0f,
			0.5f, -0.5f, 0.0f,    1.0f, 0.0f,
		   -0.5f, -0.5f, 0.0f,    0.0f, 0.0f,
		   -0.5f,  0.5f, 0.0f,    0.0f, 1.0f
		};

		std::vector<GLuint> indices = {
			0, 1, 3,
			1, 2, 3
		};

		std::vector<Vertex> vertices = {};
		for (uint32_t i = 0; i < static_cast<uint32_t>(vertexAttrs.size() / 5); i++)
		{
			Vertex vertex;
			vertex.Position = glm::vec3(vertexAttrs[i * 5], vertexAttrs[i * 5 + 1], vertexAttrs[i * 5 + 2]);
			vertex.TexCoords = glm::vec2(vertexAttrs[i * 5 + 3], vertexAttrs[i * 5 + 4]);
			vertices.push_back(vertex);
		}

		std::unordered_map<Material::ShaderType, std::string> shaderPaths =
		{
			{ Material::ShaderType::Vertex, shadersDirectory + "quad.vert" },
			{ Material::ShaderType::Fragment, shadersDirectory + "quad.frag" }
		};

		std::vector<std::pair<Texture::Type, std::string>> texturePaths = 
		{
			std::make_pair(Texture::Type::Diffuse, texturesDirectory + "timg.png")
		};

		// create quad material
		material = Material::createWithFile(shaderPaths, texturePaths);

		quad = Mesh::createWithData(vertices, indices, Mesh::DrawType::Elements, material);
	}
	virtual void render() override
	{
		// render quad mesh
		quad->Draw();
	}
};

Example* example;
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	example = new Example();
	example->setupValidation();
	if (!example->setupGLFW() ||
		!example->loadGLESFunctions() ||
		!example->setupImGui())
	{
		return 0;
	}
	example->prepare();
	example->renderLoop();
	delete(example);
	return 0;
}