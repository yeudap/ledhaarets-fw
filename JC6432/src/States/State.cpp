#include "State.h"


State::State()
{
    _menuState = nullptr;
    _upState = nullptr;
    _downState = nullptr;
    _okState = nullptr;
}
void State::setStates(State* menu, State* up, State* down, State* ok)
{
    setMenu(menu);
    setUp(up);
    setDown(down);
    setOk(ok);
}
State* State::menuState() 
{ 
    menuAction();
    return _menuState; 
}
State* State::upState()  
{ 
    upAction();
    return _upState; 
}
State* State::downState()  
{ 
    downAction();
    return _downState; 
}
State* State::okState()  
{ 
    okAction();
    return _okState; 
}

State* State::menuPushed() { return _menuState; }
State* State::upPushed() { return _upState; }
State* State::downPushed() { return _downState; }
State* State::okPushed() { return _okState; }

void State::setMenu(State* state) { _menuState = state; }
void State::setUp(State* state)  { _upState = state; }
void State::setDown(State* state)  { _downState = state; }
void State::setOk(State* state)  { _okState = state; }

void State::menuAction(){}
void State::upAction(){}
void State::downAction(){}
void State::okAction(){}

void State::showStateScreen(){}
void State::stateLoop(){}