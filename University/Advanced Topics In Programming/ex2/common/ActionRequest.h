#pragma once
#include <iostream>

enum class ActionRequest { 
MoveForward, MoveBackward,
RotateLeft90, RotateRight90, RotateLeft45, RotateRight45,
Shoot, GetBattleInfo, DoNothing
};
