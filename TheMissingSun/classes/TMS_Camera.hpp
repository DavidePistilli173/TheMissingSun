#ifndef TMS_CAMERA_HPP
#define TMS_CAMERA_HPP

#include <glm.hpp>

class TMS_Camera
{
public:
    /* Possible directions of camera movement. */
    enum class Direction
    {
        L_UP,
        C_UP,
        R_UP,
        R_MID,
        R_DOWN,
        C_DOWN,
        L_DOWN,
        L_MID
    };

    TMS_Camera();

    /***************** CONSTANTS *****************/
    static constexpr float CAMERA_SPEED_COEFF = 0.025f; // Speed multiplier for the camera.
    static constexpr float TARGET_DISTANCE = 5.0f; // Distance of the camera's target.

    /***************** METHODS *****************/
    glm::mat4 getView(); // Return the view matrix.
    void setBoundaries(const tms::Rect limits, const int winW, const int winH); // Set the coordinate limits for camera movement.
    void move(Direction dir);

private:
    /***************** METHODS *****************/
    bool _isWithinBoundaries(); // Check whether the camera is within its boundaries or not.

    /***************** VARIABLES *****************/
    glm::vec3 _cameraPos; // Position of the camera.
    glm::vec3 _cameraTarget; // Position the camera is looking at.
    glm::vec3 _cameraUp; // Direction considered "up" for the camera.
    glm::mat4 _lookAtMat; // View matrix.

    int _minX, _maxX, _minY, _maxY; // Coordinate limits.
    float _speedX, _speedY; // Camera speeds.
};

#endif