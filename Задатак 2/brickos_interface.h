/* Comments in Doxygen format (www.doxygen.org) */
/**
 * @file brickos_interface.h
 * @author Tobias Amnell <tobias.amnell@docs.uu.se>
 * @date 23 May 2002
 *
 * @brief Define interface to hybrid automata kernel/engine.
 */

#ifndef BRICKOS_INTERFACE_H
#define BRICKOS_INTERFACE_H

#include <unistd.h>
#include <dsensor.h>
#include <dmotor.h>
#include <sys/tm.h>
#include <conio.h>
#include <dsound.h>
#include <semaphore.h>

#include "brickos_system.h"

#ifdef LNP_LOGGING
#include "brickos_logging.h"
#endif

/*
 * API in brickos_kernel
 */

wakeup_t check_trans( wakeup_t data );
wakeup_t task_release(wakeup_t data);
int controller();

#endif /* BRICKOS_INTERFACE_H */
