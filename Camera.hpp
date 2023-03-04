#ifndef Camera_hpp
#define Camera_hpp

#include <C:/FAQULTATE/PG LAB/proiect final/glm/glm.hpp>
#include <C:/FAQULTATE/PG LAB/proiect final/glm/gtx/transform.hpp>
#include <GLFW/glfw3.h>

#include <string>

namespace gps {
    
    enum MOVE_DIRECTION {MOVE_FORWARD, MOVE_BACKWARD, MOVE_RIGHT, MOVE_LEFT, MOVE_UP, MOVE_DOWN};
    
    class Camera
    {
    public:
        //Camera constructor
        Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp);
        //return the view matrix, using the glm::lookAt() function
        glm::mat4 getViewMatrix();
        //update the camera internal parameters following a camera move event
        void move(MOVE_DIRECTION direction, float speed);
        //update the camera internal parameters following a camera rotate event
        //yaw - camera rotation around the y axis
        //pitch - camera rotation around the x axis
        void rotate(float pitch, float yaw);
		glm::vec3 getCameraPosition();
		glm::vec3 getCameraTarget();
		glm::vec3 getCameraUp();
		glm::vec3 getCameraFrontDirection();
		glm::vec3 getCameraRightDirection();
		void setCameraPosition(glm::vec3 cameraPosition);
		void setCameraTarget(glm::vec3 cameraTarget);
		void setCameraUp(glm::vec3 cameraUp);
		void setCameraFrontDirection(glm::vec3 cameraFrontDirection);
		void setCameraRightDirection(glm::vec3 cameraFrontDirection);
		void cameraPresentation();
        
    private:
        glm::vec3 cameraPosition;
        glm::vec3 cameraTarget;
        glm::vec3 cameraFrontDirection;
        glm::vec3 cameraRightDirection;
        glm::vec3 cameraUpDirection;
    };
    
}

#endif /* Camera_hpp */
