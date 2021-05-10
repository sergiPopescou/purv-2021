/* Comments in Doxygen format (www.doxygen.org) */
/**
  * @file   brickos_system.h
  * @author Tobias Amnell <tobias.amnell@docs.uu.se>
  * @date   Mon Apr 22 10:55:25 2002
  * 
  * @brief  Definitions of types and macros used by generated code.
  * 
  * Define types and macros used by the atomaticatly generated code
  * from TimesTool.
  */

#ifndef BRICKOS_SYSTEM_H
#define BRICKOS_SYSTEM_H

#include "brickos_hooks.h"

/**
 * Read clock value of @a clname .
 */
#define rdClock(clname) (sys_time - clock_##clname)

/**
 * Set clock with id @a clname  to value @a value.
 */
#define setClock(clname, value) clock_##clname = sys_time-value

/**
 * Copy clock values with id @a clname2 to @a clname .
 */
#define cpClock(clname, clname2) clock_##clname = clock_##clname2

/**
 * Transition information
 */
typedef struct s_TransitionType trans_t;

/**
 * Location list type
 */
typedef int loc_t;

/**
 * Define bool type
 */
typedef int bool;

/**
 * Transition table element
 */
struct s_TransitionType {
  char active;      /**< Active transition */
  char from;        /**< Source location */
  char to;          /**< Target location id */
  signed char sync; /**< Syncronisation id */
};

#ifndef true
#define true (1==1)
#endif

#ifndef false
#define false (0==1)
#endif

#define SMALL_STACK_SIZE 128

#define TASK_BEGIN( taskname ) while(true) {\
    do { \
    wait_event( &task_release, tid_##taskname - tid_offset );\
    TASK_BEGIN_HOOK( tid_##taskname - tid_offset );\
    release_list[ tid_##taskname - tid_offset]--; }while(false); {

#define TASK_END( taskname ) do {\
    wait_event( &task_complete, tid_##taskname - tid_offset );\
    TASK_END_HOOK( tid_##taskname - tid_offset );\
    } while(false); }}\
    return 0;

#define CONTROLLER( AUT ) int AUT##_controller() {\
    while( true ) { \
	wait_event(& AUT##_events, 0);\
    }\
}

#endif /* BRICKOS_SYSTEM_H */
