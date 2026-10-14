#include "States.h"
#include "Logic.h"

extern Logic bl;

void MenuClockState::showStateScreen() { menuScreen.printMenu(strings.menuTitle, strings.menuConfg, strings.menuClock, strings.menuTzuga); }
void MenuTzugaState::showStateScreen() { menuScreen.printMenu(strings.menuTitle, strings.menuClock, strings.menuTzuga, strings.menuEddot); }
void MenuEddotState::showStateScreen() { menuScreen.printMenu(strings.menuTitle, strings.menuTzuga, strings.menuEddot, strings.menuOffek); }
void MenuOffekState::showStateScreen() { menuScreen.printMenu(strings.menuTitle, strings.menuEddot, strings.menuOffek, strings.menuMinag); }
void MenuMinagState::showStateScreen() { menuScreen.printMenu(strings.menuTitle, strings.menuOffek, strings.menuMinag, strings.menuTohen); }
void MenuTohenState::showStateScreen() { menuScreen.printMenu(strings.menuTitle, strings.menuMinag, strings.menuTohen, strings.menuConfg); }
void MenuConfgState::showStateScreen() { menuScreen.printMenu(strings.menuTitle, strings.menuTohen, strings.menuConfg, strings.menuClock); }

void BrightnessState::showStateScreen() { menuScreen.printMenu(strings.tzugaTitle, strings.tzeva, strings.behirut, strings.gufan); }
void FontState::showStateScreen() { menuScreen.printMenu(strings.tzugaTitle, strings.behirut, strings.gufan, strings.tzeva); }
void ColorState::showStateScreen() { menuScreen.printMenu(strings.tzugaTitle, strings.gufan, strings.tzeva, strings.behirut); }

