#pragma once

#include "components/simple_scene.h"
#include "lab_m1/lab5/lab_camera.h"

#define Z_FAR		(200.f)
#define Z_NEAR		(.01f)

namespace m1
{
    class Tema2 : public gfxc::SimpleScene
    {
    public:
        Tema2();
        ~Tema2();

        void Init() override;

    private:
        void FrameStart() override;
        void Update(float deltaTimeSeconds) override;
        void FrameEnd() override;

        void RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, implemented::Camera& camera);

        void OnInputUpdate(float deltaTime, int mods) override;
        void OnKeyPress(int key, int mods) override;
        void OnKeyRelease(int key, int mods) override;
        void OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY) override;
        void OnMouseBtnPress(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods) override;
        void OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY) override;
        void OnWindowResize(int width, int height) override;

        void Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices);
        bool Tema2::onTrack(glm::mat4 modelMatrix);
        bool Tema2::isInTriangle(glm::vec3 pos, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3);
        void Tema2::RenderTrees(const std::string& shader, implemented::Camera& cam);
        void Tema2::RenderEnemies(const std::string& shader, implemented::Camera& cam);

    protected:
        implemented::Camera* camera;
        implemented::Camera* camera2;
        glm::mat4 projectionMatrix;
        bool renderCameraTarget;

        float right;
        float left;
        float bottom;
        float top;
        float fov;

        bool persp;

        glm::vec3 grey_c = glm::vec3(0.412f, 0.412f, 0.412f);
        glm::vec3 green_c = glm::vec3(0, 0.5f, 0);
        glm::vec3 yellow_c = glm::vec3(1, 1, 0);
        glm::vec3 red_c = glm::vec3(0.823529f, 0.117647f, 0.117647f);
        glm::vec3 brown_c = glm::vec3(0.6, 0.4, 0.2);
        glm::vec3 brown2_c = glm::vec3(0.5, 0.3, 0.1);
        glm::vec3 grass_c = glm::vec3(0.4f, 0.7f, 0.2f);

        float tr_thick = 6.f;
        float ang = 0;
        glm::mat4 modelMatrixCopy;
        glm::vec3 car_pos;
        float scaleFactor = 0;

        //Enemy
        float timer = 0;
        float enemy_speed = 1;
        float en_x = -1.5f;
        float en_z = 4.78f;
        float t_ang = 0;
        int start = 2;
        int target;

        //Enemy 2
        float timer2 = 0;
        float enemy_speed2 = 1.2f;
        float en_x2 = -1.5f;
        float en_z2 = 4.78f;
        float t_ang2 = 0;
        int start2 = 6;
        int target2;

        //Enemy 3
        float timer3 = 0;
        float enemy_speed3 = 1.1f;
        float en_x3 = -1.5f;
        float en_z3 = 4.78f;
        float t_ang3 = 0;
        int start3 = 12;
        int target3;

        int l = 16;
        int fragm = 50;

        glm::vec3 track_main_points[16] = { glm::vec3(-9.46f, 0, 1.96f), glm::vec3(-5.32f, 0, 5.2f), glm::vec3(-1.5f, 0, 4.78f),
                                            glm::vec3(1.28f, 0, 2.9f), glm::vec3(4, 0, 1.84f),
                                            glm::vec3(7.26f, 0, 4.34f), glm::vec3(10.48f, 0, 4.5), glm::vec3(12.42f, 0, 1.82f),
                                            glm::vec3(12.24f, 0, -2.32f), glm::vec3(9, 0, -4.76f),
                                            glm::vec3(7.3f, 0, -8.44f), glm::vec3(2.12f, 0, -9.32f), glm::vec3(-0.18f, 0, -7.62f),
                                            glm::vec3(-4.54f, 0, -8.16f), glm::vec3(-8.1f, 0, -6.66f), glm::vec3(-10.4f, 0, -2.18f) };
        
        glm::vec3 side_points[16*50*4];

        glm::vec3 middle_points[16*50];

        glm::vec3 track1[16];

        glm::vec3 track2[16];

    };
}   // namespace m1
