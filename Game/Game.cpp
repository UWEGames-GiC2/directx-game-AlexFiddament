//
// Game.cpp
//

#include "pch.h"
#include "Game.h"
#include <time.h>

#include <iostream>

//Scarle Headers
#include "GameData.h"
#include "GameState.h"
#include "DrawData.h"
#include "DrawData2D.h"
#include "ObjectList.h"

#include "CMOGO.h"
#include <DirectXCollision.h>
#include "Collision.h"

extern void ExitGame() noexcept;

using namespace DirectX;

using Microsoft::WRL::ComPtr;

Game::Game() noexcept :
    m_window(nullptr),
    m_outputWidth(800),
    m_outputHeight(600),
    m_featureLevel(D3D_FEATURE_LEVEL_11_0)
{
}

// Initialize the Direct3D resources required to run.
void Game::Initialize(HWND _window, int _width, int _height)
{
    m_window = _window;
    m_outputWidth = std::max(_width, 1);
    m_outputHeight = std::max(_height, 1);

    CreateDevice();

    CreateResources();

    // TODO: Change the timer settings if you want something other than the default variable timestep mode.
    // e.g. for 60 FPS fixed timestep update logic, call:
    /*
    m_timer.SetFixedTimeStep(true);
    m_timer.SetTargetElapsedSeconds(1.0 / 60);
    */

    //seed the random number generator
    srand((UINT)time(NULL));

    //set up keyboard and mouse system
    //documentation here: https://github.com/microsoft/DirectXTK/wiki/Mouse-and-keyboard-input
    m_keyboard = std::make_unique<Keyboard>();
    m_mouse = std::make_unique<Mouse>();
    m_mouse->SetWindow(_window);
    m_mouse->SetMode(Mouse::MODE_RELATIVE);
    //Hide the mouse pointer
    ShowCursor(false);

    //create GameData struct and populate its pointers
    m_GD = new GameData;
    m_GD->m_GS = GS_Menu;

    //set up systems for 2D rendering
    m_DD2D = new DrawData2D();
    m_DD2D->m_Sprites.reset(new SpriteBatch(m_d3dContext.Get()));
    m_DD2D->m_Font.reset(new SpriteFont(m_d3dDevice.Get(), L"..\\Assets\\italic.spritefont"));
    m_states = new CommonStates(m_d3dDevice.Get());

    //set up DirectXTK Effects system
    m_fxFactory = new EffectFactory(m_d3dDevice.Get());
    //Tell the fxFactory to look to the correct build directory to pull stuff in from
    ((EffectFactory*)m_fxFactory)->SetDirectory(L"..\\Assets");
    //init render system for VBGOs
    VBGO::Init(m_d3dDevice.Get());

    //set audio system
    AUDIO_ENGINE_FLAGS eflags = AudioEngine_Default;
#ifdef _DEBUG
    eflags = eflags | AudioEngine_Debug;
#endif
    m_audioEngine = std::make_unique<AudioEngine>(eflags);

    //create a set of dummy things to show off the engine

    //create a base light
    auto m_light = std::make_shared<Light>(Vector3(0.0f, 100.0f, 160.0f), Color(1.0f, 1.0f, 1.0f, 1.0f), Color(0.4f, 0.1f, 0.1f, 1.0f));
    m_GameObjects.push_back(m_light);

    //find how big my window is to correctly calculate my aspect ratio
    float AR = (float)_width / (float)_height;



   

   


    //example basic 3D stuff
    

    
    std::shared_ptr<Terrain> terrain0 = std::make_shared<Terrain>("Floor", m_d3dDevice.Get(), m_fxFactory, Vector3(0.0f, 50.0f, -70.0f), 0.0f, 0.0f, 0.0f, 4.5f * Vector3::One);
    m_GameObjects.push_back(terrain0);
    m_ColliderObjects.push_back(terrain0);

    std::shared_ptr<Terrain> terrain = std::make_shared<Terrain>("Floor", m_d3dDevice.Get(), m_fxFactory, Vector3(0.0f, 0.0f, -70.0f), 0.0f, 0.0f, 0.0f, 4.5f * Vector3::One);
    m_GameObjects.push_back(terrain);
    m_ColliderObjects.push_back(terrain);

    std::shared_ptr<Terrain> terrain1 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0f, 0.0f, 100.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain1);
    m_ColliderObjects.push_back(terrain1);

    std::shared_ptr<Terrain> terrain2 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0f, 0.0f, 100.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain2);
    m_ColliderObjects.push_back(terrain2);

    std::shared_ptr<Terrain> terrain3 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-50.0f, 0.0f, 100.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain3);
    m_ColliderObjects.push_back(terrain3);

    std::shared_ptr<Terrain> terrain4 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-50.0f, 0.0f, 100.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain4);
    m_ColliderObjects.push_back(terrain4);

    std::shared_ptr<Terrain> terrain5 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(175.0F, 0.0f, 100.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain5);
    m_ColliderObjects.push_back(terrain5);

    std::shared_ptr<Terrain> terrain6 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(175.0F, 0.0f, 25.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain6);
    m_ColliderObjects.push_back(terrain6);

    std::shared_ptr<Terrain> terrain7 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(175.0F, 0.0f, -50.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain7);
    m_ColliderObjects.push_back(terrain7);

    std::shared_ptr<Terrain> terrain8 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(175.0F, 0.0f, -125.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain8);
    m_ColliderObjects.push_back(terrain8);

    std::shared_ptr<Terrain> terrain9 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(175.0F, 0.0f, -200.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain9);
    m_ColliderObjects.push_back(terrain9);

    std::shared_ptr<Terrain> terrain10 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(175.0F, 0.0f, -275.0f), 0.0f, XMConvertToRadians(180), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain10);
    m_ColliderObjects.push_back(terrain10);

    std::shared_ptr<Terrain> terrain11 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0F, 0.0f, -275.0f), 0.0f, XMConvertToRadians(180), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain11);
    m_ColliderObjects.push_back(terrain11);

    std::shared_ptr<Terrain> terrain12 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0F, 0.0f, -275.0f), 0.0f, XMConvertToRadians(180), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain12);
    m_ColliderObjects.push_back(terrain12);

    std::shared_ptr<Terrain> terrain13 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-50.0F, 0.0f, -275.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain13);
    m_ColliderObjects.push_back(terrain13);

    std::shared_ptr<Terrain> terrain15 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-50.0F, 0.0f, -50.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain15);
    m_ColliderObjects.push_back(terrain15);

    std::shared_ptr<Terrain> terrain16 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-50.0F, 0.0f, 25.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain16);
    m_ColliderObjects.push_back(terrain16);

    std::shared_ptr<Terrain> terrain17 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-50.0F, 0.0f, -125.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain17);
    m_ColliderObjects.push_back(terrain17);

    std::shared_ptr<Terrain> terrain18 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-50.0F, 0.0f, -200.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain18);
    m_ColliderObjects.push_back(terrain18);

    std::shared_ptr<Terrain> terrain19 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0f, 25.0f, 25.0f), 0.0f, XMConvertToRadians(90), XMConvertToRadians(180), 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain19);
    m_ColliderObjects.push_back(terrain19);

    std::shared_ptr<Terrain> terrain20 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-50.0f, 0.0f, 25.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain20);
    m_ColliderObjects.push_back(terrain20);

    std::shared_ptr<Terrain> terrain21 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0f, 0.0f, 25.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain21);
    m_ColliderObjects.push_back(terrain21);

    std::shared_ptr<Terrain> terrain22 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0f, 0.0f, -50.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain22);
    m_ColliderObjects.push_back(terrain22);

    std::shared_ptr<Terrain> terrain23 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0f, 25.0f, -50.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain23);
    m_ColliderObjects.push_back(terrain23);

    std::shared_ptr<Terrain> terrain25 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-50.0f, 25.0f, -50.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain25);
    m_ColliderObjects.push_back(terrain25);

    std::shared_ptr<Terrain> terrain26 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0f, 25.0f, -125.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain26);
    m_ColliderObjects.push_back(terrain26);

    std::shared_ptr<Terrain> terrain27 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0f, 25.0f, -125.0f), 0.0f, 0.0f, XMConvertToRadians(180), 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain27);
    m_ColliderObjects.push_back(terrain27);

    std::shared_ptr<Terrain> terrain28 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0F, 0.0f, -125.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain28);
    m_ColliderObjects.push_back(terrain28);

    std::shared_ptr<Terrain> terrain29 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0F, 0.0f, -50.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain29);
    m_ColliderObjects.push_back(terrain29);

    std::shared_ptr<Terrain> terrain30 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0F, 0.0f, 25.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain30);
    m_ColliderObjects.push_back(terrain30);

    std::shared_ptr<Terrain> terrain31 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0F, 0.0f, -125.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain31);
    m_ColliderObjects.push_back(terrain31);

    std::shared_ptr<Terrain> terrain32 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0F, 0.0f, -50.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain32);
    m_ColliderObjects.push_back(terrain32);

    std::shared_ptr<Terrain> terrain33 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0f, 25.0f, -200.0f), 0.0f, 0.0f, XMConvertToRadians(180), 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain33);
    m_ColliderObjects.push_back(terrain33);

    


    std::shared_ptr<Terrain> target = std::make_shared<Terrain>("glass cube", m_d3dDevice.Get(), m_fxFactory, Vector3(135.0f, 30.0f, 60.0f), 0.0f, 0.0f, 0.0f, 0.07f * Vector3::One);
    m_GameObjects.push_back(target);
    m_TargetObjects_points.push_back(target);

    std::shared_ptr<Terrain> target1 = std::make_shared<Terrain>("glass cube", m_d3dDevice.Get(), m_fxFactory, Vector3(135.0f, 30.0f, -160.0f), 0.0f, 0.0f, 0.0f, 0.07f * Vector3::One);
    m_GameObjects.push_back(target1);
    m_TargetObjects_points.push_back(target1);

    std::shared_ptr<Terrain> targetmove = std::make_shared<Terrain>("glass cube", m_d3dDevice.Get(), m_fxFactory, Vector3(60.0f, 30.0f, 60.0f), 0.0f, 0.0f, 0.0f, 0.07f * Vector3::One);
    m_GameObjects.push_back(targetmove);
    m_TargetObjects_move_wall.push_back(targetmove);

    std::shared_ptr<Terrain> targetmove1 = std::make_shared<Terrain>("glass cube", m_d3dDevice.Get(), m_fxFactory, Vector3(50.0f, 30.0f, -10.0f), 0.0f, 0.0f, 0.0f, 0.07f * Vector3::One);
    m_GameObjects.push_back(targetmove1);
    m_TargetObjects_move_wall.push_back(targetmove1);


    std::shared_ptr<Terrain> targetmove2 = std::make_shared<Terrain>("glass cube", m_d3dDevice.Get(), m_fxFactory, Vector3(-10.0f, 30.0f, -10.0f), 0.0f, 0.0f, 0.0f, 0.07f * Vector3::One);
    m_GameObjects.push_back(targetmove2);
    m_TargetObjects_move_wall.push_back(targetmove2);


    std::shared_ptr<Terrain> targetmove3 = std::make_shared<Terrain>("glass cube", m_d3dDevice.Get(), m_fxFactory, Vector3(60.0f, 30.0f, -80.0f), 0.0f, 0.0f, 0.0f, 0.07f * Vector3::One);
    m_GameObjects.push_back(targetmove3);
    m_TargetObjects_move_wall.push_back(targetmove3);

    

    std::shared_ptr<Terrain> terrainmove = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0f, 0.0f, -200.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrainmove);
    m_ColliderObjects.push_back(terrainmove);



    std::shared_ptr<Terrain> terrain34 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0F, 0.0f, 100.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain34);
    m_ColliderObjects.push_back(terrain34);

    std::shared_ptr<Terrain> terrainmove1 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(25.0f, 0.0f, -200.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrainmove1);
    m_ColliderObjects.push_back(terrainmove1);
   


    std::shared_ptr<Terrain> end = std::make_shared<Terrain>("glass cube", m_d3dDevice.Get(), m_fxFactory, Vector3(- 100.0f, 30.0f, 65.0f), 0.0f, 0.0f, 0.0f, 0.07f * Vector3::One);
    m_GameObjects.push_back(end);
    m_WinObject.push_back(end);

    std::shared_ptr<Terrain> terrain35 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-50.0f, 25.0f, -200.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain35);
    m_ColliderObjects.push_back(terrain35);

    std::shared_ptr<Terrain> terrain36 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(100.0f, 25.0f, -125.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain36);
    m_ColliderObjects.push_back(terrain36);

    std::shared_ptr<Terrain> terrain37 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-125.0F, 0.0f, 100.0f), 0.0f, XMConvertToRadians(90), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain37);
    m_ColliderObjects.push_back(terrain37);

    std::shared_ptr<Terrain> terrain38 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-50.0F, 0.0f, 100.0f), 0.0f, XMConvertToRadians(180), 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain38);
    m_ColliderObjects.push_back(terrain38);

    std::shared_ptr<Terrain> terrain39 = std::make_shared<Terrain>("Bluestone wall", m_d3dDevice.Get(), m_fxFactory, Vector3(-125.0F, 0.0f, 25.0f), 0.0f, 0.0f, 0.0f, 0.20f * Vector3::One);
    m_GameObjects.push_back(terrain39);
    m_ColliderObjects.push_back(terrain39);
   

   
    

   
   
    std::string livesStr = std::to_string(lives);
  
    auto text = std::make_shared<TextGO2D>("lives = " + livesStr);

    text->SetPos(Vector2(450, 10));
    text->SetColour(Color((float*)&Colors::Yellow));
    m_GameObjects2D.push_back(text);

    std::string scoreStr = std::to_string(score);

    
    auto text1 = std::make_shared<TextGO2D>("lives = " + scoreStr);
    text1->SetPos(Vector2(440, 50));
    text1->SetColour(Color((float*)&Colors::Yellow));
    m_GameObjects2D.push_back(text1);

    

    
    
    
    

    

    

   
    

   
    

  
    for (size_t i = 0; i < 10; i++)
    {
        auto pProjectile = std::make_shared<Projectiles>("Ball FBX", m_d3dDevice.Get(), m_fxFactory);
        pProjectile->SetActive(false);
        m_GameObjects.push_back(pProjectile);
        m_PlayerProjectile.push_back(pProjectile);

    }


    //add Player
    auto pPlayer = std::make_shared<Player>("BirdModelV1", m_d3dDevice.Get(), m_fxFactory);
    pPlayer->SetScale(0.40f, 0.40f, 0.40f);
    m_GameObjects.push_back(pPlayer);
    m_PlayerObject.push_back(pPlayer);
    pPlayer->SetPos(Vector3(0.0F, 30.0f, 50.0f));
    pPlayer->Projectiles = m_PlayerProjectile;
    pPlayer->SetPitchYawRoll(0.0F, XMConvertToRadians(90), 0.0f);
   

    spawn = Vector3(0.0F, 30.0f, 50.0f);
    



    //create a base camera
    std::shared_ptr<Camera> m_cam = m_cam = std::make_shared<Camera>(0.25f * XM_PI, AR, 1.0f, 10000.0f, Vector3::UnitY, Vector3::Zero);
    m_cam->SetPos(Vector3(50.0f, 5000.0f, 5000.0f));
    m_GameObjects.push_back(m_cam);

    //add a secondary camera
    std::shared_ptr<Camera> m_TPScam =  m_TPScam = std::make_shared<TPSCamera>(0.25f * XM_PI, AR, 1.0f, 10000.0f, pPlayer, Vector3::UnitY, Vector3(50.0f, 50000.0f, 50000.0f));
    m_GameObjects.push_back(m_TPScam);

   //m_firstpersoncam = new firstpersoncam(0.25f * XM_PI, AR, 1.0f, 10000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 0.0f, 0.0f));

   std::shared_ptr<firstpersoncam> m_fpscam = std::make_shared<firstpersoncam>(0.25f * XM_PI, AR, 1.0f, 10000.0f, pPlayer, Vector3::UnitY, Vector3(0.0f, 0.0f, 0.0f));

   m_GameObjects.push_back(m_fpscam);

    

   
    
    
    if (GS_GAME_OVER)
    {
        
    }

  


    

    //create DrawData struct and populate its pointers
    m_DD = new DrawData;
    m_DD->m_pd3dImmediateContext = nullptr;
    m_DD->m_states = m_states;
    m_DD->m_cam = m_cam;
    m_DD->m_light = m_light;

    
}

// Executes the basic game loop.
void Game::Tick()
{
    m_timer.Tick([&]()
    {
        Update(m_timer);
    });

    Render();
}

// Updates the world.
void Game::Update(DX::StepTimer const& _timer)
{
    float elapsedTime = float(_timer.GetElapsedSeconds());
    m_GD->m_dt = elapsedTime;

    //this will update the audio engine but give us chance to do somehting else if that isn't working
    if (!m_audioEngine->Update())
    {
        if (m_audioEngine->IsCriticalError())
        {
            // We lost the audio device!
        }
    }
    else
    {
        //update sounds playing
        for (list<Sound*>::iterator it = m_Sounds.begin(); it != m_Sounds.end(); it++)
        {
            (*it)->Tick(m_GD);
        }
    }

    ReadInput();
    //upon space bar switch camera state
    //see docs here for what's going on: https://github.com/Microsoft/DirectXTK/wiki/Keyboard
    

    //update all objects
    for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
    {
        (*it)->Tick(m_GD);
    }
    for (list<GameObject2D*>::iterator it = m_GameObjects2D.begin(); it != m_GameObjects2D.end(); it++)
    {
        (*it)->Tick(m_GD);
    }

    CheckCollision();
    CheckProjectileCollision();
    CheckPlayerCollision();
    ChecktargetpointsCollision();
    ChecktargetmoveCollision();
    CheckWinCollision();
    screenupdater();

    if (lives == 0)
    {
        m_GD->m_GS = GS_GAME_OVER;
    }
    if (move && mover == 0)
    {

        m_ColliderObjects[34]->SetActive(false);


        move = false;
        mover = 1;
        printf("%d\n", mover);

    }
    if (move && mover == 1)
    {


        m_ColliderObjects[33]->SetActive(false);
        move = false;
        mover = 2;




    }
    if (move && mover == 2)
    {

        m_ColliderObjects[35]->SetActive(false);

        move = false;
        mover = 3;
        printf("%d\n", mover);
        spawn = Vector3(-10.0f, 30.0f, -10.0f);

    }
    if (move && mover == 3)
    {

        m_ColliderObjects[5]->SetActive(false);
        move = false;
        mover = 4;
        printf("%d\n", mover);

    }
    if (m_GD->m_GS == GS_Menu)
    {
        updatescreen();
        auto text = std::make_shared<TextGO2D>("welcome to the game\nnaivgate the changing maze but you are constanly falling!\nuse lmb to give a boost in height and space to shoot\nmake sure you dont hit the walls or you lose a life\nloose all three and your out!\nshoot the boxes to gain points\nsome will move walls some will only give points\nonly once you have six points the final wall will open\nmake it back to the start to win\ngood luck!  ");
        text->SetPos(Vector2(50, 100));
        text->SetColour(Color((float*)&Colors::Black));
        text->SetScale(0.7f);

        auto text1 = std::make_shared<TextGO2D>(" Press Enter to start ");
        text1->SetPos(Vector2(350, 470));
        text1->SetColour(Color((float*)&Colors::Black));
        
        m_GameObjects2D.push_back(text);
        m_GameObjects2D.push_back(text1);
        m_PlayerObject[0]->SetPos(Vector3(0.0F, 30.0f, 50.0f));
        m_PlayerObject[0]->SetAcceleration(Vector3(0.0F, 0.0f, 0.0f));
        if (m_GD->m_KBS.Enter)

        {
            m_GD->m_GS = GS_PLAY_FIRST_PERSON_CAM;
            updatescreen();

        }
    }


    if (m_GD->m_GS == GS_GAME_OVER)
    {
        updatescreen();
        auto text = std::make_shared<TextGO2D>("you lose! \npress space to play again?");
        text->SetPos(Vector2(300, 300));
        text->SetColour(Color((float*)&Colors::Black));
        m_GameObjects2D.push_back(text);
        m_PlayerObject[0]->SetPos(Vector3(0.0F, 30.0f, 50.0f));
        m_PlayerObject[0]->SetAcceleration(Vector3(0.0F, 0.0f, 0.0f));
        



        if (m_GD->m_KBS.Space)

        {
            m_GD->m_GS = GS_Menu;
            m_GameObjects2D.clear();
            lives = 3;
            score = 0;
            updatescreen();

            for (int i = 0; i < m_TargetObjects_move_wall.size(); i++)
            {
                m_TargetObjects_move_wall[i]->SetActive(true);
            }

            for (int i = 0; i < m_TargetObjects_points.size(); i++)
            {
                m_TargetObjects_points[i]->SetActive(true);
            }

            for (int i = 0; i < m_ColliderObjects.size(); i++)
            {
                m_ColliderObjects[i]->SetActive(true);
            }

            mover = 0;
            spawn = Vector3(0.0F, 30.0f, 50.0f);




        }

    }

    if (m_GD->m_GS == GS_WIN)
    {
        updatescreen();
        auto text = std::make_shared<TextGO2D>("You Win! \npress space to play again?");
        text->SetPos(Vector2(300, 300));
        text->SetColour(Color((float*)&Colors::Black));
        m_GameObjects2D.push_back(text);
        m_PlayerObject[0]->SetPos(Vector3(0.0F, 30.0f, 50.0f));
        m_PlayerObject[0]->SetAcceleration(Vector3(0.0F, 0.0f, 0.0f));




        if (m_GD->m_KBS.Space)

        {
            m_GD->m_GS = GS_Menu;
            m_GameObjects2D.clear();
            lives = 3;
            score = 0;
            updatescreen();

            for (int i = 0; i < m_TargetObjects_move_wall.size(); i++)
            {
                m_TargetObjects_move_wall[i]->SetActive(true);
            }

            for (int i = 0; i < m_TargetObjects_points.size(); i++)
            {
                m_TargetObjects_points[i]->SetActive(true);
            }

            for (int i = 0; i < m_ColliderObjects.size(); i++)
            {
                m_ColliderObjects[i]->SetActive(true);
            }

            mover = 0;
            spawn = Vector3(0.0F, 30.0f, 50.0f);


        }

    }




    

}

// Draws the scene.
void Game::Render()
{
    // Don't try to render anything before the first Update.
    if (m_timer.GetFrameCount() == 0)
    {
        return;
    }

    Clear();
    
    //set immediate context of the graphics device
    m_DD->m_pd3dImmediateContext = m_d3dContext.Get();

    //set which camera to be used
    m_DD->m_cam = m_cam;

    
    
     m_DD->m_cam = m_firstpersoncam;
    

    //update the constant buffer for the rendering of VBGOs
    VBGO::UpdateConstantBuffer(m_DD);

    //Draw 3D Game Obejects
    for (list<GameObject*>::iterator it = m_GameObjects.begin(); it != m_GameObjects.end(); it++)
    {
        if ((*it)->isactive())
        {

            (*it)->Draw(m_DD);

        }
        
    }

    // Draw sprite batch stuff 
    m_DD2D->m_Sprites->Begin(SpriteSortMode_Deferred, m_states->NonPremultiplied());
    for (list<GameObject2D*>::iterator it = m_GameObjects2D.begin(); it != m_GameObjects2D.end(); it++)
    {



        (*it)->Draw(m_DD2D);

    }

    m_DD2D->m_Sprites->End();

    //drawing text screws up the Depth Stencil State, this puts it back again!
    m_d3dContext->OMSetDepthStencilState(m_states->DepthDefault(), 0);

    if (m_GD->m_GS == GS_GAME_OVER)
    {
        
        auto text = std::make_shared<TextGO2D>("You lose!");
        text->SetPos(Vector2(300, 300));
        text->SetColour(Color((float*)&Colors::Black));
        m_GameObjects2D.push_back(text);
        m_PlayerObject[0]->SetPos(Vector3(0.0F, 30.0f, 50.0f));
        m_PlayerObject[0]->SetAcceleration(Vector3(0.0F, 0.0f, 0.0f));

        

        if (m_GD->m_KBS.P)

        {
            m_GD->m_GS = GS_Menu;
            m_GameObjects2D.clear();
            lives = 3;
            score = 0;
            updatescreen();

            for (int i = 0; i < m_TargetObjects_move_wall.size(); i++)
            {
                m_TargetObjects_move_wall[i]->SetActive(true);
            }

            for (int i = 0; i < m_TargetObjects_points.size(); i++)
            {
                m_TargetObjects_points[i]->SetActive(true);
            }

            for (int i = 0; i < m_ColliderObjects.size(); i++)
            {
                m_ColliderObjects[i]->SetActive(true);
            }

            mover = 0;
            spawn = Vector3(0.0F, 30.0f, 50.0f);
            



        }

    }

    if (m_GD->m_GS == GS_WIN)
    {
        m_GameObjects2D.clear();
        auto text = std::make_shared<TextGO2D>("You Win! \npress enter to play again?");
        text->SetPos(Vector2(300, 300));
        text->SetColour(Color((float*)&Colors::Black));
        m_GameObjects2D.push_back(text);
        m_PlayerObject[0]->SetPos(Vector3(0.0F, 30.0f, 50.0f));
        m_PlayerObject[0]->SetAcceleration(Vector3(0.0F, 0.0f, 0.0f));
        


       
        if (m_GD->m_KBS.P)

        {
            m_GD->m_GS = GS_Menu;
            m_GameObjects2D.clear();
            lives = 3;
            score = 0;
            updatescreen();

            for (int i = 0; i < m_TargetObjects_move_wall.size(); i++)
            {
                m_TargetObjects_move_wall[i]->SetActive(true);
            }

            for (int i = 0; i < m_TargetObjects_points.size(); i++)
            {
                m_TargetObjects_points[i]->SetActive(true);
            }

            for (int i = 0; i < m_ColliderObjects.size(); i++)
            {
                m_ColliderObjects[i]->SetActive(true);
            }

            mover = 0;
            spawn = Vector3(0.0F, 30.0f, 50.0f);
            
           
        }

    }

    

    Present();
}

// Helper method to clear the back buffers.
void Game::Clear()
{
    // Clear the views.
    m_d3dContext->ClearRenderTargetView(m_renderTargetView.Get(), Colors::CornflowerBlue);
    m_d3dContext->ClearDepthStencilView(m_depthStencilView.Get(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);

    m_d3dContext->OMSetRenderTargets(1, m_renderTargetView.GetAddressOf(), m_depthStencilView.Get());

    // Set the viewport.
    CD3D11_VIEWPORT viewport(0.0f, 0.0f, static_cast<float>(m_outputWidth), static_cast<float>(m_outputHeight));
    m_d3dContext->RSSetViewports(1, &viewport);
}

// Presents the back buffer contents to the screen.
void Game::Present()
{
    // The first argument instructs DXGI to block until VSync, putting the application
    // to sleep until the next VSync. This ensures we don't waste any cycles rendering
    // frames that will never be displayed to the screen.
    HRESULT hr = m_swapChain->Present(1, 0);

    // If the device was reset we must completely reinitialize the renderer.
    if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
    {
        OnDeviceLost();
    }
    else
    {
        DX::ThrowIfFailed(hr);
    }
}

// Message handlers
void Game::OnActivated()
{
    // TODO: Game is becoming active window.
}

void Game::OnDeactivated()
{
    // TODO: Game is becoming background window.
}

void Game::OnSuspending()
{
    // TODO: Game is being power-suspended (or minimized).
}

void Game::OnResuming()
{
    m_timer.ResetElapsedTime();

    // TODO: Game is being power-resumed (or returning from minimize).
}

void Game::OnWindowSizeChanged(int _width, int _height)
{
    m_outputWidth = std::max(_width, 1);
    m_outputHeight = std::max(_height, 1);

    CreateResources();

    // TODO: Game window is being resized.
}

// Properties
void Game::GetDefaultSize(int& _width, int& _height) const noexcept
{
    // TODO: Change to desired default window size (note minimum size is 320x200).
    _width = 800;
    _height = 600;
}

// These are the resources that depend on the device.
void Game::CreateDevice()
{
    UINT creationFlags = 0;

#ifdef _DEBUG
    //creationFlags |= D3D11_CREATE_DEVICE_DEBUG;
    //something missing on the machines in 2Q28
    //this should work!
#endif

    static const D3D_FEATURE_LEVEL featureLevels [] =
    {
        // TODO: Modify for supported Direct3D feature levels
        D3D_FEATURE_LEVEL_11_1,
        D3D_FEATURE_LEVEL_11_0,
        D3D_FEATURE_LEVEL_10_1,
        D3D_FEATURE_LEVEL_10_0,
        D3D_FEATURE_LEVEL_9_3,
        D3D_FEATURE_LEVEL_9_2,
        D3D_FEATURE_LEVEL_9_1,
    };

    // Create the DX11 API device object, and get a corresponding context.
    ComPtr<ID3D11Device> device;
    ComPtr<ID3D11DeviceContext> context;
    DX::ThrowIfFailed(D3D11CreateDevice(
        nullptr,                            // specify nullptr to use the default adapter
        D3D_DRIVER_TYPE_HARDWARE,
        nullptr,
        creationFlags,
        featureLevels,
        static_cast<UINT>(std::size(featureLevels)),
        D3D11_SDK_VERSION,
        device.ReleaseAndGetAddressOf(),    // returns the Direct3D device created
        &m_featureLevel,                    // returns feature level of device created
        context.ReleaseAndGetAddressOf()    // returns the device immediate context
        ));

#ifndef NDEBUG
    ComPtr<ID3D11Debug> d3dDebug;
    if (SUCCEEDED(device.As(&d3dDebug)))
    {
        ComPtr<ID3D11InfoQueue> d3dInfoQueue;
        if (SUCCEEDED(d3dDebug.As(&d3dInfoQueue)))
        {
#ifdef _DEBUG
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_CORRUPTION, true);
            d3dInfoQueue->SetBreakOnSeverity(D3D11_MESSAGE_SEVERITY_ERROR, true);
#endif
            D3D11_MESSAGE_ID hide [] =
            {
                D3D11_MESSAGE_ID_SETPRIVATEDATA_CHANGINGPARAMS,
                // TODO: Add more message IDs here as needed.
            };
            D3D11_INFO_QUEUE_FILTER filter = {};
            filter.DenyList.NumIDs = static_cast<UINT>(std::size(hide));
            filter.DenyList.pIDList = hide;
            d3dInfoQueue->AddStorageFilterEntries(&filter);
        }
    }
#endif

    DX::ThrowIfFailed(device.As(&m_d3dDevice));
    DX::ThrowIfFailed(context.As(&m_d3dContext));

    // TODO: Initialize device dependent objects here (independent of window size).
}

// Allocate all memory resources that change on a window SizeChanged event.
void Game::CreateResources()
{
    // Clear the previous window size specific context.
    ID3D11RenderTargetView* nullViews [] = { nullptr };
    m_d3dContext->OMSetRenderTargets(static_cast<UINT>(std::size(nullViews)), nullViews, nullptr);
    m_renderTargetView.Reset();
    m_depthStencilView.Reset();
    m_d3dContext->Flush();

    const UINT backBufferWidth = static_cast<UINT>(m_outputWidth);
    const UINT backBufferHeight = static_cast<UINT>(m_outputHeight);
    const DXGI_FORMAT backBufferFormat = DXGI_FORMAT_B8G8R8A8_UNORM;
    const DXGI_FORMAT depthBufferFormat = DXGI_FORMAT_D24_UNORM_S8_UINT;
    constexpr UINT backBufferCount = 2;

    // If the swap chain already exists, resize it, otherwise create one.
    if (m_swapChain)
    {
        HRESULT hr = m_swapChain->ResizeBuffers(backBufferCount, backBufferWidth, backBufferHeight, backBufferFormat, 0);

        if (hr == DXGI_ERROR_DEVICE_REMOVED || hr == DXGI_ERROR_DEVICE_RESET)
        {
            // If the device was removed for any reason, a new device and swap chain will need to be created.
            OnDeviceLost();

            // Everything is set up now. Do not continue execution of this method. OnDeviceLost will reenter this method 
            // and correctly set up the new device.
            return;
        }
        else
        {
            DX::ThrowIfFailed(hr);
        }
    }
    else
    {
        // First, retrieve the underlying DXGI Device from the D3D Device.
        ComPtr<IDXGIDevice1> dxgiDevice;
        DX::ThrowIfFailed(m_d3dDevice.As(&dxgiDevice));

        // Identify the physical adapter (GPU or card) this device is running on.
        ComPtr<IDXGIAdapter> dxgiAdapter;
        DX::ThrowIfFailed(dxgiDevice->GetAdapter(dxgiAdapter.GetAddressOf()));

        // And obtain the factory object that created it.
        ComPtr<IDXGIFactory2> dxgiFactory;
        DX::ThrowIfFailed(dxgiAdapter->GetParent(IID_PPV_ARGS(dxgiFactory.GetAddressOf())));

        // Create a descriptor for the swap chain.
        DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
        swapChainDesc.Width = backBufferWidth;
        swapChainDesc.Height = backBufferHeight;
        swapChainDesc.Format = backBufferFormat;
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
        swapChainDesc.BufferCount = backBufferCount;

        DXGI_SWAP_CHAIN_FULLSCREEN_DESC fsSwapChainDesc = {};
        fsSwapChainDesc.Windowed = TRUE;

        // Create a SwapChain from a Win32 window.
        DX::ThrowIfFailed(dxgiFactory->CreateSwapChainForHwnd(
            m_d3dDevice.Get(),
            m_window,
            &swapChainDesc,
            &fsSwapChainDesc,
            nullptr,
            m_swapChain.ReleaseAndGetAddressOf()
            ));

        // This template does not support exclusive fullscreen mode and prevents DXGI from responding to the ALT+ENTER shortcut.
        DX::ThrowIfFailed(dxgiFactory->MakeWindowAssociation(m_window, DXGI_MWA_NO_ALT_ENTER));
    }

    // Obtain the backbuffer for this window which will be the final 3D rendertarget.
    ComPtr<ID3D11Texture2D> backBuffer;
    DX::ThrowIfFailed(m_swapChain->GetBuffer(0, IID_PPV_ARGS(backBuffer.GetAddressOf())));

    // Create a view interface on the rendertarget to use on bind.
    DX::ThrowIfFailed(m_d3dDevice->CreateRenderTargetView(backBuffer.Get(), nullptr, m_renderTargetView.ReleaseAndGetAddressOf()));

    // Allocate a 2-D surface as the depth/stencil buffer and
    // create a DepthStencil view on this surface to use on bind.
    CD3D11_TEXTURE2D_DESC depthStencilDesc(depthBufferFormat, backBufferWidth, backBufferHeight, 1, 1, D3D11_BIND_DEPTH_STENCIL);

    ComPtr<ID3D11Texture2D> depthStencil;
    DX::ThrowIfFailed(m_d3dDevice->CreateTexture2D(&depthStencilDesc, nullptr, depthStencil.GetAddressOf()));

    CD3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc(D3D11_DSV_DIMENSION_TEXTURE2D);
    DX::ThrowIfFailed(m_d3dDevice->CreateDepthStencilView(depthStencil.Get(), &depthStencilViewDesc, m_depthStencilView.ReleaseAndGetAddressOf()));

    // TODO: Initialize windows-size dependent objects here.
}

void Game::OnDeviceLost()
{
    // TODO: Add Direct3D resource cleanup here.

    m_depthStencilView.Reset();
    m_renderTargetView.Reset();
    m_swapChain.Reset();
    m_d3dContext.Reset();
    m_d3dDevice.Reset();

    CreateDevice();

    CreateResources();
}

void Game::ReadInput()
{
    m_GD->m_KBS = m_keyboard->GetState();
    m_GD->m_KBS_tracker.Update(m_GD->m_KBS);
    //quit game on hiting escape
    if (m_GD->m_KBS.Escape)
    {
        ExitGame();
    }

    if (m_GD->m_KBS.Q)
    {
        ExitGame();
    }

    


    m_GD->m_MS = m_mouse->GetState();

    //lock the cursor to the centre of the window
    RECT window;
    GetWindowRect(m_window, &window);
    SetCursorPos((window.left + window.right) >> 1, (window.bottom + window.top) >> 1);
}

void Game::CheckCollision()
{
    for (int i = 0; i < m_PhysicsObjects.size(); i++) for (int j = 0; j < m_ColliderObjects.size(); j++)
    {
        if (m_PhysicsObjects[i]->Intersects(*m_ColliderObjects[j])) 
        {
            XMFLOAT3 eject_vect = Collision::ejectionCMOGO(*m_PhysicsObjects[i], *m_ColliderObjects[j]);
            auto pos = m_PhysicsObjects[i]->GetPos();
            m_PhysicsObjects[i]->SetPos(pos - eject_vect);
            updatescreen();
        }
    }
}

void Game::CheckProjectileCollision()
{
    for (int i = 0; i < m_PlayerProjectile.size(); i++) for (int j = 0; j < m_ColliderObjects.size(); j++)
    {
        if (m_PlayerProjectile[i]->isactive() && m_ColliderObjects[j]->isactive() && m_PlayerProjectile[i]->Intersects(*m_ColliderObjects[j]))
        {
            m_PlayerProjectile[i]->SetActive(false);
            updatescreen();
        }
    }
}

void Game::CheckPlayerCollision()
{
    for (int i = 0; i < m_PlayerObject.size(); i++) for (int j = 0; j < m_ColliderObjects.size(); j++)
    {
        if (m_ColliderObjects[j]->isactive() && m_PlayerObject[i]->Intersects(*m_ColliderObjects[j]))
        {
           
            m_PlayerObject[i]->SetPos(spawn);
            m_PlayerObject[i]->SetAcceleration(Vector3(0.0F, 0.0f, 0.0f));
            m_PlayerObject[0]->SetPitchYawRoll(0.0F, XMConvertToRadians(90), 0.0f);
            lives -= 1;
            updatescreen();

           
            break;
            
        }
    }
}

void Game::screenupdater()
{
    for (int i = 0; i < m_PlayerObject.size(); i++) for (int j = 0; j < m_ColliderObjects.size(); j++)
    {
        if ( m_PlayerObject[i]->Intersects(*m_ColliderObjects[j]))
        {
           
            updatescreen();
            


            break;

        }
    }
}














void Game::ChecktargetpointsCollision()
{
    for (int i = 0; i < m_PlayerObject.size(); i++) for (int j = 0; j < m_TargetObjects_points.size(); j++)
    {
        if (m_TargetObjects_points[j]->isactive() && m_PlayerObject[i]->Intersects(*m_TargetObjects_points[j]))
        { 
            m_TargetObjects_points[j]->SetActive(false);
            score += 1;
            updatescreen();
        }
    }

    for (int i = 0; i < m_PlayerProjectile.size(); i++) for (int j = 0; j < m_TargetObjects_points.size(); j++)
    {
        if (m_PlayerProjectile[i]->isactive() && m_TargetObjects_points[j]->isactive() && m_PlayerProjectile[i]->Intersects(*m_TargetObjects_points[j]))
        {   
            m_PlayerProjectile[i]->SetActive(false);
            m_TargetObjects_points[j]->SetActive(false);
            score += 1;
            updatescreen();
            
        }
    }
}







void Game::ChecktargetmoveCollision()
{
    for (int i = 0; i < m_PlayerObject.size(); i++) for (int j = 0; j < m_TargetObjects_move_wall.size(); j++)
    {
        if (m_TargetObjects_move_wall[j]->isactive() && m_PlayerObject[i]->Intersects(*m_TargetObjects_move_wall[j]))
        {
            m_TargetObjects_move_wall[j]->SetActive(false);
            score += 1;

          
                move = true;
              
            
            
            updatescreen();
            auto wallmovedtext = std::make_shared<TextGO2D>("a wall somewhere has moved...");
            wallmovedtext->SetPos(Vector2(100, 200));
            wallmovedtext->SetColour(Color((float*)&Colors::Black));
            m_GameObjects2D.push_back(wallmovedtext);

            
            
           
        }
    }

    for (int i = 0; i < m_PlayerProjectile.size(); i++) for (int j = 0; j < m_TargetObjects_move_wall.size(); j++)
    {
        if (m_PlayerProjectile[i]->isactive() && m_TargetObjects_move_wall[j]->isactive() && m_PlayerProjectile[i]->Intersects(*m_TargetObjects_move_wall[j]))
        {
            
            m_PlayerProjectile[i]->SetActive(false);
            m_TargetObjects_move_wall[j]->SetActive(false);
            score += 1;

           
            
                move = true;
                
            
           
            
            
            

           
            updatescreen();
            auto wallmovedtext = std::make_shared<TextGO2D>("a wall somewhere has moved...");
            wallmovedtext->SetPos(Vector2(100, 200));
            wallmovedtext->SetColour(Color((float*)&Colors::Black));
            m_GameObjects2D.push_back(wallmovedtext);
            
        }
    }
}










void Game::CheckWinCollision()
{
    for (int i = 0; i < m_PlayerObject.size(); i++) for (int j = 0; j < m_WinObject.size(); j++)
    {
        if (m_PlayerObject[i]->Intersects(*m_WinObject[j]))
        {
            
            updatescreen();
            printf("WIN");
            m_PlayerObject[i]->SetPos(spawn);
            m_PlayerObject[i]->SetAcceleration(Vector3(0.0F, 0.0f, 0.0f));
            m_PlayerObject[0]->SetPitchYawRoll(0.0F, XMConvertToRadians(90), 0.0f);
            m_GD->m_GS = GS_WIN;
           
            
        

        
        

        }
    }

    for (int i = 0; i < m_PlayerProjectile.size(); i++) for (int j = 0; j < m_WinObject.size(); j++)
    {
        if (m_PlayerProjectile[i]->isactive() && m_PlayerProjectile[i]->Intersects(*m_WinObject[j]))
        {
        
            
            updatescreen();
            printf("WIN");
            m_PlayerObject[i]->SetPos(spawn);
            m_PlayerObject[i]->SetAcceleration(Vector3(0.0F, 0.0f, 0.0f));
            m_PlayerObject[0]->SetPitchYawRoll(0.0F, XMConvertToRadians(90), 0.0f);
            m_GD->m_GS = GS_WIN;








        }
    }
}

void Game::updatescreen()
{
    m_GameObjects2D.clear();

    std::string livesStr = std::to_string(lives);
    std::string scoreStr = std::to_string(score);

    

    auto text = std::make_shared<TextGO2D>("lives = " + livesStr);
    text->SetPos(Vector2(450, 10));
    text->SetColour(Color((float*)&Colors::Yellow));
    m_GameObjects2D.push_back(text);

    auto text1 = std::make_shared<TextGO2D>("score = " + scoreStr);

    text1->SetPos(Vector2(440, 50));
    text1->SetColour(Color((float*)&Colors::Yellow));
    m_GameObjects2D.push_back(text1);
}


