#include "BuddyMemoryAllocator.h"
#include <stdio.h>
#include <cassert>

template< typename T >
bool IsPowerOf2( T x ) { return ! ( x & ( x - 1 ) ); }

template< typename T >
T FixSize( T size ) {
  size |= size >> 1;
  size |= size >> 2;
  size |= size >> 4;
  size |= size >> 8;
  size |= size >> 16;
  return size + 1;
}

template< typename T >
T Max( T a, T b )
{
    return a > b ? a : b;
}

static int Log2( uint32_t v )
{
    static const int MultiplyDeBruijnBitPosition[32] =
    {
      0, 9, 1, 10, 13, 21, 2, 29, 11, 14, 16, 18, 22, 25, 3, 30,
      8, 12, 20, 28, 15, 17, 24, 7, 19, 27, 23, 6, 26, 5, 4, 31
    };
    v |= v >> 1; // first round down to one less than a power of 2
    v |= v >> 2;
    v |= v >> 4;
    v |= v >> 8;
    v |= v >> 16;
    return MultiplyDeBruijnBitPosition[
        static_cast< uint32_t >( v * 0x07C4ACDDU ) >> 27 ];
}

BuddyMemoryAllocator * BuddyMemoryAllocator::Create( size_t size )
{
    if( size == 0 )
        return NULL;
    int8_t level = Log2( size );
    if( size != 1 << level )
        return NULL;
    return new BuddyMemoryAllocator( level );
}

BuddyMemoryAllocator::BuddyMemoryAllocator( size_t level )
    : m_level( level )
{
    size_t nodeCount = ( 1 << ( level + 1 ) ) - 1;
    m_longest.reserve( nodeCount );
    int8_t nodeLevel = level;
    while( true )
    {
        for( int i = 1 << ( level - nodeLevel ); i > 0; --i )
        {
            m_longest.push_back( nodeLevel );
        }
        if( nodeLevel == 0 )
            break;
        --nodeLevel;
    }
}

size_t BuddyMemoryAllocator::Allocate( size_t size )
{
    int8_t level = size <= 1 ? 0 : Log2( size - 1 ) + 1;

    size_t index = 0;
    if( m_longest[ index ] < level )
        return -1;

    for( int8_t nodeLevel = m_level; nodeLevel != level; --nodeLevel )
    {
        size_t leftIndex = m_leftNode( index );
        if( m_longest[ leftIndex ] >= level )
            index = leftIndex;
        else
            index = m_rightNode( index );
    }

    m_longest[ index ] = -1;
    size_t offset = ( index + 1 - ( 1 << ( m_level - level ) ) ) << level ;

    while (index)
    {
        index = m_parentNode( index );
        int8_t left_longest = m_longest[ m_leftNode( index ) ];
        int8_t right_longest = m_longest[ m_rightNode( index ) ];
        m_longest[ index ] = Max( left_longest, right_longest );
    }
    return offset;
}

void BuddyMemoryAllocator::Free( size_t offset )
{
    size_t fullSize = 1 << m_level;
    assert( offset < fullSize );
    size_t index = offset - 1 + fullSize;
    int8_t level = 0;
    while( m_longest[ index ] != -1 )
    {
        if( index == 0 )
            return;
        ++level;
        index = m_parentNode( index );
    }
    m_longest[ index ] = level;
    while ( index != 0 )
    {
        index = m_parentNode( index );
        int8_t left_longest = m_longest[ m_leftNode( index ) ];
        int8_t right_longest = m_longest[ m_rightNode( index ) ];
        if( left_longest == level && right_longest == level )
            m_longest[ index ] = level + 1;
        else
            m_longest[ index ] = Max( left_longest, right_longest );
        ++level;
    }
}

size_t BuddyMemoryAllocator::Size( size_t offset ) const
{
    size_t fullSize = 1 << m_level;
    assert( offset < fullSize );
    size_t index = offset - 1 + fullSize;
    int8_t level = 0;
    while( m_longest[ index ] != -1 )
    {
        if( index == 0 )
            return -1;
        ++level;
        index = m_parentNode( index );
    }
    return 1 << level;
}

void BuddyMemoryAllocator::Dump() const
{
    size_t index = 0;
    for( int8_t level = m_level; level >= 0; --level )
    {
        size_t nodeCount = 1 << ( m_level - level );
        size_t spaceSize = ( 1 << ( level + 1 ) ) - 2;
        for( size_t i = 0; i != nodeCount; ++i )
        {
            if( i != 0)
            {
                for( size_t j = 0; j < spaceSize; ++j )
                    putchar(' ');
            }
            printf( "%2d", m_longest[ index++ ] );
        }
        putchar('\n');
    }
}
