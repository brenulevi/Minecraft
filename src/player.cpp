#include "player.h"

#include "game.h"

Player::Player(float movementSpeed, float sensitivity)
    : _transform(glm::vec3(0.0f), glm::vec3(0.0f, -90.0f, 0.0f), glm::vec3(1.0f)), _camera(90.0f, 4.0f/3.0f, 0.1f, 1000.0f),
      _movementSpeed(movementSpeed), _sensitivity(sensitivity), _lastX(400.0f), _lastY(300.0f), _firstMouse(true)
{
}

void Player::update(float deltaTime)
{
    _transform.updateVectors();

    processMovement(deltaTime);
    processRotation(deltaTime);
    doRayCastAction();

    if(Input::getInstance()->isKeyJustPressed(KEY_ESCAPE))
    {
        if(Input::getInstance()->getCursorMode() == CURSOR_MODE_DISABLED)
        {
            Input::getInstance()->setCursorMode(CURSOR_MODE_NORMAL);
        }
        else
        {
            Input::getInstance()->setCursorMode(CURSOR_MODE_DISABLED);
            double x, y;
            Input::getInstance()->getCursorPos(x, y); // Reset
            _lastX = static_cast<float>(x);
            _lastY = static_cast<float>(y);
        }
    }

    if(Input::getInstance()->isKeyJustPressed(KEY_F11))
    {
        Game::getInstance().getWindow().toggleFullscreen();
    }
}

void Player::onResize(int width, int height)
{
    // Evita aspect ratio inválido quando a janela é minimizada ou tem dimensões zero.
    if(width <= 0 || height <= 0)
        return;

    _camera.aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

void Player::processMovement(float deltaTime)
{
    glm::vec3 movement(0.0f);

    glm::vec3 planeForward = glm::normalize(glm::vec3(_transform.forward.x, 0.0f, _transform.forward.z));

    if(Input::getInstance()->isKeyPressed(GLFW_KEY_W))
        movement += planeForward;
    if(Input::getInstance()->isKeyPressed(GLFW_KEY_S))
        movement -= planeForward;
    if(Input::getInstance()->isKeyPressed(GLFW_KEY_A))
        movement -= _transform.right;
    if(Input::getInstance()->isKeyPressed(GLFW_KEY_D))
        movement += _transform.right;
    if(Input::getInstance()->isKeyPressed(GLFW_KEY_SPACE))
        movement += glm::vec3(0.0f, 1.0f, 0.0f);
    if(Input::getInstance()->isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        movement -= glm::vec3(0.0f, 1.0f, 0.0f);
    
    if(glm::length(movement) > 0.0f)
        movement = glm::normalize(movement);
    
    _transform.position += movement * _movementSpeed * deltaTime;
}

void Player::processRotation(float deltaTime)
{
    if(Input::getInstance()->getCursorMode() != CURSOR_MODE_DISABLED)
        return;
        
    double xposD, yposD;
    Input::getInstance()->getCursorPos(xposD, yposD);
    float xpos = static_cast<float>(xposD);
    float ypos = static_cast<float>(yposD);

    if(_firstMouse)
    {
        _lastX = xpos;
        _lastY = ypos;
        _firstMouse = false;
    }

    float xoffset = xpos - _lastX;
    float yoffset = _lastY - ypos; // Reversed since y-coordinates go from bottom to top
    _lastX = xpos;
    _lastY = ypos;

    xoffset *= _sensitivity;
    yoffset *= _sensitivity;

    _transform.rotation.y += xoffset;
    _transform.rotation.x += yoffset;

    if(_transform.rotation.x > 89.0f)
        _transform.rotation.x = 89.0f;
    if(_transform.rotation.x < -89.0f)
        _transform.rotation.x = -89.0f;

    if(_transform.rotation.y > 360.0f || _transform.rotation.y < -360.0f)
        _transform.rotation.y = 0.0f;
}

void Player::doRayCastAction()
{
    raycast(_transform.position, _transform.forward, ACTION_DISTANCE, _lastRaycastHit);

    if(Input::getInstance()->isMouseButtonJustPressed(MOUSE_BUTTON_LEFT))
    {
        if(_lastRaycastHit.hit)
        {
            Game::getInstance().getWorld().breakBlockAt(_lastRaycastHit.blockPosition);
        }
    }
    else if(Input::getInstance()->isMouseButtonJustPressed(MOUSE_BUTTON_RIGHT))
    {
        if(_lastRaycastHit.hit)
        {
            glm::ivec3 placePosition = _lastRaycastHit.blockPosition + _lastRaycastHit.normal;
            Game::getInstance().getWorld().placeBlockAt(placePosition, WATER);
        }
    }
}
