#include "rwlock.h"


void InitalizeReadWriteLock(struct read_write_lock * rw)
{
  //	Write the code for initializing your read-write lock.
  rw->readers=0;
  pthread_mutex_init(&rw->lock,NULL);
  pthread_mutex_init(&rw->writelock,NULL);
}

void ReaderLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the reader.
  pthread_mutex_lock(&rw->lock);
  rw->readers++;
  if(rw->readers==1)
     pthread_mutex_lock(&rw->writelock);
  pthread_mutex_unlock(&rw->lock);

}

void ReaderUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the reader.
    pthread_mutex_lock(&rw->lock);
  rw->readers--;
  if(rw->readers==0)
     pthread_mutex_unlock(&rw->writelock);
  pthread_mutex_unlock(&rw->lock);
}

void WriterLock(struct read_write_lock * rw)
{
  //	Write the code for aquiring read-write lock by the writer.
  pthread_mutex_lock(&rw->writelock);
}

void WriterUnlock(struct read_write_lock * rw)
{
  //	Write the code for releasing read-write lock by the writer.
  pthread_mutex_unlock(&rw->writelock);
}
