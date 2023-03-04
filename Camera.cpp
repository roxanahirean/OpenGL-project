#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        //TODO
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
        this->cameraFrontDirection = glm::normalize(cameraTarget - cameraPosition);
        this->cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, cameraUp));


    }

	float yaw = -90.0f;

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        //TODO
		//cameraTarget = cameraPosition + cameraFrontDirection;
        return glm::lookAt(cameraPosition, cameraPosition + cameraFrontDirection, cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //TODO
		if (direction == gps::MOVE_FORWARD) {
			cameraPosition += cameraFrontDirection * speed;
		}

		if (direction == gps::MOVE_BACKWARD) {
			cameraPosition -= cameraFrontDirection * speed;
		}

		if (direction == gps::MOVE_LEFT) {
			cameraPosition -= cameraRightDirection * speed;
		}

		if (direction == gps::MOVE_RIGHT) {
			cameraPosition += cameraRightDirection * speed;
		}
		if (direction == MOVE_UP)
		{
			this->cameraPosition += this->cameraUpDirection * speed;
		}
		if (direction == MOVE_DOWN)
		{
			this->cameraPosition -= this->cameraUpDirection * speed;
		}

		cameraTarget = cameraPosition + cameraFrontDirection;
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO
        glm::vec3 front;
        front.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        front.y = sin(glm::radians(pitch));
        front.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
        cameraFrontDirection = glm::normalize(front);
		cameraRightDirection = glm::normalize(glm::cross(cameraFrontDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
		cameraUpDirection = glm::normalize(glm::cross(cameraRightDirection, cameraFrontDirection));
    }

	glm::vec3 Camera::getCameraPosition() {
		return cameraPosition;
	}

	glm::vec3 Camera::getCameraTarget() {
		return cameraTarget;
	}

	glm::vec3 Camera::getCameraUp() {
		return cameraUpDirection;
	}

	glm::vec3 Camera::getCameraFrontDirection(){
		return cameraFrontDirection;
	}

	glm::vec3 Camera::getCameraRightDirection() {
		return cameraRightDirection;
	}

	void Camera::setCameraPosition(glm::vec3 cameraPosition) {
		this->cameraPosition = cameraPosition;
	}

	void Camera::setCameraTarget(glm::vec3 cameraTarget) {
		this->cameraTarget = cameraTarget;
	}

	void Camera::setCameraUp(glm::vec3 cameraUp) {
		this->cameraUpDirection = cameraUp;
	}

	void Camera::setCameraFrontDirection(glm::vec3 cameraFrontDirection) {
		this->cameraFrontDirection = cameraFrontDirection;
	}

	void Camera::setCameraRightDirection(glm::vec3 cameraRightDirection) {
		this->cameraRightDirection = cameraRightDirection;
	}

	void Camera::cameraPresentation() {
		if (glfwGetTime() < 8.0f) {
			this->rotate(0.0f, yaw);
			yaw += 0.25f;
		}

		if(glfwGetTime() >= 8.0f && glfwGetTime() < 10.0f)
			this->move(gps::MOVE_RIGHT, 0.025f);

		if (glfwGetTime() >= 10.0f && glfwGetTime() < 15.0f)
			this->move(gps::MOVE_FORWARD, 0.025f);

		if (glfwGetTime() >= 15.0f && glfwGetTime() < 16.0f) {
			this->rotate(0.0f, yaw);
			yaw -= 0.25f;
		}
			
		if (glfwGetTime() >= 16.0f && glfwGetTime() < 28.0f)
			this->move(gps::MOVE_FORWARD, 0.025f);

		if (glfwGetTime() >= 28.0f && glfwGetTime() < 33.0f) {
			this->rotate(0.0f, yaw);
			yaw -= 0.25f;
		}
			
	}
	
}