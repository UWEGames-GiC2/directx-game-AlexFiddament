#ifndef _GAME_STATE_H_
#define _GAME_STATE_H_

//=================================================================
//Possible GameStates
//=================================================================

enum GameState {
	GS_NULL = 0,
	GS_ATTRACT,
	GS_GAME_OVER,
	GS_PLAY_FIRST_PERSON_CAM,
	GS_WIN,
	GS_COUNT
};

#endif
