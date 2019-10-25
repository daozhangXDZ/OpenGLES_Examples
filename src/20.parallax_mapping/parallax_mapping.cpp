﻿/*
#include <common.h>
using namespace es;

class Example final : public ExampleBase
{
public:
	Model* planeModel;

	Example()
	{
		title = "parallax mapping";
		settings.vsync = false;
		defaultClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		modelsDirectory = getResourcesPath(ResourceType::Model);
		shadersDirectory = getResourcesPath(ResourceType::Shader) + "/20.parallax_mapping/";
	}
	~Example()
	{
		delete(planeModel);
	}
public:
	virtual void prepare() override
	{
		ExampleBase::prepare();

		// setup camera
		camera->movementSpeed = 3.0f;
		camera->rotationSpeed = 0.5f;
		camera->setPosition(glm::vec3(0.0f, 2.0f, 2.0f));
		camera->setRotation(glm::vec3(45.0f, 0.0f, 0.0f));

		// enable depth test
		glEnable(GL_DEPTH_TEST);

		std::unordered_map<Material::ShaderType, std::string> shaderPaths =
		{
			{ Material::ShaderType::VERTEX, shadersDirectory + "parallax.vert" },
			{ Material::ShaderType::FRAGMENT, shadersDirectory + "parallax.frag" }
		};

		std::vector<std::pair<Texture::Type, std::string>> texturePaths =
		{
			std::make_pair(Texture::Type::DIFFUSE, modelsDirectory + "/rocks_plane/rocks_color_bc3_unorm.png"),
			std::make_pair(Texture::Type::NORMAL, modelsDirectory + "/rocks_plane/rocks_normal_height_rgba.png")
		};

		std::shared_ptr<Material> material = std::make_shared<Material>(shaderPaths, texturePaths);

		planeModel = Model::createWithFile(modelsDirectory + "/rocks_plane/rocks_plane.obj", material);
		planeModel->setRotation(glm::vec3(-90.0f, 0.0f, 0.0f));
		planeModel->setScale(glm::vec3(0.2f, 0.2f, 0.2f));

		planeModel->setFloat("numLayers", 48.0f);
		planeModel->setFloat("heightScale", 0.1f);
		planeModel->setFloat("parallaxBias", -0.02f);
	}

	virtual void render(float deltaTime) override
	{
		glfwGetFramebufferSize(window, &width, &height);
		glViewport(0, 0, width, height);
		glClearColor(defaultClearColor.r, defaultClearColor.g, defaultClearColor.b, defaultClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		planeModel->setVector3("lightPos", glm::vec3(sin(glm::radians(timePassed * 360.0f)) * 1.5f, 5.0f, cos(glm::radians(timePassed * 360.0f)) * 1.5f));
		planeModel->setVector3("viewPos", camera->getPosition());
		planeModel->render(deltaTime);
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
*/

#include <examplebase.h>
#include <model.h>
#include <material.h>
using namespace es;

class Example final : public ExampleBase
{
public:
	std::shared_ptr<Model> planeModel;

	Example()
	{
		title = "parallax mapping";
		settings.vsync = true;
		settings.validation = true;
		defaultClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);

		modelsDirectory = getResourcesPath(ResourceType::Model);
		shadersDirectory = getResourcesPath(ResourceType::Shader) + "/20.parallax_mapping/";
	}
	~Example()
	{

	}
public:
	virtual void prepare() override
	{
		ExampleBase::prepare();

		// enable depth test
		glEnable(GL_DEPTH_TEST);

		mMainCamera->setPosition(glm::vec3(0.0f, 15.0f, 15.0f));
		mMainCamera->setRotation(glm::vec3(45.0f, 0.0f, 0.0f));

		std::shared_ptr<Material> mat = Material::createFromFiles("mat",
			{
				shadersDirectory + "parallax.vert",
				shadersDirectory + "parallax.frag"
			},
			{
				{ "diffuseMap_0", modelsDirectory + "/rocks_plane/rocks_color_bc3_unorm.png" },
				{ "normalMap_0", modelsDirectory + "/rocks_plane/rocks_normal_height_rgba.png" }
			}
		);

		planeModel = Model::createFromFile("rocks_plane",
			modelsDirectory + "/rocks_plane/rocks_plane.obj",
			{

			},
			false
		);

		planeModel->setMaterial(mat);
		planeModel->setRotation(glm::vec3(90.0f, 0.0f, 0.0f));

		planeModel->setUniform("numLayers", 48.0f);
		planeModel->setUniform("heightScale", 0.1f);
		planeModel->setUniform("parallaxBias", -0.02f);
	}

	virtual void render(float deltaTime) override
	{
		SDL_GetWindowSize(window, &destWidth, &destHeight);
		glViewport(0, 0, destWidth, destHeight);
		glClearColor(defaultClearColor.r, defaultClearColor.g, defaultClearColor.b, defaultClearColor.a);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

		planeModel->setUniform("lightPos", glm::vec3(sin(glm::radians(timePassed * 360.0f)) * 1.5f, 8.0f, cos(glm::radians(timePassed * 360.0f)) * 1.5f));
		planeModel->setUniform("viewPos", mMainCamera->getPosition());
		planeModel->render();
	}
};

Example* example;
int APIENTRY WinMain(HINSTANCE hInstance, HINSTANCE, LPSTR, int)
{
	example = new Example();
	example->setupValidation();
	if (!example->setupSDL() ||
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