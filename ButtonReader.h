#ifndef ButtonReader
#define ButtonReader

#include "Arduino.h"

void ReadButtons();

void SetMetaParameters();
void EditParameter(bool increment);
void EditMenuItem(bool increment);

#endif