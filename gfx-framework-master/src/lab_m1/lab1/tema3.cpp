#include "lab_m1/lab1/tema3.h"

#include <vector>
#include <string>
#include <iostream>

using namespace std;
using namespace m1;


/*
 *  To find out more about `FrameStart`, `Update`, `FrameEnd`
 *  and the order in which they are called, see `world.cpp`.
 */


Tema3::Tema3()
{
}


Tema3::~Tema3()
{
}


void Tema3::Init()
{
    //CAMERA && PROJECTION
    camera = new implemented::CameraTema();
    camera->Set(glm::vec3(12, 5, 0), glm::vec3(8, 5, 0), glm::vec3(0, 1, 0));
    projectionMatrix = glm::perspective(glm::radians(60.f), window->props.aspectRatio, 0.01f, 200.0f);
    camera->RotateFirstPerson_OX(RADIANS(-30));
    //SHADER
    Shader* shader = new Shader("ShaderTema3");
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab1", "shaders", "VertexShader.glsl"), GL_VERTEX_SHADER);
    shader->AddShader(PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab1", "shaders", "FragmentShader.glsl"), GL_FRAGMENT_SHADER);
    shader->CreateAndLink();
    shaders[shader->GetName()] = shader;

    //MESHES
    Mesh* mesh = new Mesh("plane");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "plane50.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("box");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "box.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("sphere");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "sphere.obj");
    meshes[mesh->GetMeshID()] = mesh;

    mesh = new Mesh("cone");
    mesh->LoadMesh(PATH_JOIN(window->props.selfDir, RESOURCE_PATH::MODELS, "primitives"), "cone.obj");
    meshes[mesh->GetMeshID()] = mesh;

    //TEXTURES
    const string sourceTextureDir = PATH_JOIN(window->props.selfDir, SOURCE_PATH::M1, "lab1", "textures");

    Texture2D* texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "snow_ground_big.jpg").c_str(), GL_REPEAT);
    mapTextures["snow"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "snow.jpg").c_str(), GL_REPEAT);
    mapTextures["snow2"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "red labirint.png").c_str(), GL_REPEAT);
    mapTextures["model"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "carrot.jpg").c_str(), GL_REPEAT);
    mapTextures["carrot"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "rock.jpg").c_str(), GL_REPEAT);
    mapTextures["rock"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "bark1.jpg").c_str(), GL_REPEAT);
    mapTextures["bark1"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "needles.jpg").c_str(), GL_REPEAT);
    mapTextures["pine"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "bark2.jpg").c_str(), GL_REPEAT);
    mapTextures["bark2"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "xmastree.jpg").c_str(), GL_REPEAT);
    mapTextures["xmastree"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "steel.jpg").c_str(), GL_REPEAT);
    mapTextures["steel"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "present1.jpg").c_str(), GL_REPEAT);
    mapTextures["present1"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "present2.jpg").c_str(), GL_REPEAT);
    mapTextures["present2"] = texture;

    texture = new Texture2D();
    texture->Load2D(PATH_JOIN(sourceTextureDir, "beam.jpg").c_str(), GL_REPEAT);
    mapTextures["beam"] = texture;

    //VARIABLES
    center = 0;
    srand(time(NULL));

    for (int i = -7; i < 7; i++)
    {
        int r = rand() % 6;
        float y = (center + i * 3)* 1/tan(RADIANS(-60));
        int z = rand() % 28 - 14;
        if (i == 0 || i == 1)
        {
            while (z < 4 && z > -4)
            {
                z = rand() % 40 - 20;
            }
        }
        boulders boulder;
        trees tree;
        switch (r)
        {
        case 0:
            boulder.generateRocks(glm::vec3(i*3,y,z));
            rocks.push_back(boulder);
            break;
        case 1:
            tree.initiliase(glm::vec3(i*3,y,z),"PINE");
            treees.push_back(tree);
            break;
        case 2:
            tree.initiliase(glm::vec3(i * 3, y, z), "KMASTREE");
            treees.push_back(tree);
            break;
        case 3:
            poles.push_back(glm::vec3(i * 3, y, z));
            break;
        case 4:
            presents.push_back(glm::vec3(i * 3, y, z));
            collected.push_back(false);
            break;
        case 5:
            present2.push_back(glm::vec3(i * 3, y, z));
            collected2.push_back(false);
            break;
        default:
            break;
        }
        
    }

    movx = 0;
    movy = 0;
    playerAngle = 0;
    movOX = 0;
    score = 0;
    gamestate = 0;
}


void Tema3::FrameStart()
{
    // Clears the color buffer (using the previously set color) and depth buffer
    glClearColor(0, 0, 0, 1);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::ivec2 resolution = window->GetResolution();
    // Sets the screen area where to draw
    glViewport(0, 0, resolution.x, resolution.y);
}

void Tema3::CreateObstacle()
{
    int r = rand() % 6;
    float y = (center + movx + 24) * 1 / tan(RADIANS(-60));
    int z = rand() % 28 - 14;
    boulders boulder;
    trees tree;
    switch (r)
    {
    case 0:
        boulder.generateRocks(glm::vec3(movx + 24, y, z));
        rocks.push_back(boulder);
        break;
    case 1:
        tree.initiliase(glm::vec3(movx + 24, y, z), "PINE");
        treees.push_back(tree);
        break;
    case 2:
        tree.initiliase(glm::vec3(movx + 24, y, z), "KMASTREE");
        treees.push_back(tree);
        break;
    case 3:
        poles.push_back(glm::vec3(movx + 24, y, z));
        break;
    case 4:
        presents.push_back(glm::vec3(movx + 24, y, z));
        collected.push_back(false);
        break;
    case 5:
        present2.push_back(glm::vec3(movx + 24, y, z));
        collected2.push_back(false);
        break;
    default:
        break;
    }
}

void Tema3::CheckObstacle()
{
    bool del = false;
    for (int i = 0; i < rocks.size() ; i++)
    {
        if (rocks[i].rocks[0].x < movx - 24)
        {
            rocks.erase(rocks.begin() + i);
            del = true;
            break;
        }
    }

    for (int i = 0; i < treees.size() &&!del; i++)
    {
        if (treees[i].position.x < movx - 24)
        {
            treees.erase(treees.begin()+ i);
            del = true;
            break;
        }
    }
    for (int i = 0; i < poles.size() && !del; i++)
    {
        if (poles[i].x < movx - 24)
        {
            poles.erase(poles.begin() + i);
            del = true;
            break;
        }
    }
    for (int i = 0; i < presents.size() && !del; i++)
    {
        if (presents[i].x < movx - 24)
        {
            presents.erase(presents.begin() + i);
            collected.erase(collected.begin() + i);
            del = true;
            break;
        }
    }
    for (int i = 0; i < present2.size() && !del; i++)
    {
        if (present2[i].x < movx - 24)
        {
            present2.erase(present2.begin() + i);
            collected2.erase(collected2.begin() + i);
            del = true;
            break;
        }
    }
    if (del)
    {
        CreateObstacle();
    }
    
}

void Tema3::CheckColossion()
{
    bool bonk = false;
    glm::vec3 pos = glm::vec3(movx, movy, movOX);
    for (int i = 0; i < rocks.size(); i++)
    {
        if (glm::distance(pos,rocks[i].rocks[0]) < 1)
        {
            bonk = true;
            break;
        }
    }

    for (int i = 0; i < treees.size() && !bonk; i++)
    {
        if (glm::distance(treees[i].position,pos) < 1)
        {
            bonk = true;
            break;
        }
    }
    for (int i = 0; i < poles.size() && !bonk; i++)
    {
        if (glm::distance( poles[i],pos) < 1)
        {
            bonk = true;
            break;
        }
    }
    for (int i = 0; i < presents.size() && !bonk; i++)
    {
        if (glm::distance(presents[i],pos) < 1 && !collected[i])
        {
            collected[i] = true;
            score++;
            break;
        }
    }
    for (int i = 0; i < present2.size() && !bonk; i++)
    {
        if (glm::distance(present2[i],pos) < 1 && !collected2[i])
        {
            collected2[i] = true;
            score+= 2;
            break;
        }
        if (glm::distance(present2[i] + glm::vec3(0,0,-2), pos) < 1 || glm::distance(present2[i] + glm::vec3(0,0,2), pos) < 1)
        {
            bonk = true;
            break;
        }
    }
    if (movOX < -20 || movOX > 20)
    {
        bonk = true;
    }
    if (bonk)
    {
        cout << "GAMEOVER\n SCORE = " <<" " << score << endl;
        cout << "Press R to restart\n";
        gamestate = 1;
    }
    
}

void Tema3::Restart()
{
    treees.clear();
    rocks.clear();
    poles.clear();
    presents.clear();
    present2.clear();
    collected.clear();
    collected2.clear();
    for (int i = -7; i < 7; i++)
    {
        int r = rand() % 6;
        float y = (center + i * 3) * 1 / tan(RADIANS(-60));
        int z = rand() % 28 - 14;
        if (i == 0 || i == 1)
        {
            while (z < 4 && z > -4)
            {
                z = rand() % 40 - 20;
            }
        }
        boulders boulder;
        trees tree;
        switch (r)
        {
        case 0:
            boulder.generateRocks(glm::vec3(i * 3, y, z));
            rocks.push_back(boulder);
            break;
        case 1:
            tree.initiliase(glm::vec3(i * 3, y, z), "PINE");
            treees.push_back(tree);
            break;
        case 2:
            tree.initiliase(glm::vec3(i * 3, y, z), "KMASTREE");
            treees.push_back(tree);
            break;
        case 3:
            poles.push_back(glm::vec3(i * 3, y, z));
            break;
        case 4:
            presents.push_back(glm::vec3(i * 3, y, z));
            collected.push_back(false);
            break;
        case 5:
            present2.push_back(glm::vec3(i * 3, y, z));
            collected2.push_back(false);
            break;
        default:
            break;
        }

    }

    movx = 0;
    movy = 0;
    //playerAngle = 0;
    movOX = 0;
    score = 0;
    gamestate = 0;
}

void Tema3::Update(float deltaTimeSeconds)
{
    mooving = true;
    glm::mat4 modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(movx, movy, movOX));
    modelMatrix = glm::rotate(modelMatrix,RADIANS(-30),glm::vec3(0,0,1));
    RenderSimpleMesh(meshes["plane"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(),glm::vec3(1), mapTextures["snow"]);
    
    //Draw barriers
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(movx, movy, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -20));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(20, 1, 1));
    RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(0,0,0.6));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(movx, movy, 0));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 20));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(20, 1, 1));
    RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(0,0,0.6));
    mooving = false;
    //Draw player
    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(movx, movy, movOX));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, playerAngle/500, glm::vec3(0, 1, 0));
    modelMatrix = glm::translate(modelMatrix,glm::vec3(0,0.5,0));
    RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["snow2"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(movx, movy, movOX));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, playerAngle / 500, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.8,0.8,0.8));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 1.2, 0));
    RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["snow2"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(movx, movy, movOX));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, playerAngle / 500, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.5, 0.5));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 3, 0));
    RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["snow2"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(movx, movy, movOX));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, playerAngle / 500, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 0.1, 0.5));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 17, 0));
    RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(0));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(movx, movy, movOX));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, playerAngle / 500, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3, 0.3, 0.3));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 6, 0));
    RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(0));

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(movx, movy, movOX));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, 0.2));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, playerAngle / 500, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 0.1, 0.2));
    RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["model"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(movx, movy, movOX));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0, -0.2));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, playerAngle / 500, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(3, 0.1, 0.2));
    RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["model"]);

    modelMatrix = glm::mat4(1);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(movx, movy, movOX));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
    modelMatrix = glm::rotate(modelMatrix, playerAngle / 500, glm::vec3(0, 1, 0));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(0.15, 0.1, 0.1));
    modelMatrix = glm::translate(modelMatrix, glm::vec3(1.8, 15, 0));
    modelMatrix = glm::rotate(modelMatrix, RADIANS(-90), glm::vec3(0, 0, 1));
    RenderSimpleMesh(meshes["cone"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["carrot"]);

    //DrawObstacles
    for (int i = 0; i < rocks.size(); i++)
    {
        for (int j = 0; j < 3; j++)
        {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, rocks[i].rocks[j]);
            modelMatrix = glm::scale(modelMatrix, rocks[i].scala[j]);
            modelMatrix = glm::rotate(modelMatrix, RADIANS(rocks[i].angle[j]), glm::vec3(0, 0, 1));
            RenderSimpleMesh(meshes["sphere"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["rock"]);
        }
    }

    for (int i = 0; i < treees.size(); i++)
    {
        if (treees[i].type == "PINE")
        {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, treees[i].position);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 3, 1));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.3, 0));
            RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["bark1"]);

            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, treees[i].position);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 6, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2,4,2));
            RenderSimpleMesh(meshes["cone"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["pine"]);
        }
        else
        {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, treees[i].position);
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1, 3, 1));
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.3, 0));
            //modelMatrix = glm::scale(modelMatrix, glm::vec3(0.1f));
            RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["bark2"]);
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, treees[i].position);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 3, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(2));
            RenderSimpleMesh(meshes["cone"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["xmastree"]);
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, treees[i].position);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 4, 0));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(1.5));
            RenderSimpleMesh(meshes["cone"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["xmastree"]);
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, treees[i].position);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 5, 0));
            RenderSimpleMesh(meshes["cone"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["xmastree"]);
        }
    }
    for (int i = 0; i < poles.size(); i++)
    {
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, poles[i]);

        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 7, 0.5));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.4, 0));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["steel"]);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, poles[i]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6, 0.6, 4));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 11, 0));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["steel"]);
    }
    for (int i = 0; i < presents.size(); i++)
    {
        if (collected[i] == true)
        {
            continue;
        }
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, presents[i]);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0,0.5,0));
        modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["present1"]);
    }
    for (int i = 0; i < present2.size(); i++)
    {
        if (collected2[i] == false)
        {
            modelMatrix = glm::mat4(1);
            modelMatrix = glm::translate(modelMatrix, present2[i]);
            modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 0.5, 0));
            modelMatrix = glm::rotate(modelMatrix, RADIANS(-30), glm::vec3(0, 0, 1));
            RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["present2"]);
        }
        
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, present2[i]);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 2, -2));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 5, 0.5));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["beam"]);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, present2[i]);
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 2, 2));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.5, 5, 0.5));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["beam"]);
        modelMatrix = glm::mat4(1);
        modelMatrix = glm::translate(modelMatrix, present2[i]);
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.6, 0.6, 7));
        modelMatrix = glm::translate(modelMatrix, glm::vec3(0, 7.1, 0));
        RenderSimpleMesh(meshes["box"], shaders["ShaderTema3"], modelMatrix, camera->GetViewMatrix(), glm::vec3(1), mapTextures["beam"]);
    }

    //GAME MECANICS
    camera->Set(glm::vec3(movx + 12,movy + 5,movOX), glm::vec3(8 + movx, 5 + movy, movOX), glm::vec3(0, 1, 0));
    camera->RotateFirstPerson_OX(RADIANS(-30));
    if (gamestate == 0)
    {
        movx += 4 * deltaTimeSeconds * cos(RADIANS(30)) * cos(playerAngle/500);
        movy -= 4 * deltaTimeSeconds * sin(RADIANS(30)) * cos(playerAngle / 500);
        movOX += 4 * deltaTimeSeconds * sin(-playerAngle / 500);
        CheckObstacle(); 
        CheckColossion();
    }
}


void Tema3::FrameEnd()
{
    //DrawCoordinateSystem(camera->GetViewMatrix(), projectionMatrix);
}


void Tema3::RenderSimpleMesh(Mesh* mesh, Shader* shader, const glm::mat4& modelMatrix, glm::mat4& ViewMatrix, const glm::vec3& color, Texture2D* texture1)
{
    if (!mesh || !shader || !shader->GetProgramID())
        return;

    // Render an object using the specified shader and the specified position
    glUseProgram(shader->program);

    //bind color or Texture
    glActiveTexture(GL_TEXTURE0);
    if (texture1)
    {
        glBindTexture(GL_TEXTURE_2D, texture1->GetTextureID());
        glUniform1i(glGetUniformLocation(shader->program, "texture_1"), 0);
        glActiveTexture(GL_TEXTURE1);
        glUniform1i(glGetUniformLocation(shader->program, "texture"), true);
        glUniform1i(glGetUniformLocation(shader->program, "isGround"), mooving);
        
        glUniform1f(glGetUniformLocation(shader->program, "moveX"), movx/43);
        glUniform1f(glGetUniformLocation(shader->program, "moveY"), movOX/50);
 
    }
    else
    {
        int location = glGetUniformLocation(shader->program, "object_color");
        glUniform3fv(location, 1, glm::value_ptr(color));
        glUniform1i(glGetUniformLocation(shader->program, "texture"), false);
    }
    // Bind model matrix
    GLint loc_model_matrix = glGetUniformLocation(shader->program, "Model");
    glUniformMatrix4fv(loc_model_matrix, 1, GL_FALSE, glm::value_ptr(modelMatrix));

    // Bind view matrix
    glm::mat4 viewMatrix = ViewMatrix;
    int loc_view_matrix = glGetUniformLocation(shader->program, "View");
    glUniformMatrix4fv(loc_view_matrix, 1, GL_FALSE, glm::value_ptr(viewMatrix));

    // Bind projection matrix
    int loc_projection_matrix = glGetUniformLocation(shader->program, "Projection");
    glUniformMatrix4fv(loc_projection_matrix, 1, GL_FALSE, glm::value_ptr(projectionMatrix));

    //light
    glm::vec3 eyePosition = GetSceneCamera()->m_transform->GetWorldPosition();
    int eye_position = glGetUniformLocation(shader->program, "eye_position");
    glUniform3f(eye_position, eyePosition.x, eyePosition.y, eyePosition.z);

    int light_position = glGetUniformLocation(shader->program, "directional");
    glUniform3f(light_position, 0.f, -1.f, 0.f);

    for (int i = 0; i < poles.size(); i++)
    {
        string name = string("lamps[") + to_string(i) + string("]"); 
        light_position = glGetUniformLocation(shader->program, name.c_str());
        glUniform3fv(light_position,1,glm::value_ptr(poles[i]));
    }
    glUniform1i(glGetUniformLocation(shader->program, "size"), poles.size());
    int j = 0;
    for (int i = 0; i < treees.size(); i++)
    {
        string name = string("lights[") + to_string(j) + string("]");
        light_position = glGetUniformLocation(shader->program, name.c_str());
        glUniform3fv(light_position, 1, glm::value_ptr(treees[i].position));
        glm::vec3 color; 
        if (treees[i].type == "PINE")
        {
            color = glm::vec3(0.f, 0.8f, 0);
        }
        else
        {
            color = glm::vec3(0.8f, 0.8f, 0);
        }
        name = string("lightsColor[") + to_string(j) + string("]");
        light_position = glGetUniformLocation(shader->program, name.c_str());
        glUniform3fv(light_position, 1, glm::value_ptr(color));
        j++;
    }
    int offset = 0;
    for (int i = 0; i < presents.size(); i++)
    {
        if (collected[i] == true)
        {
            offset++;
            continue;
        }
        string name = string("lights[") + to_string(j) + string("]");
        light_position = glGetUniformLocation(shader->program, name.c_str());
        glUniform3fv(light_position, 1, glm::value_ptr(presents[i]));
        glm::vec3 color = glm::vec3(0, 0, 0.5f);
        name = string("lightsColor[") + to_string(j) + string("]");
        light_position = glGetUniformLocation(shader->program, name.c_str());
        glUniform3fv(light_position, 1, glm::value_ptr(color));
        j++;
    }

    for (int i = 0; i < present2.size(); i++)
    {
        if (collected2[i] == true)
        {
            offset++;
            continue;
        }
        string name = string("lights[") + to_string(j) + string("]");
        light_position = glGetUniformLocation(shader->program, name.c_str());
        glUniform3fv(light_position, 1, glm::value_ptr(present2[i]));
        glm::vec3 color = glm::vec3(0.8f, 0, 0);
        name = string("lightsColor[") + to_string(j) + string("]");
        light_position = glGetUniformLocation(shader->program, name.c_str());
        glUniform3fv(light_position, 1, glm::value_ptr(color));
        j++;
    }

    glUniform1i(glGetUniformLocation(shader->program, "size2"), treees.size() + presents.size() + present2.size() - offset );

    // Draw the object
    glBindVertexArray(mesh->GetBuffers()->m_VAO);
    glDrawElements(mesh->GetDrawMode(), static_cast<int>(mesh->indices.size()), GL_UNSIGNED_INT, 0);
}

/*
 *  These are callback functions. To find more about callbacks and
 *  how they behave, see `input_controller.h`.
 */


void Tema3::OnInputUpdate(float deltaTime, int mods)
{
    if (!window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        glm::vec3 up = glm::vec3(0, 1, 0);
        glm::vec3 right = GetSceneCamera()->m_transform->GetLocalOXVector();
        glm::vec3 forward = GetSceneCamera()->m_transform->GetLocalOZVector();
        forward = glm::normalize(glm::vec3(forward.x, 0, forward.z));
    }
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT ))
    {
        if (window->KeyHold(GLFW_KEY_W)) 
        {
            camera->MoveForward(deltaTime*2);
        }

        if (window->KeyHold(GLFW_KEY_A)) 
        {
            camera->TranslateRight(-deltaTime*2);
        }

        if (window->KeyHold(GLFW_KEY_S))
        {
            camera->MoveForward(-deltaTime*2);
        }

        if (window->KeyHold(GLFW_KEY_D))
        {
            camera->TranslateRight(deltaTime*2);
        }

        if (window->KeyHold(GLFW_KEY_Q)) 
        {
            camera->TranslateUpward(-deltaTime*2);
        }

        if (window->KeyHold(GLFW_KEY_E)) 
        {
            camera->TranslateUpward(deltaTime*2);
        }
    }
    else
    {
        if (window->KeyHold(GLFW_KEY_W))
        {
            camera->MoveForward(deltaTime);
            camera->TranslateUpward(deltaTime);
        }

        if (window->KeyHold(GLFW_KEY_S))
        {
            camera->MoveForward(-deltaTime);
            camera->TranslateUpward(-deltaTime);
        }
    }

}


void Tema3::OnKeyPress(int key, int mods)
{
    if (gamestate == 1 && key == GLFW_KEY_R)
    {
        gamestate = 0;
        Restart();
    }
}


void Tema3::OnKeyRelease(int key, int mods)
{
    // Add key release event
}


void Tema3::OnMouseMove(int mouseX, int mouseY, int deltaX, int deltaY)
{
    if (window->MouseHold(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float sensivityOX = 0.001f;
        float sensivityOY = 0.001f;

        camera->RotateThirdPerson_OY(-deltaX * sensivityOX);
        camera->RotateThirdPerson_OX(-deltaY * sensivityOY);

    }
    glm::ivec2 resolution = window->GetResolution();
    playerAngle += deltaX; 
}


void Tema3::OnMouseBtnPress(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button press event
}


void Tema3::OnMouseBtnRelease(int mouseX, int mouseY, int button, int mods)
{
    // Add mouse button release event
}


void Tema3::OnMouseScroll(int mouseX, int mouseY, int offsetX, int offsetY)
{
}


void Tema3::OnWindowResize(int width, int height)
{
}
