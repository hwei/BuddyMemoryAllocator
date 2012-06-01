#include "BuddyMemoryAllocator.h"

IMemoryAllocator * BuddyMemoryAllocatorFactory::Create( size_t size )
{
    return BuddyMemoryAllocator::Create( size );
}
