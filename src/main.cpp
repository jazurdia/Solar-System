#include <SDL.h>
#include "gl.h"
#include "camera.h"
#include "uniforms.h"
#include "shaders.h"
#include "object.h"
#include "triangle.h"
#include <iostream>
#include <vector>

// Constantes.
std::vector<Model> models;
std::string planet;

bool shipMoving = false;

bool init() {
    if (SDL_Init(SDL_INIT_VIDEO) != 0) {
        std::cerr << "Error: Failed to initialize SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("Software Renderer", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window) {
        std::cerr << "Error: Failed to create SDL window: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer) {
        std::cerr << "Error: Failed to create SDL renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    setupNoise();

    return true;
}

using namespace std;
void render() {
    for (auto model : models) {
        Uniforms uniform = model.uniforms;
        uniform.model = model.modelMatrix;

        // 1. Vertex Shader
        // vertex -> transformedVertices
        std::vector<Vertex> transformedVertices;

        for (int i = 0; i < model.vertices.size(); i+=3) {
            glm::vec3 v = model.vertices[i];
            glm::vec3 n = model.vertices[i+1];
            glm::vec3 t = model.vertices[i+2];

            auto vertex = Vertex{v, n, t};

            Vertex transformedVertex = vertexShader(vertex, uniform);
            transformedVertices.push_back(transformedVertex);
        }

        // 2. Primitive Assembly
        // transformedVertices -> triangles
        std::vector<std::vector<Vertex>> triangles = primitiveAssembly(transformedVertices);


        // 3. Rasterize
        // triangles -> Fragments
        std::vector<Fragment> fragments;
        for (const std::vector<Vertex>& triangleVertices : triangles) {
            std::vector<Fragment> rasterizedTriangle = triangle(
                    triangleVertices[0],
                    triangleVertices[1],
                    triangleVertices[2]
            );
            fragments.insert(
                    fragments.end(),
                    rasterizedTriangle.begin(),
                    rasterizedTriangle.end()
            );
        }


        // 4. Fragment Shader
        for (Fragment fragment : fragments) {
            switch (model.shader) {
                case Shader::Earth:
                    point(fragmentShaderEarth5(fragment));
                    break;
                case Shader::Sun:
                    point(fragmentShaderSun(fragment));
                    break;
                case Shader::Jupiter:
                    point(fragmentShaderJupiter(fragment));
                    break;
                case Shader::Uranus:
                    point(fragmentShaderUranusRevised(fragment));
                    break;
                case Shader::Mars:
                    point(fragmentShaderMars(fragment));
                    break;
                case Shader::Neptune:
                    point(fragmentShaderNeptune(fragment));
                    break;
                case Shader::Noise:
                    point(noiseFragmentShader(fragment));
                    break;
                case Shader::Ship:
                    point(shipFragmentShader(fragment));
                    break;
                default:
                    point(fragmentShader(fragment));
                    break;
            }
        }
    }
}

std::vector<glm::vec3> setupVertexFromObject(const std::vector<Face>& faces, const std::vector<glm::vec3>& vertices, const std::vector<glm::vec3>& normals, const std::vector<glm::vec3>& texCoords){
    std::vector<glm::vec3> vertexBufferObject;

    for (const auto& face : faces)
    {
        for (int i = 0; i < 3; ++i)
        {
            // Get the vertex position
            glm::vec3 vertexPosition = vertices[face.vertexIndices[i]];

            // Get the normal for the current vertex
            glm::vec3 vertexNormal = normals[face.normalIndices[i]];

            // Get the texture for the current vertex
            glm::vec3 vertexTexture = texCoords[face.texIndices[i]];

            // Add the vertex position and normal to the vertex array
            vertexBufferObject.push_back(vertexPosition);
            vertexBufferObject.push_back(vertexNormal);
            vertexBufferObject.push_back(vertexTexture);
        }
    }

    return vertexBufferObject;
}

std::vector<Model> c_update(std::vector<Model>& mToUpdate, Camera newCamera) {
    for (auto& model : mToUpdate) {
        model.uniforms.view = createViewMatrix(newCamera);
    }
    return mToUpdate;
}


int main(int argc, char** argv) {
    if (!init()) {
        return 1;
    }

    Camera camera = setupInitialCamera();

    std::vector<glm::vec3> planetVertices, planetNormals, planetTexCoords, shipVertices, shipNormals, shipTexCoords;
    std::vector<Face> planetFaces, shipFaces;

    // Load the OBJ file
    bool success1 = loadOBJ("../model/sphere.obj", planetVertices, planetFaces, planetNormals, planetTexCoords);
    bool success2 = loadOBJ("../model/naveEspacial.obj", shipVertices, shipFaces, shipNormals, shipTexCoords);
    if (!success1 || !success2) {
        std::cerr << "Error loading OBJ file!" << std::endl;
        return 1;
    }


    // OBJ into VBO
    std::vector<glm::vec3> planetVBO = setupVertexFromObject(planetFaces, planetVertices, planetNormals, planetTexCoords);
    std::vector<glm::vec3> shipVBO = setupVertexFromObject(shipFaces, shipVertices, shipNormals, shipTexCoords);

    Uint32 frameStart, frameTime; // For calculating the frames per second

    Uniforms shipUniform = planetBaseUniform(camera);
    float shipScale = 0.03f;

    glm::vec3 shipTranslationVector(0.0f, 0.4f, 13.5f);
    glm::vec3 shipRotationAxis(0.0f, 1.0f, 1.5f); // Rotate around the Y-axis every model
    glm::vec3 shipScaleFactor(shipScale, shipScale, shipScale);  // Scale of the model

    // Create model
    Model shipModel;
    shipModel.vertices = shipVBO;
    shipModel.uniforms = shipUniform;
    shipModel.shader = Shader::Ship;

    Uniforms sunUniform = planetBaseUniform(camera);
    float sunScale = 3.0f;

    glm::vec3 sunTranslationVector(0.0f, 0.0f, 0.0f);  // Move the model to the center of the world
    glm::vec3 sunRotationAxis(0.0f, 1.0f, 0.0f); // Rotate around the Y-axis every model
    glm::vec3 sunScaleFactor(sunScale, sunScale, sunScale);  // Scale of the model

    // Create model
    Model sunModel;
    sunModel.vertices = planetVBO;
    sunModel.uniforms = sunUniform;
    sunModel.shader = Shader::Sun;

    Uniforms earthUniform = planetBaseUniform(camera);
    float earthScale = 0.3f;

    glm::vec3 earthRotationAxis(0.0f, 1.0f, 0.0f); // Rotate around the Y-axis every model
    glm::vec3 earthScaleFactor(earthScale, earthScale, earthScale);  // Scale of the model

    // Create model
    Model earthModel;
    earthModel.vertices = planetVBO;
    earthModel.uniforms = earthUniform;
    earthModel.shader = Shader::Earth;

    Uniforms jupiterUniform = planetBaseUniform(camera);
    float jupiterScale = 0.7f;

    glm::vec3 jupiterRotationAxis(0.0f, 1.0f, 0.0f); // Rotate around the Y-axis every model
    glm::vec3 jupiterScaleFactor(jupiterScale, jupiterScale, jupiterScale);  // Scale of the model

    // Create model
    Model jupiterModel;
    jupiterModel.vertices = planetVBO;
    jupiterModel.uniforms = jupiterUniform;
    jupiterModel.shader = Shader::Jupiter;

    Uniforms uranusUniform = planetBaseUniform(camera);
    float uranusScale = 0.5f;

    glm::vec3 uranusRotationAxis(0.0f, 1.0f, 0.0f); // Rotate around the Y-axis every model
    glm::vec3 uranusScaleFactor(uranusScale, uranusScale, uranusScale);  // Scale of the model

    // Create model
    Model uranusModel;
    uranusModel.vertices = planetVBO;
    uranusModel.uniforms = uranusUniform;
    uranusModel.shader = Shader::Uranus;

    Uniforms marsUniform = planetBaseUniform(camera);
    float marsScale = 0.4f;

    glm::vec3 marsRotationAxis(0.0f, 1.0f, 0.0f); // Rotate around the Y-axis every model
    glm::vec3 marsScaleFactor(marsScale, marsScale, marsScale);  // Scale of the model

    // Create model
    Model marsModel;
    marsModel.vertices = planetVBO;
    marsModel.uniforms = marsUniform;
    marsModel.shader = Shader::Mars;

    Uniforms neptuneUniform = planetBaseUniform(camera);
    float neptuneScale = 0.6f;

    glm::vec3 neptuneRotationAxis(0.0f, 2.0f, 0.0f); // Rotate around the Y-axis every model
    glm::vec3 neptuneScaleFactor(neptuneScale, neptuneScale, neptuneScale);  // Scale of the model

    // Create model
    Model neptuneModel;
    neptuneModel.vertices = planetVBO;
    neptuneModel.uniforms = neptuneUniform;
    neptuneModel.shader = Shader::Neptune;

    cout << "Starting loop" << endl;

    bool running = true;
    bool orbiting = true;

    float rotationSpeedPlanets = 1.0f;  // Base speed of the planets
    float orbitSpeedPlanets = 1.0f;  // Base speed of the planets

    float rotationAngleSun = 0.2f;

    float earthOrbitAngle = -90.0f;
    float earthDistanceToSun = -3.5f;
    float rotationAngleEarth = 0.0f;

    float marsOrbitAngle = -50.0f;
    float marsDistanceToSun = 5.0f;
    float rotationAngleMars = 0.0f;

    float jupiterOrbitAngle = 175.0f;
    float jupiterDistanceToSun = -6.5f;
    float rotationAngleJupiter = 4.0f;

    float uranusOrbitAngle = 80.0f;
    float uranusDistanceToSun = 8.0f;
    float rotationAngleUranus = -3.0f;

    float neptuneOrbitAngle = 200.0f;
    float neptuneDistanceToSun = 9.5f;
    float rotationAngleNeptune = 0.0f;

    if (shipMoving) {
        shipModel.shader = Shader::ShipMoving; // Asumiendo que Shader::ShipMoving está definido
    } else {
        shipModel.shader = Shader::Ship;
    }

    while (running) {
        frameStart = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event)) {
            if (event.type == SDL_QUIT) {
                running = false;
            }

            if (event.type == SDL_KEYDOWN) {
                switch (event.key.keysym.sym) {
                    case SDLK_s:
                    case SDLK_d:
                        shipMoving = true;
                        break;
                        // otros casos
                }
            }

            if (event.type == SDL_KEYUP) {
                switch (event.key.keysym.sym) {
                    case SDLK_s:
                    case SDLK_d:
                        shipMoving = false;
                        break;
                        // otros casos
                }
            }

            if (event.type == SDL_KEYDOWN) {
                float increment = 0.5f;
                switch (event.key.keysym.sym) {
                    case SDLK_ESCAPE:
                        running = false;
                        break;
                    case SDLK_w:
                        camera = zoomIn(camera);
                        break;
                    case SDLK_s:
                        camera = zoomOut(camera);
                        break;
                    case SDLK_a:
                        camera = moveLeft(camera);
                        break;
                    case SDLK_d:
                        camera = moveRight(camera);
                        break;
                    case SDLK_LEFT:
                        rotationSpeedPlanets -= increment;
                        orbitSpeedPlanets -= increment;

                        break;
                    case SDLK_RIGHT:
                        rotationSpeedPlanets += increment;
                        orbitSpeedPlanets += increment;

                        break;
                    case SDLK_p:
                        orbiting = !orbiting;
                        break;
                }
            }
        }


        rotationAngleSun += rotationSpeedPlanets * 0.2f;
        rotationAngleEarth += rotationSpeedPlanets * 0.8f;
        rotationAngleMars += rotationSpeedPlanets * 0.6f;
        rotationAngleJupiter += rotationSpeedPlanets * 0.4f;
        rotationAngleUranus += rotationSpeedPlanets * 0.3f;

        if(orbiting) {
            earthOrbitAngle += 1.0f * orbitSpeedPlanets;
            marsOrbitAngle += 0.8f * orbitSpeedPlanets;
            jupiterOrbitAngle += 0.6f * orbitSpeedPlanets;
            uranusOrbitAngle += 0.4f * orbitSpeedPlanets;
            neptuneOrbitAngle += 0.3f * orbitSpeedPlanets;
        }

        shipUniform.model = createShipModelMatrix(shipTranslationVector, shipScaleFactor);
        shipModel.modelMatrix = shipUniform.model;


        sunUniform.model = createModelMatrix(sunTranslationVector, sunScaleFactor, sunRotationAxis, rotationAngleSun);
        sunModel.modelMatrix = sunUniform.model;

        models.push_back(sunModel);

        glm::vec3 earthTranslationVector(
                earthDistanceToSun * cos(glm::radians(earthOrbitAngle)),
                -0.2f,
                earthDistanceToSun * sin(glm::radians(earthOrbitAngle))
        );
        earthUniform.model = createModelMatrix(earthTranslationVector, earthScaleFactor, earthRotationAxis, rotationAngleEarth);
        earthModel.modelMatrix = earthUniform.model;

        models.push_back(earthModel);

        // move the planet around the sun on the x and y axis

        glm::vec3 marsTranslationVector(
                marsDistanceToSun * cos(glm::radians(marsOrbitAngle)),
                0.3f,
                marsDistanceToSun * sin(glm::radians(marsOrbitAngle))
        );
        marsUniform.model = createModelMatrix(marsTranslationVector, marsScaleFactor, marsRotationAxis, rotationAngleMars);
        marsModel.modelMatrix = marsUniform.model;

        models.push_back(marsModel);


        glm::vec3 jupiterTranslationVector(
                jupiterDistanceToSun * cos(glm::radians(jupiterOrbitAngle)),
                -0.2f,
                jupiterDistanceToSun * sin(glm::radians(jupiterOrbitAngle))
        );
        jupiterUniform.model = createModelMatrix(jupiterTranslationVector, jupiterScaleFactor, jupiterRotationAxis, rotationAngleJupiter);
        jupiterModel.modelMatrix = jupiterUniform.model;

        models.push_back(jupiterModel);

        glm::vec3 uranusTranslationVector(
                uranusDistanceToSun * cos(glm::radians(uranusOrbitAngle)),
                -0.1f,
                uranusDistanceToSun * sin(glm::radians(uranusOrbitAngle))
        );
        uranusUniform.model = createModelMatrix(uranusTranslationVector, uranusScaleFactor, uranusRotationAxis, rotationAngleUranus);
        uranusModel.modelMatrix = uranusUniform.model;

        models.push_back(uranusModel);

        glm::vec3 nepTranslationVector(
                neptuneDistanceToSun * cos(glm::radians(neptuneOrbitAngle)),
                -0.1f,
                neptuneDistanceToSun * sin(glm::radians(neptuneOrbitAngle))
        );
        neptuneUniform.model = createModelMatrix(nepTranslationVector, neptuneScaleFactor, neptuneRotationAxis, rotationAngleNeptune);
        neptuneModel.modelMatrix = neptuneUniform.model;

        models.push_back(neptuneModel);

        clear();

        models = c_update(models, camera);

        models.push_back(shipModel);

        render();

        models.clear();

        // Present the frame buffer to the screen
        SDL_RenderPresent(renderer);

        // Delay to limit the frame rate
        SDL_Delay(1000 / 60);

        frameTime = SDL_GetTicks() - frameStart;

        // Calculate frames per second and update window title
        if (frameTime > 0) {
            std::ostringstream titleStream;
            titleStream << planet + " FPS: " << 1000.0 / frameTime;  // Milliseconds to seconds
            SDL_SetWindowTitle(window, titleStream.str().c_str());
        }
    }

    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
