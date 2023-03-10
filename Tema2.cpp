#include "lab_m1/Tema2/Tema2.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema2::Tema2()
{
}


Tema2::~Tema2()
{
}


//Function that calculates distance between two points
float distanceCalc(glm::vec3 a, glm::vec3 b) {
    float dx = a.x - b.x;
    float dz = a.z - b.z;

    return std::sqrt(dx * dx + dz * dz);
}

bool coll(glm::vec3 a, glm::vec3 b, float r1, float r2) {
    float d = distanceCalc(a, b);

    if (r1 + r2 >= d)
        return true;

    return false;
}

void addPoints(vector<glm::vec3>& p, glm::vec3 a, glm::vec3 b, int n) {
    //int d = distance(a, b);
    float dx = (b.x - a.x) / n;
    float dz = (b.z - a.z) / n;

    for (int i = 0; i < n; i++) {
        float x = a.x + dx * i;
        float z = a.z + dz * i;
        //cout << dx << endl;
        p.push_back(glm::vec3(x, 0, z));
    }
}

void Tema2::Init()
{

    renderCameraTarget = false;

    right = 3.f;
    left = -3.f;
    bottom = -2.f;
    top = 2.f;
    fov = 40.f;
    persp = true;
    scaleFactor = 0.025f;

    camera = new implemented::Camera();
    camera->Set(glm::vec3(1.28f - camera->getDistanceTo(), -0.21f, 2.9f - camera->getDistanceTo()), glm::vec3(0, 1, 0), glm::vec3(0, 10, 0));
    //Set camera angle
    camera->RotateThirdPerson_OX(-0.5f);
    camera->RotateThirdPerson_OY(-0.54f);

    camera->RotateThirdPerson_OY(2);
    ang += 2;

    camera2 = new implemented::Camera();
    camera2->Set(glm::vec3(1.28f - camera2->getDistanceTo(), -0.21f, 2.9f - camera2->getDistanceTo()), glm::vec3(0, 1, 0), glm::vec3(0, 10, 0));
    //Set camera angle
    camera2->RotateThirdPerson_OX(-1.7f);
    camera2->RotateThirdPerson_OY(-0.54f);

    {
        Mesh* mesh = new Mesh("box");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("sphere");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    {
        Mesh* mesh = new Mesh("plane");
        mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
        meshes[mesh->GetMeshID()] = mesh;
    }

    //Track
    {

        vector<glm::vec3> p;
        for (int i = 0; i < l - 1; i++) {
            addPoints(p, track_main_points[i], track_main_points[i + 1], fragm);
        }
        addPoints(p, track_main_points[l - 1], track_main_points[0], fragm);

        int newSize = p.size();


        vector<VertexFormat> vertices
        {
        };

        for (int i = 0; i < newSize - 1; i++) {
            glm::vec3 D = p[i] - p[i + 1];
            glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
            vertices.push_back(VertexFormat(p[i] + tr_thick * P, grey_c, glm::vec3(0.2, 0.8, 0.6)));
            vertices.push_back(VertexFormat(p[i] + tr_thick / 2 * P, grey_c, glm::vec3(0.2, 0.8, 0.6)));
            vertices.push_back(VertexFormat(p[i] - tr_thick / 2 * P, grey_c, glm::vec3(0.2, 0.8, 0.6)));
            vertices.push_back(VertexFormat(p[i] - tr_thick * P, grey_c, glm::vec3(0.2, 0.8, 0.6)));
            side_points[4 * i] = p[i] + tr_thick * P;
            side_points[4 * i + 1] = p[i] + tr_thick / 2 * P;
            side_points[4 * i + 2] = p[i] - tr_thick / 2 * P;
            side_points[4 * i + 3] = p[i] - tr_thick * P;
            middle_points[i] = p[i];
            if (i % 50 == 0) {
                track1[i / 50] = p[i] + tr_thick / 1.5f * P;
                track2[i / 50] = p[i] - tr_thick / 1.5f * P;
            }
        }
        //Manually create points for last point
        glm::vec3 D = p[newSize - 1] - p[0];
        glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
        vertices.push_back(VertexFormat(p[newSize - 1] + tr_thick * P, grey_c, glm::vec3(0.2, 0.8, 0.6)));
        vertices.push_back(VertexFormat(p[newSize - 1] + tr_thick / 2 * P, grey_c, glm::vec3(0.2, 0.8, 0.6)));
        vertices.push_back(VertexFormat(p[newSize - 1] - tr_thick / 2 * P, grey_c, glm::vec3(0.2, 0.8, 0.6)));
        vertices.push_back(VertexFormat(p[newSize - 1] - tr_thick * P, grey_c, glm::vec3(0.2, 0.8, 0.6)));
        side_points[newSize * 4 - 4] = p[newSize - 1] + tr_thick * P;
        side_points[newSize * 4 - 3] = p[newSize - 1] + tr_thick / 2 * P;
        side_points[newSize * 4 - 2] = p[newSize - 1] - tr_thick / 2 * P;
        side_points[newSize * 4 - 1] = p[newSize - 1] - tr_thick * P;
        middle_points[newSize - 1] = p[newSize - 1];
        track1[newSize - 1] = p[newSize - 1] + tr_thick / 1.5f * P;
        track2[newSize - 1] = p[newSize - 1] - tr_thick / 1.5f * P;

        vector<unsigned int> indices =
        {
        };

        for (int i = 0; i < newSize * 4 - 4; i += 4) {

            //first triangle
            indices.push_back(i);
            indices.push_back(i + 1);
            indices.push_back(i + 4);
            //second triangle
            indices.push_back(i + 4);
            indices.push_back(i + 1);
            indices.push_back(i + 5);
            //third triangle
            indices.push_back(i + 1);
            indices.push_back(i + 2);
            indices.push_back(i + 5);
            //fourth triangle
            indices.push_back(i + 5);
            indices.push_back(i + 2);
            indices.push_back(i + 6);
            //5th triangle
            indices.push_back(i + 2);
            indices.push_back(i + 3);
            indices.push_back(i + 6);
            //6th triangle
            indices.push_back(i + 6);
            indices.push_back(i + 3);
            indices.push_back(i + 7);
        }

        //Manually join track ends
        //first triangle
        indices.push_back(newSize * 4 - 4);
        indices.push_back(newSize * 4 - 3);
        indices.push_back(0);
        //second triangle
        indices.push_back(0);
        indices.push_back(newSize * 4 - 3);
        indices.push_back(1);
        //third triangle
        indices.push_back(newSize * 4 - 3);
        indices.push_back(newSize * 4 - 2);
        indices.push_back(1);
        //fourth triangle
        indices.push_back(1);
        indices.push_back(newSize * 4 - 2);
        indices.push_back(2);
        //5th triangle
        indices.push_back(newSize * 4 - 2);
        indices.push_back(newSize * 4 - 1);
        indices.push_back(2);
        //6th triangle
        indices.push_back(2);
        indices.push_back(newSize * 4 - 1);
        indices.push_back(3);


        CreateMesh("track", vertices, indices);
    }



    //Cube mesh
    {
        vector<VertexFormat> vertices
        {
            VertexFormat(glm::vec3(-1, -1, -1), yellow_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-1, -1,  1), yellow_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-1, 1,  1), yellow_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-1, 1, -1), yellow_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, -1, -1), yellow_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, -1, 1), yellow_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, 1), yellow_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(1, 1, -1), yellow_c, glm::vec3(0.2, 0.8, 0.6))

        };

        vector<unsigned int> indices =
        {
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            3, 2, 6,
            6, 7, 3,
            0, 1, 5,
            5, 4, 0,
            1, 2, 6,
            6, 5, 1,
            0, 3, 7,
            7, 4, 0

        };

        CreateMesh("cube", vertices, indices);
    }

    //Enemy mesh
    {
        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        int resolution = 20;
        float radius = 0.06f;

        float PI = 3.14159265358979323846;
        for (int j = 0; j <= resolution; j++) {
            for (int i = 0; i <= resolution; i++) {
                float theta = i * (2 * PI) / resolution;
                float phi = j * PI / resolution;
                glm::vec3 position(
                    radius * cos(theta) * sin(phi),
                    radius * sin(theta) * sin(phi),
                    radius * cos(phi)
                );
                vertices.push_back(VertexFormat(position, red_c, glm::vec3(0.2, 0.8, 0.6)));
            }
        }

        for (int j = 0; j < resolution; j++) {
            for (int i = 0; i < resolution; i++) {
                indices.push_back(j * (resolution + 1) + i);
                indices.push_back(j * (resolution + 1) + i + 1);
                indices.push_back((j + 1) * (resolution + 1) + i);

                indices.push_back((j + 1) * (resolution + 1) + i);
                indices.push_back(j * (resolution + 1) + i + 1);
                indices.push_back((j + 1) * (resolution + 1) + i + 1);
            }
        }


        CreateMesh("enemy", vertices, indices);
    }

    //Tree mesh
    {
        vector<VertexFormat> vertices
        {
            // Trunk
            // Front face
            VertexFormat(glm::vec3(-0.25, 0, 0.25), brown_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.25, 0, 0.25), brown_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.25, 1, 0.25), brown2_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.25, 1, 0.25), brown2_c, glm::vec3(0.2, 0.8, 0.6)),
            // Back face
            VertexFormat(glm::vec3(-0.25, 0, -0.25), brown_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.25, 0, -0.25), brown_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.25, 1, -0.25), brown2_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.25, 1, -0.25), brown2_c, glm::vec3(0.2, 0.8, 0.6)),



            // Leaf
            VertexFormat(glm::vec3(-0.5, 1, 0.5), green_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5, 1, 0.5), green_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5, 2, 0.5), green_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5, 2, 0.5), green_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5, 1, -0.5), green_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5, 1, -0.5), green_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(0.5, 2, -0.5), green_c, glm::vec3(0.2, 0.8, 0.6)),
            VertexFormat(glm::vec3(-0.5, 2, -0.5), green_c, glm::vec3(0.2, 0.8, 0.6)),


        };

        vector<unsigned int> indices =
        {
            // Trunk
            // Front face
            0, 1, 2,
            2, 3, 0,
            // Back face
            4, 5, 6,
            6, 7, 4,
            // Left face
            3, 2, 6,
            6, 7, 3,
            // Right face
            0, 1, 5,
            5, 4, 0,
            // Top face
            1, 2, 6,
            6, 5, 1,
            // Bottom face
            0, 3, 7,
            7, 4, 0,

            // Leaf
            // Front face
            8, 9, 10,
            10, 11, 8,
            // Back face
            12, 13, 14,
            14, 15, 12,
            // Left face
            11, 10, 14,
            14, 15, 11,
            // Right face
            8, 9, 13,
            13, 12, 8,
            // Top face
            9, 10, 14,
            14, 13, 9,
            // Bottom face
            8, 11, 15,
            15, 12, 8,

        };

        CreateMesh("tree", vertices, indices);
    }

    //Grass mesh
    {
        const int detail_level = 80;
        const float step = 2.0f / detail_level;

        std::vector<VertexFormat> vertices;
        std::vector<unsigned int> indices;

        for (int i = 0; i < detail_level; i++) {
            for (int j = 0; j < detail_level; j++) {
                glm::vec3 pos = glm::vec3(-1 + j * step * 50, 0, -1 + i * step * 50);

                glm::vec3 tex_coord = glm::vec3(j / (float)detail_level, 0, i / (float)detail_level);

                vertices.push_back(VertexFormat(pos, grass_c, tex_coord));

                if (i < detail_level - 1 && j < detail_level - 1) {
                    int current = i * detail_level + j;
                    indices.push_back(current);
                    indices.push_back(current + 1);
                    indices.push_back(current + detail_level);

                    indices.push_back(current + 1);
                    indices.push_back(current + detail_level + 1);
                    indices.push_back(current + detail_level);
                }
            }
        }

        CreateMesh("grass", vertices, indices);
    }


    {
        Shader* shader = new Shader("TemaShader");
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
        shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "Tema2", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
        shader->CreateAndLink();
        shaders[shader->GetName()] = shader;
    }

    // TODO(student): After you implement the changing of the projection
    // parameters, remove hardcodings of these parameters
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);

    en_x2 = track1[6].x;
    en_z2 = track1[6].z;

    en_x3 = track2[12].x;
    en_z3 = track2[12].z;
}

void Tema2::CreateMesh(const char* name, const std::vector<VertexFormat>& vertices, const std::vector<unsigned int>& indices)
{
    unsigned int VAO = 0;
    // TODO(student): Create the VAO and bind it
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    unsigned int VBO = 0;
    // TODO(student): Create the VBO and bind it
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // TODO(student): Send vertices data into the VBO buffer
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices[0]) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    unsigned int IBO = 0;
    // TODO(student): Create the IBO and bind it
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);

    // TODO(student): Send indices data into the IBO buffer
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices[0]) * indices.size(), &indices[0], GL_STATIC_DRAW);

    // Set vertex position attribute
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), 0);

    // Set vertex normal attribute
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(sizeof(glm::vec3)));

    // Set texture coordinate attribute
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3)));

    // Set vertex color attribute
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexFormat), (void*)(2 * sizeof(glm::vec3) + sizeof(glm::vec2)));
    // ========================================================================

    // TODO(student): Unbind the VAO
    glBindVertexArray(0);

    // Check for OpenGL errors
    if (GetOpenGLError() == GL_INVALID_OPERATION)
    {
        cout << "\t[NOTE] : For students : DON'T PANIC! This error should go away when completing the tasks." << std::endl;
        cout << "\t[NOTE] : For developers : This happens because OpenGL core spec >=3.1 forbids null VAOs." << std::endl;
    }

    // Mesh information is saved into a Mesh object
    meshes[name] = new Mesh(name);
    meshes[name]->InitFromBuffer(VAO, static_cast<unsigned int>(indices.size()));
}


void Tema2::FrameStart()
{

    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0.529f, 0.808f, 0.921f, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);

    glm::mat4 modelMatrix = glm::mat4(1);
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(2, 2, 2));
    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    RenderMesh(meshes["track"], shaders["TemaShader"], modelMatrix, *camera);



    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-15, -0.01f, -15));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(15, 15, 15));
    RenderMesh(meshes["grass"], shaders["TemaShader"], modelMatrix, *camera);

}

bool Tema2::isInTriangle(glm::vec3 pos, glm::vec3 p1, glm::vec3 p2, glm::vec3 p3) {
    float s = p1.z * p3.x - p1.x * p3.z + (p3.z - p1.z) * pos.x + (p1.x - p3.x) * pos.z;
    float t = p1.x * p2.z - p1.z * p2.x + (p1.z - p2.z) * pos.x + (p2.x - p1.x) * pos.z;

    if ((s > 0) != (t > 0))
        return false;

    float area = -p2.z * p3.x + p1.z * (p3.x - p2.x) + p1.x * (p2.z - p3.z) + p2.x * p3.z;

    if (area < 0) {
        s = -s;
        t = -t;
        area = -area;
    }

    return s > 0 && t > 0 && (s + t) <= area;
}

bool Tema2::onTrack(glm::mat4 modelMatrix) {
    float pos_x, pos_z;
    pos_x = modelMatrix[3][0];
    pos_z = modelMatrix[3][2];
    glm::vec3 pos = glm::vec3(pos_x, 0, pos_z);

    for (int i = 0; i < (l * fragm - 1) * 4; i += 4) {
        if (isInTriangle(pos, side_points[i], side_points[i + 3], side_points[i + 4]) ||
            isInTriangle(pos, side_points[i + 4], side_points[i + 3], side_points[i + 7])
            )
            return true;
    }

    if (isInTriangle(pos, side_points[4 * (l * fragm) - 4], side_points[4 * (l * fragm) - 1], side_points[0]) ||
        isInTriangle(pos, side_points[0], side_points[3], side_points[4 * (l * fragm) - 1]))
        return true;

    return false;
}

void Tema2::RenderTrees(const std::string& shader, implemented::Camera& cam) {
    for (int i = 0; i < 16 * 50; i += 80) {
        glm::vec3 D = middle_points[i] - middle_points[i + 1];
        glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, middle_points[i] + (tr_thick + 7) * P);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.2f, 0));
        modelMatrix = glm::rotate(modelMatrix, 0.3f, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f, 0.6f, 0.6f));
        //projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
        RenderMesh(meshes["tree"], shaders[shader], modelMatrix, cam);
    }
    for (int i = 40; i < 16 * 50; i += 80) {
        glm::vec3 D = middle_points[i] - middle_points[i + 1];
        glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, middle_points[i] + (tr_thick + 7) * P);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.2f, 0));
        modelMatrix = glm::rotate(modelMatrix, 0.9f, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f, 0.6f, 0.6f));
        //projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
        RenderMesh(meshes["tree"], shaders[shader], modelMatrix, cam);
    }
    for (int i = 13; i < 16 * 50; i += 80) {
        glm::vec3 D = middle_points[i] - middle_points[i + 1];
        glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, middle_points[i] - (tr_thick + 7) * P);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.2f, 0));
        modelMatrix = glm::rotate(modelMatrix, 0.7f, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f, 0.6f, 0.6f));
        //projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
        RenderMesh(meshes["tree"], shaders[shader], modelMatrix, cam);
    }
    for (int i = 53; i < 16 * 50; i += 80) {
        glm::vec3 D = middle_points[i] - middle_points[i + 1];
        glm::vec3 P = glm::cross(D, glm::vec3(0, 1, 0));
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, middle_points[i] - (tr_thick + 7) * P);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, -0.2f, 0));
        modelMatrix = glm::rotate(modelMatrix, 0.1f, glm::vec3(0, 1, 0));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f, 0.6f, 0.6f));
        //projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
        RenderMesh(meshes["tree"], shaders[shader], modelMatrix, cam);
    }
}

void Tema2::RenderEnemies(const std::string& shader, implemented::Camera& cam) {

}

void Tema2::Update(float deltaTimeSeconds)
{


    projectionMatrix = glm::perspective(RADIANS(60), window->props.aspectRatio, 0.01f, 200.0f);
    glm::ivec2 resolution = window->GetResolution();
    persp = true;

    timer += deltaTimeSeconds;

    // Render the camera target. This is useful for understanding where
    // the rotation point is, when moving in third-person camera mode.
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    car_pos = glm::vec3(modelMatrix[3][0], modelMatrix[3][1], modelMatrix[3][2]);
    modelMatrix = glm::rotate(modelMatrix, ang, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.05f, 0.125f));
    RenderMesh(meshes["cube"], shaders["TemaShader"], modelMatrix, *camera);

    //Enemy render and movement
    //Enemy 1
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(en_x, 0.06f, en_z));
    modelMatrix = glm::rotate(modelMatrix, t_ang2, glm::vec3(0, 1, 0));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6f, 0.6f, 0.6f));
    RenderMesh(meshes["enemy"], shaders["TemaShader"], modelMatrix, *camera);

    //Enemy 2

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(en_x2, 0.06f, en_z2));
    modelMatrix = glm::rotate(modelMatrix, t_ang2, glm::vec3(0, 1, 0));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
    RenderMesh(meshes["enemy"], shaders["TemaShader"], modelMatrix, *camera);

    //Enemy 3
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(en_x3, 0.06f, en_z3));
    modelMatrix = glm::rotate(modelMatrix, t_ang3, glm::vec3(0, 1, 0));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
    RenderMesh(meshes["enemy"], shaders["TemaShader"], modelMatrix, *camera);

    RenderTrees("TemaShader", *camera);

    glClear(GL_DEPTH_BUFFER_BIT);
    glViewport(resolution.x - 300, 50, resolution.x / 5.f, resolution.y / 5.f);
    projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
    persp = false;
    //cout <<right<<" "<< left << " " << bottom << " " << top <<endl;

    modelMatrix = glm::mat4(1);
    RenderMesh(meshes["track"], shaders["VertexColor"], modelMatrix, *camera2);


    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(-15, -0.01f, -15));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(20, 20, 20));
    RenderMesh(meshes["grass"], shaders["VertexColor"], modelMatrix, *camera2);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
    camera2->SetPosition(glm::vec3(modelMatrix[3][0], 4, modelMatrix[3][2]));
    modelMatrix = glm::rotate(modelMatrix, ang, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.05f, 0.05f, 0.125f));
    RenderMesh(meshes["cube"], shaders["VertexColor"], modelMatrix, *camera2);

    RenderTrees("VertexColor", *camera2);

    //Enemy render and movement
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(en_x, 0.1f, en_z));
    modelMatrix = glm::rotate(modelMatrix, t_ang, glm::vec3(0, 1, 0));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
    RenderMesh(meshes["enemy"], shaders["VertexColor"], modelMatrix, *camera2);

    //Enemy 2
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(en_x2, 0.1f, en_z2));
    modelMatrix = glm::rotate(modelMatrix, t_ang2, glm::vec3(0, 1, 0));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
    RenderMesh(meshes["enemy"], shaders["VertexColor"], modelMatrix, *camera2);

    //Enemy 3
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(en_x3, 0.1f, en_z3));
    modelMatrix = glm::rotate(modelMatrix, t_ang3, glm::vec3(0, 1, 0));
    //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15f, 0.15f, 0.15f));
    RenderMesh(meshes["enemy"], shaders["VertexColor"], modelMatrix, *camera2);

    if (start + 1 < l)
        target = start + 1;
    else {
        target = 0;
    }
    //cout << target <<" : " << start<<endl;
    float dx = track_main_points[target].x - track_main_points[start].x;
    float dz = track_main_points[target].z - track_main_points[start].z;
    t_ang = std::atan2(dz, dx);
    //cout << t_ang << endl;
    float distance = std::sqrt(dx * dx + dz * dz);
    float duration = distance / (enemy_speed);
    //cout << duration<<endl;
    if (timer >= duration) {
        timer = 0;
        if (start < l - 1)
            start++;
        else start = 0;
    }

    //Moving the enemy
    en_x += enemy_speed * deltaTimeSeconds * cos(t_ang);
    en_z += enemy_speed * deltaTimeSeconds * sin(t_ang);

    //cout << en_x << " " << en_z << endl;

    //ENEMY 2!!
    timer2 += deltaTimeSeconds;

    if (start2 + 1 < l)
        target2 = start2 + 1;
    else {
        target2 = 0;
    }
    //cout << target <<" : " << start<<endl;
    float dx2 = track1[target2].x - track1[start2].x;
    float dz2 = track1[target2].z - track1[start2].z;
    t_ang2 = std::atan2(dz2, dx2);
    //cout << t_ang << endl;
    float distance2 = std::sqrt(dx2 * dx2 + dz2 * dz2);
    float duration2 = distance2 / (enemy_speed2);
    //cout << duration<<endl;
    if (timer2 >= duration2) {
        timer2 = 0;
        if (start2 < l - 1)
            start2++;
        else start2 = 0;
    }

    ////Moving the enemy
    en_x2 += enemy_speed2 * deltaTimeSeconds * cos(t_ang2);
    en_z2 += enemy_speed2 * deltaTimeSeconds * sin(t_ang2);

    //ENEMY 3!!
    timer3 += deltaTimeSeconds;

    if (start3 + 1 < l)
        target3 = start3 + 1;
    else {
        target3 = 0;
    }
    //cout << target <<" : " << start<<endl;
    float dx3 = track2[target3].x - track2[start3].x;
    float dz3 = track2[target3].z - track2[start3].z;
    t_ang3 = std::atan2(dz3, dx3);
    //cout << t_ang << endl;
    float distance3 = std::sqrt(dx3 * dx3 + dz3 * dz3);
    float duration3 = distance3 / (enemy_speed3);
    //cout << duration<<endl;
    if (timer3 >= duration3) {
        timer3 = 0;
        if (start3 < l - 1)
            start3++;
        else start3 = 0;
    }

    ////Moving the enemy
    en_x3 += enemy_speed3 * deltaTimeSeconds * cos(t_ang3);
    en_z3 += enemy_speed3 * deltaTimeSeconds * sin(t_ang3);

    //cout << scaleFactor << endl;
}


void Tema2::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema2::RenderMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, implemented::Camera& camera)
{
    if (!mesh || !shader || !shader->program)
        return;

    // Render an object using the specified shader and the specified position
    shader->Use();
    glUniformMatrix4fv(shader->loc_view_matrix, 1, GL_FALSE, glm::value_ptr(camera.GetViewMatrix()));
    glUniformMatrix4fv(shader->loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));
    glUniformMatrix4fv(shader->loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));



    int u_carPosLocation = glGetUniformLocation(shader->program, "car_position");
    glUniform3f(u_carPosLocation, car_pos.x, car_pos.y, car_pos.z);

    int u_scaleFactor = glGetUniformLocation(shader->program, "scale_factor");
    glUniform1f(u_scaleFactor, scaleFactor);


    mesh->Render();
}


/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema2::OnInputUpdate(float deltaTime, int mods)
{
    // move the camera only if MOUSE_RIGHT button is pressed
    float cameraSpeed = 2.0f;

    if (window->KeyHold(GLFW_KEY_W)) {
        // TODO(student): Translate the camera forward
        glm::mat4 modelMatrixCopy = glm::mat4(1);
        modelMatrixCopy = glm::translate(modelMatrixCopy, camera->GetFutureTargetPosition(deltaTime * cameraSpeed));
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        if (onTrack(modelMatrixCopy) && !coll(glm::vec3(modelMatrix[3][0], 0, modelMatrix[3][2]), glm::vec3(en_x, 0, en_z), 0.1f, 0.08f)
            && !coll(glm::vec3(modelMatrix[3][0], 0, modelMatrix[3][2]), glm::vec3(en_x2, 0, en_z2), 0.1f, 0.08f)
            && !coll(glm::vec3(modelMatrix[3][0], 0, modelMatrix[3][2]), glm::vec3(en_x3, 0, en_z3), 0.1f, 0.08f)) {
            camera->MoveForward(deltaTime * cameraSpeed);
        }
    }

    if (window->KeyHold(GLFW_KEY_A)) {
        // TODO(student): Translate the camera to the left
        //camera->TranslateRight(-deltaTime * cameraSpeed);

        camera->RotateThirdPerson_OY(+deltaTime);
        ang += deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_S)) {
        // TODO(student): Translate the camera backward
        glm::mat4 modelMatrixCopy = glm::mat4(1);
        modelMatrixCopy = glm::translate(modelMatrixCopy, camera->GetFutureTargetPosition(-deltaTime * cameraSpeed));
        glm::mat4 modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, camera->GetTargetPosition());
        if (onTrack(modelMatrixCopy) && !coll(glm::vec3(modelMatrix[3][0], 0, modelMatrix[3][2]), glm::vec3(en_x, 0, en_z), 0.1f, 0.08f)
            && !coll(glm::vec3(modelMatrix[3][0], 0, modelMatrix[3][2]), glm::vec3(en_x2, 0, en_z2), 0.1f, 0.08f)
            && !coll(glm::vec3(modelMatrix[3][0], 0, modelMatrix[3][2]), glm::vec3(en_x3, 0, en_z3), 0.1f, 0.08f)) {
            camera->MoveForward(-deltaTime * cameraSpeed);
        }
    }

    if (window->KeyHold(GLFW_KEY_D)) {
        // TODO(student): Translate the camera to the right
        //camera->TranslateRight(deltaTime * cameraSpeed);
        camera->RotateThirdPerson_OY(-deltaTime);
        ang -= deltaTime;
    }

    if (window->KeyHold(GLFW_KEY_Q)) {
        scaleFactor += deltaTime * 0.01f;
    }

    if (window->KeyHold(GLFW_KEY_E)) {
        scaleFactor -= deltaTime * 0.01f;
    }

    if (window->KeyHold(GLFW_KEY_F))
    {
        fov += deltaTime * cameraSpeed * 3;

        if (persp)
            projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);

    }
    if (window->KeyHold(GLFW_KEY_G))
    {
        fov -= deltaTime * cameraSpeed * 3;

        if (persp)
            projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);

    }

    if (window->KeyHold(GLFW_KEY_UP))
    {
        bottom -= deltaTime;
        top += deltaTime;

        if (!persp)
        {
            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
    }
    if (window->KeyHold(GLFW_KEY_DOWN))
    {
        bottom += deltaTime;
        top -= deltaTime;

        if (!persp)
        {
            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
    }
    if (window->KeyHold(GLFW_KEY_LEFT))
    {
        left -= deltaTime;
        right += deltaTime;

        if (!persp)
        {
            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
    }
    if (window->KeyHold(GLFW_KEY_RIGHT))
    {
        left += deltaTime;
        right -= deltaTime;

        if (!persp)
        {
            projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
        }
    }

}


void Tema2::OnKeyPress(int key, int mods)
{
    // Add key press event
    if (key == GLFW_KEY_T)
    {
        renderCameraTarget = !renderCameraTarget;
    }
    // TODO(student): Switch projections
    if (key == GLFW_KEY_O)
    {
        persp = false;
        projectionMatrix = glm::ortho(left, right, bottom, top, Z_NEAR, Z_FAR);
    }
    if (key == GLFW_KEY_P)
    {
        persp = true;
        projectionMatrix = glm::perspective(RADIANS(fov), window->props.aspectRatio, Z_NEAR, Z_FAR);
    }
}


void Tema2::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema2::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    // Add mouse move event

    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        if (window->GetSpecialKeyState() == 0) {
            renderCameraTarget = false;
            // TODO(student): Rotate the camera in first-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            //camera->RotateFirstPerson_OX(-deltaY * sensivityOY);
            //camera->RotateFirstPerson_OY(-deltaX * sensivityOX);
            //camera->RotateFirstPerson_OX(deltaY * sensivityOX);
        }

        if (window->GetSpecialKeyState() & GLFW_MOD_CONTROL) {
            renderCameraTarget = true;
            // TODO(student): Rotate the camera in third-person mode around
            // OX and OY using `deltaX` and `deltaY`. Use the sensitivity
            // variables for setting up the rotation speed.
            //camera->RotateThirdPerson_OX(-deltaY * sensivityOY);
            //camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
            //camera->RotateFirstPerson_OX(-deltaY * sensivityOX);
        }
    }
}


void Tema2::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema2::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema2::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema2::OnWindowResize(int width, int height)
{
}
