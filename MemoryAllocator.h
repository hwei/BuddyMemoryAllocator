typedef unsigned int size_t;

class IMemoryAllocator
{
public:
    virtual ~IMemoryAllocator() { }
    virtual size_t Allocate( size_t size ) = 0;
    virtual void Free( size_t offset ) = 0;
    virtual size_t Size( size_t offset ) const = 0;
    virtual void Dump() const = 0;
};

class BuddyMemoryAllocatorFactory
{
public:
    static IMemoryAllocator * Create( size_t size );
};
