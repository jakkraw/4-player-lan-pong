#pragma once

#include "netInterface.h"
#include "idInterface.h"

class NetComponentInterface:public NetInterface,public IdInterface {
// wrapper
};