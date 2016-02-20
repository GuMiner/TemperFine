#pragma once
#include <mutex>

struct CopyableMutex : std::mutex
{
    CopyableMutex()
    {

    }

    CopyableMutex(const CopyableMutex& copyableMutex) noexcept : std::mutex()
    {

    }

    bool operator==(const CopyableMutex& copyableMutex) noexcept
    {
        return this == &copyableMutex;
    }
};

// Also known as a multiple-reader, single-writer lock.
// Allows for multiple readers to access the single object, but only a single object.
//  [Not indended for direct use. Use ReadLock and WriteLock for most use cases]
class SharedExclusiveLock
{
    friend class ReadLock;
    friend class WriteLock;

public:
    SharedExclusiveLock();
    void ReadLock();
    void ReadUnlock();

protected:
    void WriteLock();
    void WriteUnlock();

private:
    CopyableMutex mutex;
    
    unsigned int readers;
};

// Acquires the SharedExclusiveLock for reading.
// Lock is freed when the object is disposed.
class ReadLock
{
public:
    ReadLock(SharedExclusiveLock& lock);
    ~ReadLock();

private:
    SharedExclusiveLock& lock;
};

// Acquires the SharedExclusiveLock for writing.
// Lock is freed when the object is disposed.
class WriteLock
{
public:
    WriteLock(SharedExclusiveLock& lock);
    ~WriteLock();

private:
    SharedExclusiveLock& lock;
};