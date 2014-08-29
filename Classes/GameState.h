//
//  GameState.h
//  pf01
//
//  Created by 野口一也 on 2014/08/26.
//
//

#ifndef pf01_GameState_h
#define pf01_GameState_h

#include "Constants.h"

class GameState
{
public:
    GameState()
    : _remaining(START_REMAINING)
    , _score(0)
    , _level(0)
    {
        
    }
    int _remaining;
    int _score;
    int _level;
};

#endif
