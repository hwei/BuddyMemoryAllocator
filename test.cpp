#include <stdio.h>
#include "MemoryAllocator.h"

static size_t test_alloc( IMemoryAllocator & allocator, size_t sz)
{
    size_t r = allocator.Allocate( sz );
    printf( "alloc %d ( sz= %d )\n", r, sz );
    allocator.Dump();
    return r;
}

static void test_free( IMemoryAllocator & allocator, size_t addr )
{
    allocator.Free( addr );
    printf( "free %d\n", addr );
    allocator.Dump();
}

static void test_size( IMemoryAllocator & allocator, size_t addr )
{
    size_t s = allocator.Size( addr );
    printf( "size %d (sz = %d)\n", addr, s );
}

int main()
{
    IMemoryAllocator * pAllocator =
        BuddyMemoryAllocatorFactory::Create( 32 );
    pAllocator->Dump();
    size_t m1 = test_alloc( *pAllocator, 4 );
    test_size( *pAllocator, m1 );
    size_t m2 = test_alloc( *pAllocator, 9 );
    test_size( *pAllocator, m2 );
    size_t m3 = test_alloc( *pAllocator, 3 );
    test_size( *pAllocator, m3 );
    size_t m4 = test_alloc( *pAllocator, 7 );
    test_size( *pAllocator, m4 );
    test_free( *pAllocator, m3 );
    test_free( *pAllocator, m1 );
    test_free( *pAllocator, m4 );
    test_free( *pAllocator, m2 );

    size_t m5 = test_alloc( *pAllocator, 32 );
    test_free( *pAllocator, m5 );

    size_t m6 = test_alloc( *pAllocator, 0 );
    test_free( *pAllocator, m6 );

    delete pAllocator;
    return 0;
}
