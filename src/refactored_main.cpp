
#include <SDL.h>
#include "gl.h"
#include "camera.h"
#include "uniforms.h"
#include "shaders.h"
#include "object.h"
#include "triangle.h"
#include <iostream>
#include <vector>

// Setting up the scene
SceneCamera mainCamera = initializeCamera();
std::vector<Shape> sceneModels;
std::string celestialBody;

using namespace std;

// Function to handle the rendering process
void executeRender() {
    for (Shape& shape : sceneModels) {
        ShapeUniforms shapeUniforms = shape.uniforms;
        shapeUniforms.modelMatrix = shape.transformMatrix;

        // 1. Process Vertex Shader
        // Transforming vertices
        std::vector<Vertex> transformedVertices;
        for (Vertex& vertex : shape.vertices) {
            // Vertex transformation logic here...
        }

        // 2. Fragment Shader
        // Processing fragments/pixels
        for (Fragment& fragment : shape.fragments) {
            // Fragment processing logic here...
        }
    }
}
