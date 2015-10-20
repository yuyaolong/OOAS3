#include <iostream>
#include <string>
#include <iomanip>
#include "vector2f.h"
#include "multisprite.h"
#include "sprite.h"
#include "gamedata.h"
#include "manager.h"
#include "extractSurface.h"
#include "frameFactory.h"
#include "twowaysprite.h"
#include "enemy.h"
#include "vessel.h"

Manager::~Manager() { 
  std::list<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    delete (*ptr);
    ++ptr;
  }
}

Manager::Manager() :
  env( SDL_putenv(const_cast<char*>("SDL_VIDEO_CENTERED=center")) ),
  io( IOManager::getInstance() ),
  clock( Clock::getInstance() ),
  screen( io.getScreen() ),
  world("back"),
  viewport( Viewport::getInstance() ),


  sprites(),
  currentSprite(),

  makeVideo( false ),
  frameCount( 0 ),
  username(  Gamedata::getInstance().getXmlStr("username") ),
  title( Gamedata::getInstance().getXmlStr("screenTitle") ),
  frameMax( Gamedata::getInstance().getXmlInt("frameMax") )
{
  if (SDL_Init(SDL_INIT_VIDEO) != 0) {
    throw string("Unable to initialize SDL: ");
  }
  SDL_WM_SetCaption(title.c_str(), NULL);
  atexit(SDL_Quit);

 
  
  	
  sprites.push_back( new TwoWaySprite("Gundam"));
    
    
for (int i = 0; i< Gamedata::getInstance().getXmlInt("Enemy/number"); i++) {
        sprites.push_back( new Enemy("Enemy"));
}
  
for (int i=0; i< Gamedata::getInstance().getXmlInt("Vessel1/number"); i++) {
    sprites.push_back( new Vessel("Vessel1") );
}
  
  

  currentSprite = sprites.begin();
  viewport.setObjectToTrack(*currentSprite);
}

void Manager::draw() const {
  world.draw();
  clock.draw();
  std::list<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->draw();
    ++ptr;
  }

  io.printMessageAt("Press T to switch sprites", 10, 70);
  io.printMessageAt(title, 10, 450);
  viewport.draw();

  SDL_Flip(screen);
}

// Move this to IOManager
// void Manager::makeFrame() {
//   std::stringstream strm;
//   strm << "frames/" << username<< '.' 
//        << std::setfill('0') << std::setw(4) 
//        << frameCount++ << ".bmp";
//   std::string filename( strm.str() );
//   std::cout << "Making frame: " << filename << std::endl;
//   SDL_SaveBMP(screen, filename.c_str());
// }

void Manager::switchSprite() {
  ++currentSprite;
  if ( currentSprite == sprites.end() ) {
    currentSprite = sprites.begin();
  }
  viewport.setObjectToTrack(*currentSprite);
}

void Manager::update() {
  clock.update();
  Uint32 ticks = clock.getTicksSinceLastFrame();

  std::list<Drawable*>::const_iterator ptr = sprites.begin();
  while ( ptr != sprites.end() ) {
    (*ptr)->update(ticks);
    ++ptr;
  }
  if ( makeVideo && frameCount < frameMax ) {
    io.makeFrame(frameCount);
  }
  world.update();
  viewport.update(); // always update viewport last
}

void Manager::play() {
  SDL_Event event;
  bool done = false;
  clock.start();

  while ( not done ) {
    while ( SDL_PollEvent(&event) ) {
      Uint8 *keystate = SDL_GetKeyState(NULL);
      if (event.type ==  SDL_QUIT) { done = true; break; }
      if(event.type == SDL_KEYDOWN) {
        if (keystate[SDLK_ESCAPE] || keystate[SDLK_q]) {
          done = true;
          break;
        }
        if ( keystate[SDLK_t] ) {
          switchSprite();
        }
        if ( keystate[SDLK_p] ) {
          if ( clock.isPaused() ) clock.unpause();
          else clock.pause();
        }
        if (keystate[SDLK_s]) {
          clock.toggleSloMo();
        }
        if (keystate[SDLK_F4] && !makeVideo) {
          std::cout << "Making video frames" << std::endl;
          makeVideo = true;
        }
      }
    }

    update();
    draw();
    
  }
}
