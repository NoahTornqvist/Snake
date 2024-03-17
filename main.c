/**
 * @file event_handler.c
 * @brief Implementation file for the event handler function.
 * @details This file contains the implementation of the event handler function
 *          for the Extension project, which initializes the game and sets up
 *          the update callback.
 * @author Noah Törnqvist
 */

#include <stdio.h>
#include <stdlib.h>

#include "game_functions.h"
#include "pd_api.h"

#ifdef _WINDLL
__declspec(dllexport)
#endif

/** @brief Event handler function for the Extension project.
 *  @param playdate Pointer to the PlaydateAPI instance.
 *  @param event The system event.
 *  @param arg Additional argument associated with the event.
 *  @return 0 if event is handled successfully.
 */
int eventHandler(PlaydateAPI* playdate, PDSystemEvent event, uint32_t arg)
{
    if (event == kEventInit)
    {
        // Set the pointer to the PlaydateAPI instance
        setPDPtr(playdate);
        
        // Set the display refresh rate to run as fast as possible
        playdate->display->setRefreshRate(0);
        
        // Set the update callback function
        playdate->system->setUpdateCallback(update, NULL);
        
        // Initialize the game (optional)
        // setupGame();
    }
    
    return 0;
}
