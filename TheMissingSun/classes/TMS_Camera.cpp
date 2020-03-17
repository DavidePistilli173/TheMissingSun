#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

#include "../include/tms.hpp"
#include "TMS_Camera.hpp"

TMS_Camera::TMS_Camera() :
    _cameraPos(tms::DEFAULT_CAMERA_POSITION),
    _cameraTarget(tms::DEFAULT_CAMERA_TARGET),
    _cameraUp(tms::DEFAULT_CAMERA_UP),
    _minX(0.0f), _maxX(0.0f), _minY(0.0f), _maxY(0.0f),
    _speedX(0.0f), _speedY(0.0f)
{
    _lookAtMat = glm::lookAt(_cameraPos, _cameraTarget, _cameraUp);
}

glm::mat4 TMS_Camera::getView()
{
    return _lookAtMat;
}

glm::fvec2 TMS_Camera::getPosition() const
{
    return glm::fvec2(_cameraPos.x, _cameraPos.y);
}

void TMS_Camera::setBoundaries(const tms::Rect<float> limits, const int winW, const int winH)
{
    /* Set horizontal and vertical limits. */
    _minX = limits.minX;
    _maxX = limits.maxX - winW;
    _minY = limits.minY;
    _maxY = limits.maxY - winH;

    /* Set camera speed. */
    _speedX = CAMERA_SPEED_COEFF * winW;
    _speedY = CAMERA_SPEED_COEFF * winH;

    /* Reset the camera's position to the centre if it falls outside the new boundaries. */
    if (!_isWithinBoundaries())
    {
        _cameraPos.x = (limits.maxX - limits.minX) / 2;
        _cameraPos.y = (limits.maxY - limits.minY) / 2;

        _cameraTarget = _cameraPos;
        _cameraTarget.z -= TARGET_DISTANCE;

        /* If the camera position was reset, update the view matrix. */
        _lookAtMat = glm::lookAt(_cameraPos, _cameraTarget, _cameraUp);
    }
}

void TMS_Camera::move(Direction dir)
{
    /* Move the camera. */
    switch (dir)
    {
    case Direction::L_UP:
        _cameraPos.x -= _speedX;
        _cameraPos.y -= _speedY;
        if (_cameraPos.x < _minX) _cameraPos.x = _minX;
        if (_cameraPos.y < _minY) _cameraPos.y = _minY;
        break;
    case Direction::C_UP:
        _cameraPos.y -= _speedY;
        if (_cameraPos.y < _minY) _cameraPos.y = _minY;
        break;
    case Direction::R_UP:
        _cameraPos.x += _speedX;
        _cameraPos.y -= _speedY;
        if (_cameraPos.x > _maxX) _cameraPos.x = _maxX;
        if (_cameraPos.y < _minY) _cameraPos.y = _minY;
        break;
    case Direction::R_MID:
        _cameraPos.x += _speedX;
        if (_cameraPos.x > _maxX) _cameraPos.x = _maxX;
        break;
    case Direction::R_DOWN:
        _cameraPos.x += _speedX;
        _cameraPos.y += _speedY;
        if (_cameraPos.x > _maxX) _cameraPos.x = _maxX;
        if (_cameraPos.y > _maxY) _cameraPos.y = _maxY;
        break;
    case Direction::C_DOWN:
        _cameraPos.y += _speedY;
        if (_cameraPos.y > _maxY) _cameraPos.y = _maxY;
        break;
    case Direction::L_DOWN:
        _cameraPos.x -= _speedX;
        _cameraPos.y += _speedY;
        if (_cameraPos.x < _minX) _cameraPos.x = _minX;
        if (_cameraPos.y > _maxY) _cameraPos.y = _maxY;
        break;
    case Direction::L_MID:
        _cameraPos.x -= _speedX;
        if (_cameraPos.x < _minX) _cameraPos.x = _minX;
        break;
    }

    /* Move the target. */
    _cameraTarget = _cameraPos;
    _cameraTarget.z -= TARGET_DISTANCE;

    /* Update the view matrix. */
    _lookAtMat = glm::lookAt(_cameraPos, _cameraTarget, _cameraUp);
}

bool TMS_Camera::_isWithinBoundaries()
{
    if (_cameraPos.x < _minX || _cameraPos.x > _maxX || _cameraPos.y < _minY || _cameraPos.y > _maxY) return false;
    return true;
}
