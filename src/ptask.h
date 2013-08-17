/*--------------------------------------------------------------*/
/*		PTASK LIBRARY - HEADER FILE			*/
/*--------------------------------------------------------------*/

#ifndef __PTASK_H__
#define __PTASK_H__

#include <pthread.h>
#include <semaphore.h>
#include <ptime.h>
#include <rtmode.h>

/*--------------------------------------------------------------*/

#define	MAX_TASKS	50
#define	MAX_GROUPS	10

/* activation flag for task_create */
#define	NOACT		0
#define	ACT		1

typedef enum {PARTITIONED, GLOBAL} global_policy;
typedef enum {PRIO_INHERITANCE, PRIO_CEILING, NO_PROTOCOL} sem_protocol;
typedef enum {PERIODIC, APERIODIC} ptask_type;

/**
   This structure is used to simplify the creation of a task by
   setting standard arguments
 */
typedef struct {
  ptask_type type; 
  tspec period; 
  tspec rdline;
  int priority;              /*< from 0 to 99                         */
  int processor;             /*< processor id                         */
  int act_flag;              /*< ACT if the create activates the task */

  int measure;               /*< if 1, activates measure of exec time */
  void *arg;                 /*< pointer to a task argument           */
  rtmode_t *modes;           /*< a pointer to the mode handler        */
  int mode_list[RTMODE_MAX_MODES];  /*< the maximum number of modes   */
  int nmodes;               /*< num of modes in which the task is act */
} task_spec_t;

extern const task_spec_t TASK_SPEC_DFL;

/* ------------------------------------------------------------------ */
/*                     GLOBAL FUNCTIONS                               */
/* ------------------------------------------------------------------ */
int  ptask_getnumcores(); /* returns the number of available cores    */

/** The following function initializes the library. The policy can be:
    - SCHED_FIFO      fixed priority, fifo for same priority tasks
    - SCHED_RR        fixed priority, round robin for same priority tasks 
    - SCHED_OTHER     classical Linux scheduling policy (background) 
    
    The global policy can be:
    - GLOBAL          global scheduling with migration
    - PARTITIONED     partitioned scheduling (no migration)

    The semaphore protocol can be:
    - PRIO_INHERIT
    - PRIO_CEILING
*/ 
void  ptask_init(int policy,
		 global_policy global, 
		 sem_protocol protocol); 

/** Prints an error message on stderr and exits. The first string
    should contain the name of the failing function, the second string
    should contain a description of the error */
void  ptask_syserror(char *fun, char *msg); 


/*--------------------------------------------------------------------- */
/*			TASK CREATION                                   */
/*----------------------------------------------------------------------*/
int  ptask_create(void (*task)(void), 
		  int period, int drel, int prio, int aflag);

int   ptask_create_ex(void (*task)(void), task_spec_t *tp);

/*-------------------------------------------------------------------------- */
/*			TASK FUNCTIONS                                       */
/*---------------------------------------------------------------------------*/
void      wait_for_instance(); /** waits for next act, periodic or aperiodic */
void	  wait_for_activation();  /** waits for an exp. activation           */
//void	  wait_for_period();      /** waits for next periodic act.           */

void	  task_activate(int i); /** activates the task of idx i              */


void      set_activation(const tspec *off); /** sets the act. time           */
int       get_taskindex();        /** returns the task own index             */
pthread_t get_threadid(int i);    /** returns the thread own id              */
int	  deadline_miss(int i);

void	  task_setdeadline(int i, int dline);
void *    task_argument();
void	  task_setwcet(int i, long wc);
long	  task_wcet(int i);
void	  task_setperiod(int i, int per);
int	  task_period(int i);
int	  task_deadline(int i);
void	  task_setdeadline(int i, int dline);
int	  task_dmiss(int i);
long	  task_atime(int i);
long	  task_absdl(int i);

int       task_migrate_to(int core_id); 

#endif

/*--------------------------------------------------------------*/

