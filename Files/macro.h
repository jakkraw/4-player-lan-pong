#pragma once

#define SAFE_DELETE(X){delete X;X = 0;}
#define IS_A(PTR,CLASSPTR)(bool)(dynamic_cast<CLASSPTR>(PTR) != nullptr)