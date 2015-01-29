/* Utils for the main code of the robot, for things such as timer
 *
 *
*/
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
    void reset();

};

#endif // UTILS_H
