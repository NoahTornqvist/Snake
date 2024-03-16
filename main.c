//
//  main.c
//  Extension
//
//  Created by Dave Hayden on 7/30/14.
//  Copyright (c) 2014 Panic, Inc. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>

#include "game_functions.h"
#include "pd_api.h"

#ifdef _WINDLL
__declspec(dllexport)
#endif
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
	if (event == kEventInit)
	{
		setPDPtr(playdate);
		playdate->display->setRefreshRate(0); // run as fast as possible
		playdate->system->setUpdateCallback(update, NULL);
		//setupGame();
	}
	
	return 0;
}
