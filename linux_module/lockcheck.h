#ifndef _lockcheck
#define _lockcheck


#ifdef V3_CONFIG_DEBUG_LOCKS

// Maxmimum number of locks to handle
#define NUM_LOCKS        1024
// Maximum number of locks that can be simultaneously
// held on each CPU
#define LOCK_STACK_DEPTH 16

//
// The following macros are used
// in the stub functions to call back to the lock
// checker - if lock checking is not enabled, these 
// turn into nothing
//
#define LOCKCHECK_INIT() palacios_lockcheck_init()
#define LOCKCHECK_ALLOC(lock) palacios_lockcheck_alloc(lock)
#define LOCKCHECK_FREE(lock)  palacios_lockcheck_free(lock)
#define LOCKCHECK_LOCK_PRE(lock)
#define LOCKCHECK_LOCK_POST(lock)  palacios_lockcheck_lock(lock)
#define LOCKCHECK_UNLOCK_PRE(lock) palacios_lockcheck_unlock(lock)
#define LOCKCHECK_UNLOCK_POST(lock) 
#define LOCKCHECK_LOCK_IRQSAVE_PRE(lock, flags) 
#define LOCKCHECK_LOCK_IRQSAVE_POST(lock, flags)  palacios_lockcheck_lock_irqsave(lock,flags)
#define LOCKCHECK_UNLOCK_IRQRESTORE_PRE(lock, flags) palacios_lockcheck_unlock_irqrestore_pre(lock,flags)
#define LOCKCHECK_UNLOCK_IRQRESTORE_POST(lock, flags) palacios_lockcheck_unlock_irqrestore_post(lock,flags)
#define LOCKCHECK_DEINIT() palacios_lockcheck_deinit()

void palacios_lockcheck_init(void);
void palacios_lockcheck_alloc(void *lock);
void palacios_lockcheck_free(void *lock);
void palacios_lockcheck_lock(void *lock);
void palacios_lockcheck_unlock(void *lock);
void palacios_lockcheck_lock_irqsave(void *lock,unsigned long flags);
void palacios_lockcheck_unlock_irqrestore_pre(void *lock,unsigned long flags);
void palacios_lockcheck_unlock_irqrestore_post(void *lock,unsigned long flags);
void palacios_lockcheck_deinit(void);

#else

//
// The following is what happens when lock checking is not on
//
#define LOCKCHECK_INIT()
#define LOCKCHECK_ALLOC(lock) 
#define LOCKCHECK_FREE(lock)  
#define LOCKCHECK_LOCK_PRE(lock)
#define LOCKCHECK_LOCK_POST(lock)  
#define LOCKCHECK_UNLOCK_PRE(lock) 
#define LOCKCHECK_UNLOCK_POST(lock)
#define LOCKCHECK_LOCK_IRQSAVE_PRE(lock, flags)  
#define LOCKCHECK_LOCK_IRQSAVE_POST(lock, flags)  
#define LOCKCHECK_UNLOCK_IRQRESTORE_PRE(lock, flags) 
#define LOCKCHECK_UNLOCK_IRQRESTORE_POST(lock, flags) 
#define LOCKCHECK_DEINIT()

#endif


#endif
