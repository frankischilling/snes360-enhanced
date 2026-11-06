#ifndef XBOX_JOYPADS_H
#define XBOX_JOYPADS_H

#include "controls.h"

// The caller enables MultiPlayer5Master before selecting the adapter.
inline void SetXboxJoypadPorts(bool multitap)
{
    S9xSetController(0, CTL_JOYPAD, 0, -1, -1, -1);
    if (multitap)
        S9xSetController(1, CTL_MP5, 1, 2, 3, -1);
    else
        S9xSetController(1, CTL_JOYPAD, 1, -1, -1, -1);
    S9xVerifyControllers();
}

#endif
