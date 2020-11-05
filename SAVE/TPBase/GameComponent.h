#ifndef GAMECOMPONENT_H
#define GAMECOMPONENT_H



class GameComponent{
public :

    virtual void input();
    virtual void update();
    virtual void render();


};

#endif // GAMECOMPONENT_H
