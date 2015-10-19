//
//  enemy.cpp
//  OOAS3
//
//  Created by yuyaolong on 15/10/18.
//  Copyright © 2015年 yuyaolong. All rights reserved.
//

#include "enemy.h"
#include "gamedata.h"

Enemy::Enemy(const std::string& name):TwoWaySprite(name,
                                                   Gamedata::getInstance().getXmlInt("world/width")*(rand()*1.0/RAND_MAX),
                                                   Gamedata::getInstance().getXmlInt("world/height")*(rand()*1.0/RAND_MAX),
                                                   Gamedata::getInstance().getXmlInt(name+"/speedX")*(rand()*1.0/RAND_MAX+0.3),
                                                   Gamedata::getInstance().getXmlInt(name+"/speedY")*(rand()*1.0/RAND_MAX+0.3)
                                                   )
{
    
}

