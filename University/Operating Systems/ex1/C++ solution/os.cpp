#define _GNU_SOURCE

#include <cassert>
#include <cstdlib>
#include <cstdio>
#include <err.h>
#include <sys/mman.h>
#include <cstdint>

#include "os.h"

/* 2^20 pages ought to be enough for anybody */
#define NPAGES	(1024*1024)

static void* pages[NPAGES];

uint64_t alloc_page_frame(void)
{
	static uint64_t nalloc = 0;
	uint64_t ppn;
	void* va;

	if (nalloc == NPAGES) { errx(1, "out of physical memory"); }

	/* OS memory management isn't really this simple */
	ppn = nalloc;
	nalloc++;

	va = mmap(nullptr, 4096, PROT_READ|PROT_WRITE, MAP_PRIVATE|MAP_ANONYMOUS, -1, 0);
	if (va == MAP_FAILED) { err(1, "mmap failed"); }

	pages[ppn] = va;	
	return ppn;
}

void* phys_to_virt(uint64_t phys_addr)
{
	uint64_t ppn = phys_addr >> 12;
	uint64_t off = phys_addr & 0xfff;
	void* va = nullptr;

	if (ppn < NPAGES) { va = static_cast<char*>(pages[ppn]) + off; }

	return va;
}

int main(int argc, char **argv)
{
	uint64_t pt = alloc_page_frame();

	assert(page_table_query(pt, 0xcafe) == NO_MAPPING);
	page_table_update(pt, 0xcafe, 0xf00d);
	assert(page_table_query(pt, 0xcafe) == 0xf00d);
	page_table_update(pt, 0xcafe, NO_MAPPING);
	assert(page_table_query(pt, 0xcafe) == NO_MAPPING);

	return 0;
}