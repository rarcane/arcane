#ifndef TELESCOPE_H_INCLUDED
#define TELESCOPE_H_INCLUDED

#include "tsfoundation/tsfoundation.h"

#ifdef TS2D
#include "ts2d/ts2d.h"
#endif

#ifdef TS3D
#include "ts3d/ts3d.h"
#endif

#ifdef TSASSETS
#include "tsassets/tsassets.h"
#endif

#ifdef TSDEVTERMINAL
#include "tsdevterminal/tsdevterminal.h"
#endif

#ifdef TSINPUT
#include "tsinput/tsinput.h"
#endif

#ifdef TSSCRIPT
#include "tsscript/tsscript.h"
#endif

#ifdef TSUI
#include "tsui/tsui.h"
#endif

internal void GameInit(void);
internal void GameHotLoad(void);
internal void GameHotUnload(void);
internal void GameUpdate(void);

#endif // TELESCOPE_H_INCLUDED
