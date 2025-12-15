#pragma once

#include "transform.h"
#include "camera.h"
#include "input.h"
#include "raycast.h"

#define ACTION_DISTANCE 5.0f

class Player
{
public:
    Player(float movementSpeed, float sensitivity);

    void update(float deltaTime);

    inline Transform& getTransform() { return _transform; }
    inline Camera& getCamera() { return _camera; }
    inline RaycastHit& getLastRaycastHit() { return _lastRaycastHit; }
    inline float getMovementSpeed() const { return _movementSpeed; }
    inline float getSensitivity() const { return _sensitivity; }

    inline void setMovementSpeed(float speed) { _movementSpeed = speed; }
    inline void setSensitivity(float sensitivity) { _sensitivity = sensitivity; }

    void onResize(int width, int height);
    
private:
    void processMovement(float deltaTime);
    void processRotation(float deltaTime);
    void doRayCastAction();

private:
    Transform _transform;
    Camera _camera;

    RaycastHit _lastRaycastHit;

    float _movementSpeed;
    float _sensitivity;

    float _lastX;
    float _lastY;
    bool _firstMouse;
};