/**
 * @file   brickos_kernel.c
 * @author Tobias Amnell <tobias.amnell@docs.uu.se>
 * @date   Thu Mar 28 14:28:23 2002
 * 
 * @brief The kernel functions that executes the controller.
 */

#include "brickos_interface.h"

extern int chanusage[];
extern trans_t trans[];
extern loc_t loc[];
extern int eval_guard(int);
extern void assign(int);
//extern bool pc[];

#if NB_TASK>0
extern char release_list[NB_TASK];
#endif /* NB_TASK>0 */

#ifndef tid_offset
/**
 * Offset to separate transition ids from task ids. Transitions are
 * between 1 and tid_offset and tasks are between tid_offset and MAX_TRANS.
 */
#define tid_offset 200
#endif /* tid_offset */

#ifndef recv_channels
/**
 * Receive channels have id larger or equal to recv_channel
 */
#define recv_channels 100
#endif /* recv_channels */

/**
 * Release task with id @a tid.  Copy variables used by task to local
 * store and increase task element in release list.
 */
#if (NB_TASK>0)
#define releaseTask( tid ) do{ \
  release_list[ tid ]++; \
} while(0)
#else
#define releaseTask( tid ) do{ } while(0)
#endif /* NB_TASK>0 */

/*
 * Private functions
 */
#if NB_CHAN>0
static int check_synch( unsigned char sync );
#endif

static void clear_and_set ( unsigned char sync );

/** 
 * Deterimines if it is possible to synchronise on a channel.
 * N.B. Transition id 0 is not allowed, since 0 indicates that no
 * synchronisation was possible.
 *
 * @param sync Channel id, i.e. an index into the chanusage array.
 * 
 * @return 0 if synchronisation is not possible, transition id of the
 * other transition if it is.
 */
#if NB_CHAN>0
static int check_synch( unsigned char sync ) {
  while( chanusage[sync] < NB_TRANS ) {
    if( (trans[chanusage[sync]].active==true) && 
	eval_guard( chanusage[sync] ) )
      return chanusage[sync];
    sync++;
  }
  return false;
}
#endif

/**
 * Update the list of active transitions when a transition is taken,
 * and release task of target location.
 *
 * @param trn The transition taken
 */
static void clear_and_set ( unsigned char trn ) {
  int jj;
  // Clear outgoing transition from source location
  jj=trans[trn].from;
  do {
    trans[loc[jj]].active=0;
  } while(loc[++jj]<tid_offset);
  // Set outgoing transition from target location 
  jj=trans[trn].to;
  while ( loc[jj]<tid_offset ) {
    trans[loc[jj]].active=1;
    jj++;
  }
  // Release task of target location
  releaseTask(loc[jj]-tid_offset);

  TASK_RELEASED_HOOK(loc[jj]-tid_offset);
}

/**
 * Is a channel @a id a sending channel?
 */
#define IS_SEND( id ) (id < recv_channels )

/**
 * Check if any of the active transitions are enabled, and if so take
 * it. Will continue until a stable state (no more enabled
 * transitions) is reached.
 *
 * @param data Unused (should be null).
 * @return false when in stable state
 */
wakeup_t check_trans( wakeup_t data ) {
  int trn;
#if NB_CHAN>0
  int compl_trn;
#endif /* NB_CHAN>0 */
  for(trn=0; trn<NB_TRANS; trn++) {
    if( trans[trn].active ) {
      if( eval_guard(trn) ) {
#if NB_CHAN>0
	if( trans[trn].sync > -1 ) {
	  if( (compl_trn = 
	      check_synch( trans[trn].sync )) ) {
	    if( IS_SEND(trans[trn].sync) ) {
	      assign( trn );
	      assign( compl_trn );

	      TRANS_TAKEN_HOOK(trans[trn].to);
	      TRANS_TAKEN_HOOK(trans[compl_trn].to);

	    } else {
	      assign( compl_trn ); 
	      assign( trn );	    

	      TRANS_TAKEN_HOOK(trans[compl_trn].to);
	      TRANS_TAKEN_HOOK(trans[trn].to);

	    }
	    clear_and_set( trn );
	    clear_and_set( compl_trn );	   
	    trn=-1;
	  }
	} else 
#endif /* NB_CHAN>0 */ 
	{
	  assign( trn );
	  TRANS_TAKEN_HOOK(trans[trn].to);
	  clear_and_set( trn );
	  trn=-1;
	} 
      }
    }
  }
  return false;
}

/**
 * Wake-up function for threads (tasks).
 * @param data Unused (should be null)
 */
#if NB_TASK>0
/* Replaced by preceedence implementation */
/* wakeup_t task_release(wakeup_t data) { */
/*     return release_list[data]; */
/* } */
#endif /* NB_TASK */

/**
 * Thread body for automata controller
 * @return 0 (allways)
 */
int controller() {
  wait_event( &check_trans, 0);
  return 0;
}
