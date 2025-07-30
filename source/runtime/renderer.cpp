#include "ryn/graphics/renderer.h"

ryn::Renderer::Renderer(SharedPtr<Window> window) 
	: window(window) {}

//glfwMakeContextCurrent(window.get());
//if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
//	throw std::runtime_error("Failed to initialize GLAD (OpenGL)");
//std::cout << "OpenGL Version: " << glGetString(GL_VERSION) << std::endl;