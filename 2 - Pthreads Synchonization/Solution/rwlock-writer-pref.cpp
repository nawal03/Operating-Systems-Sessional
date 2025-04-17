#include "rwlock.h"

void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  rw->writers=0;
  rw->readers=0;
  rw->writing=0;
  pthread_mutex_init(&rw->lock,NULL);
  pthread_cond_init(&rw->readers_wait,NULL);
  pthread_cond_init(&rw->writers_wait,NULL);
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->lock);
  while(rw->writers!=0){
    pthread_cond_wait(&rw->readers_wait,&rw->lock);
  }
  rw->readers++;
  pthread_mutex_unlock(&rw->lock);
}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
  pthread_mutex_lock(&rw->lock);
  rw->readers--;
  if(rw->readers==0)
    pthread_cond_signal(&rw->writers_wait);
  pthread_mutex_unlock(&rw->lock);
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->lock);
  rw->writers++;
  while(rw->writing==1 || rw->readers > 0){
    pthread_cond_wait(&rw->writers_wait,&rw->lock);
  }
  rw->writing=1;
  pthread_mutex_unlock(&rw->lock);
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_lock(&rw->lock);
  rw->writers--;
  if(rw->writers>0){
    rw->writing=0;
    pthread_cond_signal(&rw->writers_wait);
  }
  else pthread_cond_broadcast(&rw->readers_wait);
  pthread_mutex_unlock(&rw->lock);
  
}
