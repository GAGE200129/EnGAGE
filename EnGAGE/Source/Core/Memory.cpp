#include "pch.hpp"
#include "Memory.hpp"

static struct Allocations
{
    uint64_t totalAllocated = 0;
    uint64_t totalFreed = 0;

    uint64_t getCurrentUsage() { return totalAllocated - totalFreed; }

} sAllocations;

void* operator new(size_t size)
{
    sAllocations.totalAllocated += size;
    return malloc(size);
}

void operator delete(void* memory, size_t size)
{
    sAllocations.totalFreed += size;
    free(memory);
}

uint64_t Core::Memory::getUsage()
{
    return sAllocations.getCurrentUsage();
}
