#include "utils.h"
#include <time.h>

utils::utils()
{
    beginTime= time(NULL);
    gameTime=GAME_TIME;
}

utils::utils(int totalTime)
{
    beginTime= time(NULL);
    gameTime=totalTime;
}

int utils::gameTimeRemaining(){
      int elapsedTime= difftime (time(0),beginTime);
      return gameTime - elapsedTime;
}

void utils::reset(){
    beginTime= time(NULL);
}
