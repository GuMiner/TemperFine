#include <SFML\System.hpp>
#include "SharedExclusiveLock.h"

ReadLock::ReadLock(SharedExclusiveLock& lock) : lock(lock)
{
    lock.ReadLock();
}

ReadLock::~ReadLock()
{
    lock.ReadUnlock();
}

WriteLock::WriteLock(SharedExclusiveLock& lock) : lock(lock)
{
    lock.WriteLock();
}

WriteLock::~WriteLock()
{
    lock.WriteUnlock();
}

SharedExclusiveLock::SharedExclusiveLock()
{
    readers = 0;
}

void SharedExclusiveLock::ReadLock()
{
    mutex.lock();
    ++readers;
    mutex.unlock();
}

void SharedExclusiveLock::ReadUnlock()
{
    mutex.lock();
    --readers;
    mutex.unlock();
}

void SharedExclusiveLock::WriteLock()
{
    bool attemptingToLock = true;
    while (attemptingToLock)
    {
        if (readers == 0)
        {
            mutex.lock();
            if (readers == 0)
            {
                // We locked the mutex with no readers. Continue with write.
                return;
            }

            // Oops, reader acquired a handle. Continue waiting.
            mutex.unlock();
        }

        sf::sleep(sf::microseconds(100)); // 0.1 ms
    }
}

void SharedExclusiveLock::WriteUnlock()
{
    // Unlock the held-open-during-write mutex.
    mutex.unlock();
}