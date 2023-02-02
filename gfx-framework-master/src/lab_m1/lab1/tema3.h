#pragma once

#include <string>
#include <unordered_map>

#include "components/simple_scene.h"
#include "components/transform.h"
#include "lab_m1/lab1/lab_camera.h"


namespace m1
{
    class Tema3 : public gfxc::SimpleScene
    {
    public:
        Tema3();
        ~Tema3();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::mat4& ViewMatrix, const glm::vec3& color = glm::vec3(1), Texture2D* texture1 = NULL);
       
        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;
        void Restart();
        void CreateObstacle();
        void CheckObstacle();
        void CheckColossion();
        implemented::CameraTema* camera;
        glm::mat4 projectionMatrix;
        std::unordered_map<std::string, Texture2D*> mapTextures;
        float center,movx,movy,playerAngle,movOX;
        bool mooving;
        int score,gamestate;

        class boulders
        {  
            public :
                glm::vec3 rocks[3];
                glm::vec3 scala[3];
                int angle[3];
                void generateRocks(glm::vec3 position)
                {
                    rocks[0] = position;
                    float x = float(rand() % 10 )/10 + 0.7, y = float(rand() % 10)/ 10 + 0.7, z = float(rand() % 10) / 10 + 0.7;
                    scala[0] = glm::vec3(x, y, z);
                    angle[0] = rand() % 90;
                
                    x = float(rand() % 10) / 10 + 0.7;
                    y = float(rand() % 10) / 10 + 0.7;
                    z = float(rand() % 10) / 10 + 0.7;
                    float xdist = float(rand() % 10) / 10 + 0.2;
                    float heigh = float(rand() % 3) / 10 - 0.3;
                    float ydist = float(rand() % 10) / 10 + 0.2;
                    rocks[1] = position + glm::vec3(xdist,heigh,ydist);
                    scala[1] = glm::vec3(x, y, z);
                    angle[1] = rand() % 90;
                
                    x = float(rand() % 10) / 10 + 0.7;
                    y = float(rand() % 10) / 10 + 0.7;
                    z = float(rand() % 10) / 10 + 0.7;
                    xdist = float(rand() % 10) / 10 + 0.2;
                    heigh = float(rand() % 3) / 10 + 0.2;
                    ydist = float(rand() % 10) / 10 + 0.2;
                    rocks[2] = position - glm::vec3(xdist, -heigh, ydist);
                    scala[2] = glm::vec3(x, y, z);
                    angle[2] = rand() % 90;
                }
        };
        class trees
        {
            public :
                glm::vec3 position;
                std::string type;
                void initiliase(glm::vec3 pos, std::string typ)
                {
                    position = pos;
                    type = typ;
                }
        };

        std::vector<boulders> rocks;
        std::vector<trees> treees;
        std::vector <glm::vec3> poles;
        std::vector <glm::vec3> presents;
        std::vector <bool> collected;
        std::vector < glm::vec3> present2;
        std::vector <bool> collected2;
    };
}   // namespace m1

