#ifndef UTILS_H
#define UTILS_H
#define GAME_TIME 180

class utils
{
public:
    utils();
    utils(int totalTime);
    int gameTimeRemaining();
    int beginTime;
    int gameTime;

};

#endif // UTILS_H
