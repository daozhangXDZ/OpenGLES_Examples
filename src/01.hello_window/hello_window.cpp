#include <examplebase.h>
#pragma comment(lib, "SDL2.lib")                  
#pragma comment(lib, "SDL2main.lib")

using namespace es;

class Example : public ExampleBase
{
public:
	Example()  
	{
		title = "hello window";
		settings.vsync = true;
	}

	~Example()
	{

	}
public:
	virtual void prepare() override
	{
		ExampleBase::prepare();
	}

	virtual void render(float deltaTime) override
	{

	}

	virtual void windowResized() override
	{
		ExampleBase::windowResized();
	}
};

Example* example;
int main(int argc, char* argv[])
{
	//SDL_Init(SDL_INIT_EVERYTHING);
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
	return EXIT_SUCCESS;
}

//int APIENTRY SDL_main(int args, char **argc)
//{
//	example = new Example();
//	example->setupValidation();
//	if (!example->setupSDL() ||
//		!example->loadGLESFunctions() ||
//		!example->setupImGui())
//	{
//		return 0;
//	}
//	example->prepare();
//	example->renderLoop();
//	delete(example);
//	return EXIT_SUCCESS;
//}
