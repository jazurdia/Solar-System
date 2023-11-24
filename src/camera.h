// camera.h
#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct Camera {
    glm::vec3 cameraPosition;
    glm::vec3 targetPosition;
    glm::vec3 upVector;
};


glm::mat4 createViewMatrix(const Camera& camera) {
    // print target position
//     std::cout << "target position: " << camera.targetPosition.x << ", " << camera.targetPosition.y << ", " << camera.targetPosition.z << std::endl;

    // print camera position
//     std::cout << "camera position: " << camera.cameraPosition.x << ", " << camera.cameraPosition.y << ", " << camera.cameraPosition.z << std::endl;
    return glm::lookAt(camera.cameraPosition, camera.targetPosition, camera.upVector);
}

Camera setupInitialCamera() {
    Camera camera{};
    camera.cameraPosition = glm::vec3(0.0f, 0.0f, 15.0f);
    camera.targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    return camera;
}

/*
 * CAMERA MOVEMENTS
 *
 * Si la camara se mueve para arriba, la camara debe mantenerse a la misma distancia del target, por lo que deben compensarse (con trigonometria) los movimientos en x y z.
 *
 * */

float cameraMoveAngle = 5.0f;

Camera moveUp(Camera camera) {
    // calcular el ángulo de rotacion
    float angle = glm::radians(cameraMoveAngle);

    // obtener el ángulo actual entre y y z
    float currentAngle = glm::atan(camera.cameraPosition.y, camera.cameraPosition.z);

    angle = currentAngle - angle;

    // round to 1 decimal
    angle = round(angle * 10) / 10;

    // calcular el desplazamiento en y y en z
    float y = glm::sin(angle) * glm::length(camera.cameraPosition);
    float z = glm::cos(angle) * glm::length(camera.cameraPosition);

    // aproximar a un decimal
    y = round(y * 10) / 10;
    z = round(z * 10) / 10;

    // actualizar la posicion de la camara
    camera.cameraPosition.y = y;
    camera.cameraPosition.z = z;

    return camera;
}

Camera moveDown(Camera camera) {
    // Si se llama esta funcion, la cámara va a rotar hacia arriba
    // esto significa que la camara se va a mover en el eje y y en el eje z, manteniendo la misma distancia hacia el target

    // calcular el ángulo de rotacion
    float angle = glm::radians(cameraMoveAngle);

    // obtener el ángulo actual entre y y z
    float currentAngle = glm::atan(camera.cameraPosition.y, camera.cameraPosition.z);

    angle += currentAngle;

    // round to 1 decimal
    angle = round(angle * 10) / 10;

    // calcular el desplazamiento en y y en z
    float y = glm::sin(angle) * glm::length(camera.cameraPosition);
    float z = glm::cos(angle) * glm::length(camera.cameraPosition);

    y = round(y * 10) / 10;
    z = round(z * 10) / 10;

    // actualizar la posicion de la camara
    camera.cameraPosition.y = y;
    camera.cameraPosition.z = z;

    return camera;
}

Camera moveLeft(Camera camera) {
    // Si se llama esta funcion, la cámara va a rotar hacia arriba
    // esto significa que la camara se va a mover en el eje x y en el eje z, manteniendo la misma distancia hacia el target

    // calcular el ángulo de rotacion
    float angle = glm::radians(cameraMoveAngle);

    // obtener el ángulo actual entre x y z
    float currentAngle = glm::atan(camera.cameraPosition.x, camera.cameraPosition.z);

    angle = currentAngle - angle;
    // round to 1 decimal
    angle = round(angle * 10) / 10;

    // calcular el desplazamiento en x y en z
    float x = glm::sin(angle) * glm::length(camera.cameraPosition);
    float z = glm::cos(angle) * glm::length(camera.cameraPosition);

    x = round(x * 10) / 10;
    z = round(z * 10) / 10;

    // actualizar la posicion de la camara
    camera.cameraPosition.x = x;
    camera.cameraPosition.z = z;

    return camera;
}

Camera moveRight(Camera camera) {
    // Si se llama esta funcion, la cámara va a rotar hacia arriba
    // esto significa que la camara se va a mover en el eje x y en el eje z, manteniendo la misma distancia hacia el target

    // calcular el ángulo de rotacion
    float angle = glm::radians(cameraMoveAngle);

    // obtener el ángulo actual entre x y z
    float currentAngle = glm::atan(camera.cameraPosition.x, camera.cameraPosition.z);

    angle += currentAngle;

    // round to 1 decimal
    angle = round(angle * 10) / 10;

    // calcular el desplazamiento en x y en z
    float x = glm::sin(angle) * glm::length(camera.cameraPosition);
    float z = glm::cos(angle) * glm::length(camera.cameraPosition);

    x = round(x * 10) / 10;
    z = round(z * 10) / 10;

    // actualizar la posicion de la camara
    camera.cameraPosition.x = x;
    camera.cameraPosition.z = z;

    return camera;
}

Camera zoomIn(Camera camera) {
    // prevent zooming in too much
    // si la tangente del ángulo entre y y z es mayor a 1, significa que la camara se está acercando demasiado al target
    float distance = glm::length(camera.cameraPosition);
    if (distance <= 3.6f)
        return camera;

    // print z
    camera.cameraPosition.z -= 2.0f;
    return camera;
}

Camera zoomOut(Camera camera) {
    // prevent zooming out too much
    if (camera.cameraPosition.z >= 30.0f)
        return camera;
    camera.cameraPosition.z += 2.0f;
    return camera;
}

