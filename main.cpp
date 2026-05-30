/**
* Author: Radi Najeeb
* Assignment: And I Do Make Art, Thank You Very Much
* Date due: [05/30/2026]
* I pledge that I have completed this assignment without
* collaborating with anyone else, in conformance with the
* NYU School of Engineering Policies and Procedures on
* Academic Misconduct.
**/

#include "CS3113/cs3113.h"
#include <math.h>

enum Member { MURDOC, TWO_D, RUSSEL, NOODLE };
enum Direction {RIGHT, LEFT};

// Global Constants
constexpr int   SCREEN_WIDTH  = 1600 / 2,
                SCREEN_HEIGHT = 900 / 2,
                FPS           = 60,
                SIZE_SUN      = 150,
                SIZE_EARTH = 100,
                SIZE_TOM = 100,
                SIZE_STEWIE = 100,
                FRAME_LIMIT   = 100;
constexpr float MAX_AMP       = 10.0f;


constexpr char    BG_COLOUR[] = "#000000";
constexpr Vector2 ORIGIN      = { SCREEN_WIDTH / 2, SCREEN_HEIGHT / 2 };
constexpr Vector2 BASE_SIZE   = { (float) SIZE_SUN, (float) SIZE_SUN };
constexpr Vector2 BASE_SIZE_earth = { (float) SIZE_EARTH, (float) SIZE_EARTH };
constexpr Vector2 BASE_SIZE_Tom = { (float) SIZE_TOM, (float) SIZE_TOM };
constexpr Vector2 BASE_SIZE_Stewie = { (float) SIZE_STEWIE, (float) SIZE_STEWIE };
constexpr float RADIUS          = 160.0f, // radius of the orbit
                ORBIT_SPEED     = 1.0f,  // the speed at which the triangle will travel its orbit
                // BASE_SIZE       = 50,     // the size of the triangle when it's not being scaled
                MAX_AMPLITUDE   = 10.0f,  // by how much the triangle will be expanding/contracting
                PULSE_SPEED     = 100.0f, // how fast the triangle is going to be "pulsing"
                PULSE_INCREMENT = 10.0f,  // the current value we're scaling by
                SPIN_SPEED = 90.0f;

// Images
constexpr char Sun_FP[] = "assets/sun.png";
constexpr char Earth_FP[] = "assets/earthh.png";
constexpr char Tom_FP[] = "assets/Tom.png";
constexpr char Stewie_FP[] = "assets/stewiebaby.png";

// Global Variables
AppStatus gAppStatus = RUNNING;
float gPreviousTicks = 0.0f;
// for Sun
Texture2D gSunTexture;
Vector2 gSunPosition = ORIGIN;
Vector2 gSunScale = BASE_SIZE;
float gSunPulse = 0.0f;

// Earth
Texture2D gEarthTexture;
Vector2 gEarthPosition = ORIGIN;
Vector2 gEarthScale = BASE_SIZE_earth;
constexpr float LIMIT_ANGLE = 20.0f; // when turning directions will flip
float gEarthRotation = 0.0f;
float gOrbitAngle = 0.0f;

// Tom
Texture2D gTomTexture;
Vector2 gTomPosition = ORIGIN;
Vector2 gTomScale = BASE_SIZE_Tom;

// Stewie
Texture2D gStewieTexture;
Vector2 gStewiePosition = ORIGIN;
Vector2 gStewieScale = BASE_SIZE_Stewie;

// Function Declarations
void initialise();
void processInput();
void update();
void render();
void shutdown();


// Function Definitions
void initialise()
{
    InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "Solar System");

    gSunTexture = LoadTexture(Sun_FP);
    gEarthTexture = LoadTexture(Earth_FP);
    gTomTexture = LoadTexture(Tom_FP);
    gStewieTexture = LoadTexture(Stewie_FP);

    SetTargetFPS(FPS);
}

void processInput() 
{
    if (WindowShouldClose()) gAppStatus = TERMINATED;
}

void update() 
{
    /**
     * @todo Calculate delta time
     */

     float ticks = static_cast<float>(GetTime());
     float deltaTime = ticks - gPreviousTicks;
     gPreviousTicks = ticks;
 
     gSunPulse += 1.0f * deltaTime;
     gSunScale = {
        BASE_SIZE.x + (MAX_AMP+5.0f) * cos(gSunPulse),
        BASE_SIZE.y + (MAX_AMP+5.0f) * cos(gSunPulse)
      };

      gSunPosition = {
        ORIGIN.x + 20.0f * cos(gSunPulse * 0.3f),
        ORIGIN.y + 20.0f * sin(gSunPulse * 0.7f)
    }; // very slight motion, just to meet the expectations of translation.

      gOrbitAngle = gOrbitAngle + (ORBIT_SPEED * deltaTime);

      gEarthRotation += SPIN_SPEED * deltaTime;
        

      gEarthPosition = {
        ORIGIN.x + RADIUS * cos(gOrbitAngle),
        ORIGIN.y + RADIUS * sin(gOrbitAngle)
      };
     
      gEarthScale = {
        BASE_SIZE_earth.x + (MAX_AMP+5.0f) * cos(gSunPulse),
        BASE_SIZE_earth.y + (MAX_AMP+5.0f) * cos(gSunPulse)
      };

    // Stewie is ahead
    gStewiePosition = {
        ORIGIN.x + (RADIUS * 2 + 10.0f) * cos(gOrbitAngle),
        ORIGIN.y + (RADIUS - 1) * sin(2 * gOrbitAngle)
    };

    // Tom is behind Stewie (chasing)
    gTomPosition = {
        ORIGIN.x + (RADIUS * 2 + 10.0f) * cos(gOrbitAngle - 0.3f),
        ORIGIN.y + (RADIUS - 1) * sin(2 * (gOrbitAngle - 0.3f))
    };

   
}

void render()
{
    BeginDrawing();
    // background color changes
    // we use the sin because it cycles between 1 and =1.
    Color bgColor = {
        (unsigned char)(128 + 127 * sin(gSunPulse * 0.5f)),
        (unsigned char)(128 + 127 * sin(gSunPulse * 0.5f + 2.1f)),
        (unsigned char)(128 + 127 * sin(gSunPulse * 0.5f + 4.2f)),
        255
    };
    ClearBackground(bgColor);

    /**
     * Drawing Sun
     */
    Rectangle SunTextureArea = {
        // top-left corner
        0.0f, 0.0f,

        // bottom-right corner (of texture)

        static_cast<float>(gSunTexture.width),
        static_cast<float>(gSunTexture.height)
    };

    // Destination rectangle – centred on gPosition
    Rectangle SundestinationArea = {
        gSunPosition.x,
        gSunPosition.y,
        static_cast<float>(gSunScale.x),
        static_cast<float>(gSunScale.y)
    };

    // Origin inside the source texture (centre of the texture)
    Vector2 SunOrigin = {
        static_cast<float>(gSunScale.x) / 2.0f,
        static_cast<float>(gSunScale.y) / 2.0f
    };

    // Render the texture on screen
    DrawTexturePro(
        gSunTexture, 
        SunTextureArea, 
        SundestinationArea, 
        SunOrigin, 
        0.0f, 
        WHITE
    );

    // Drawing Earth

    Rectangle EarthTextureArea = {
        //top left corner
        0.0f, 0.0f,
        //size of earth texture
        static_cast<float>(gEarthTexture.width),
        static_cast<float>(gEarthTexture.height)
    };

    Rectangle earthDestinationArea = {
        gEarthPosition.x,
        gEarthPosition.y,
        static_cast<float>(gEarthScale.x),
        static_cast<float>(gEarthScale.y)
    };

    Vector2 EarthOrigin = {
        static_cast<float>(gEarthScale.x) / 2.0f, static_cast<float>(gEarthScale.y)/2.0f
    };

    DrawTexturePro(
        gEarthTexture, EarthTextureArea, earthDestinationArea, 
        EarthOrigin, gEarthRotation, WHITE);

    // Drawing Tom

    Rectangle TomTextureArea = {
        0.0f, 0.0f, 
        static_cast<float>(gTomTexture.width), 
        static_cast<float>(gTomTexture.height)
    };

    Rectangle TomDestinationArea = {
        gTomPosition.x, 
        gTomPosition.y,
        static_cast<float>(gTomScale.x),
        static_cast<float>(gTomScale.y)
    };

    Vector2 TomOrigin = {
        static_cast<float>(gEarthScale.x) / 4.0f, static_cast<float>(gEarthScale.y)/4.0f
    };

    DrawTexturePro (
        gTomTexture, TomTextureArea, TomDestinationArea, TomOrigin, 0.0f, WHITE
    );

    // Drawing Stewie

    Rectangle StewieTextureArea = {
        0.0f, 0.0f,
        static_cast<float>(gStewieTexture.width),
        static_cast<float>(gStewieTexture.height)
    };

    Rectangle StewieDestinationArea = {
        gStewiePosition.x,
        gStewiePosition.y,
        static_cast<float>(gStewieScale.x),
        static_cast<float>(gStewieScale.y)
    };

    Vector2 gStewieOrigin = {
        static_cast<float>(gEarthScale.x) / 4.0f, static_cast<float>(gEarthScale.y)/4.0f
    };

    DrawTexturePro(
        gStewieTexture, 
        StewieTextureArea, 
        StewieDestinationArea, 
        gStewieOrigin,
        0.0f, 
        WHITE
    );

    EndDrawing();
}

void shutdown() { CloseWindow(); }

int main(void)
{
    initialise();

    while (gAppStatus == RUNNING)
    {
        processInput();
        update();
        render();
    }

    shutdown();

    return 0;
}