#ifndef BUDDYMEMORYALLOCATOR_H
#define BUDDYMEMORYALLOCATOR_H

#include <vector>
#include "MemoryAllocator.h"

typedef signed char int8_t;
typedef unsigned int size_t;

class BuddyMemoryAllocator : public IMemoryAllocator
{
public:
    static BuddyMemoryAllocator * Create( size_t size );
    ~BuddyMemoryAllocator() { }
    size_t Allocate( size_t size );
    void Free( size_t offset );
    size_t Size( size_t offset ) const;
    void Dump() const;
protected:
    BuddyMemoryAllocator( size_t level );
    int8_t m_level;
    std::vector< int8_t > m_longest;
    size_t m_leftNode( size_t index ) const { return ( index << 1 ) + 1; }
    size_t m_rightNode( size_t index ) const { return ( index << 1 ) + 2; }
    size_t m_parentNode( size_t index ) const
    {
        return ( ( index + 1 ) >> 1 ) - 1;
    }
};

#endif /*BUDDYMEMORYALLOCATOR_H*/
