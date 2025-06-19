/**
 * @file pt.cpp
 * @brief The required pt.c file as a C++ file instead.
 * 
 * Number of page table levels = 5
 * We have 4KB = 4096 bytes in the entire page table structure.
 * Each entry consists of 64 bits = 8 bytes, thus there are 4096/8 = 512 entries.
 * log_2 (512) = 9, thus each level handles 9 bits of the virtual adress, and each phisical page number
 * takes up to 9 bits.
 * The virtual adress's first 12 and last 7 bits do not represent the virtual page number,
 * hence there are only 64-12-7 = 45 bits that do represent the virtual page number.
 * Therefore, there are 45/9 = 5 levels, indexed from 0 to 4.
 * Each level's bits are indexed as follows:
 * Level 0's bits are bits 12-20.
 * Level 1's bits are bits 21-29.
 * Level 2's bits are bits 30-38.
 * Level 3's bits are bits 39-47.
 * Level 4's bits are bits 48-56.
 * 
 * Useful identities:
 * (111 1111) base 2 = (127) base 10 = (0x7F) base 16. This is what the sign should be equal to, in bases
 * 10 and 16, when the 56th bit of a virtual address is 1.
 * (1 1111 1111) base 2 = (511) base 10 = (0x1FF) base 16. This is what the level bits would be equal to,
 * in bases 10 and 16, if they were all 1s.
 */

#include "os.h"

bool validVPN(uint64_t vpn) {
    uint64_t bit56, sign;

    bit56 = (vpn >> 56) & 1;
    sign = vpn >> 57;

    return ((bit56 == 0 && sign == 0) || (bit56 == 1 && sign == 0x7F));
}

uint64_t* getLevelBits(uint64_t curr_pt, uint64_t vpn, int level) {
    uint64_t index, *vpt;

    if ((level < 0) || (level > 4)) { return nullptr; } // Might be changed later to show some error.

    index = (vpn >> (12 + 9 * level)) & 0x1FF;
    vpt = (uint64_t*) phys_to_virt(curr_pt);

    return &vpt[index];
}

/**Creates/Destroys virtual memory mappings in a page table.
 * @param pt The physical page number of the page table root. Has been previously returned by
 * alloc_page_frame().
 * @param vpn The virtual page number the caller wishes to map/unmap.
 * @param ppn Can be one of two cases. If ppn is equal to a special NO_MAPPING value (defined in os.h),
 * then vpn’s mapping (if it exists) should be destroyed. Otherwise, ppn specifies the physical page
 * number that vpn should be mapped to.
 */
void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn) {
    uint64_t curr_pt, *entry;

    if (!validVPN(vpn)) { return; } // Might be changed later to show some error.
    if ((pt == 0) || (pt >= 1024*1024)) { return; } // Might be changed later to show some error.

    curr_pt = pt << 12;

    for (int level = 4; level > 0; level--) {
        entry = getLevelBits(curr_pt, vpn, level);
        if (!(*entry & 1)) {
            if (ppn == NO_MAPPING) { return; }  // No valid mapping to destroy.
            *entry = (alloc_page_frame() << 12) | 1;
        }
        curr_pt = *entry >> 12;
        curr_pt = curr_pt << 12;
    }

    entry = getLevelBits(curr_pt, vpn, 0);
    if (ppn == NO_MAPPING) { *entry = 0; }
    else { *entry = (ppn << 12) | 1; }
}

/**Queries the mapping of a virtual page number in a page table.
 * @param pt The physical page number of the page table root. Has been previously returned by
 * alloc_page_frame().
 * @param vpn The virtual page number the caller wishes to query.
 * @returns The physical page number that vpn is mapped to, or NO MAPPING if no mapping exists.
 */
uint64_t page_table_query(uint64_t pt, uint64_t vpn) {
    uint64_t curr_pt, *entry;

    if (!validVPN(vpn)) { return NO_MAPPING; }
    if ((pt == 0) || (pt >= 1024*1024)) { return NO_MAPPING; }

    curr_pt = pt << 12;

    for (int level = 4; level > 0; level--) {
        entry = getLevelBits(curr_pt, vpn, level);
        if (!(*entry & 1)) { return NO_MAPPING; }
        curr_pt = *entry >> 12;
        curr_pt = curr_pt << 12;
    }

    entry = getLevelBits(curr_pt, vpn, 0);
    if (!(*entry & 1)) { return NO_MAPPING; }
    return *entry >> 12;
}