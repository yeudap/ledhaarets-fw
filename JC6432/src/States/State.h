#ifndef State_H
#define State_H

class State
{
protected:
    State* _menuState;
    State* _upState;
    State* _downState;
    State* _okState;

public:
    //Constructor
    State();

    //Getters
    State* menuPushed();
    State* upPushed();
    State* downPushed();
    State* okPushed();
    
    State* menuState();
    State* upState();
    State* downState();
    State* okState();

    //Setters
    void setMenu(State* state);
    void setUp(State* state);
    void setDown(State* state);
    void setOk(State* state);
    void setStates(State* menu, State* up, State* down, State* ok);

    //Functions
    virtual void menuAction();
    virtual void upAction();
    virtual void downAction();
    virtual void okAction();
    virtual void showStateScreen();
    virtual void stateLoop();
};

#endif