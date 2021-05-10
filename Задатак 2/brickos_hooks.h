/**
 * @file   brickos_hooks.h
 * @author Tobias Amnell <tobiasa@docs.uu.se>
 * @date   Thu Nov 21 11:56:51 2002
 * 
 * @brief Defines actions to take when hooks are called. Currently
 * only used together with lnp logging.
 * 
 * The hooks are called by the code in (@see brickos_kernel.c) when actions are
 * taken. The currently supported hooks are:
 * - Transition taken
 * - Task released
 * - Taske begin
 * - Task end
 */

#ifdef LNP_LOGGING

#include "brickos_logging.h"
/**
 * Hook called when transition was taken
 */
#define TRANS_TAKEN_HOOK( trn ) (void)addDataI( LOG_TRANS, trn )

/**
 * Hook called when task was released
 */
#define TASK_RELEASED_HOOK( tid ) (void)addDataI( LOG_TASK_RELEASE, tid)

/**
 * Hook called when task begins executing
 */
#define TASK_BEGIN_HOOK( tid ) (void)addDataI( LOG_TASK_BEGIN, tid)

/**
 * Hook called when task begins executing
 */
#define TASK_END_HOOK( tid ) (void)addDataI( LOG_TASK_END, tid)

#else // No-action hooks

#define TRANS_TAKEN_HOOK( trn )
#define TASK_RELEASED_HOOK( tid )
#define TASK_BEGIN_HOOK( tid )
#define TASK_END_HOOK( tid )

#endif // LNP_LOGGING
