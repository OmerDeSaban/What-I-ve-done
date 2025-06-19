/* This code is written for 2 functions, 1 of which either creates or destroys virtual memory mappings in
 * a page table, and the other checks if an address is mapped in a page table.
 * This code uses another file named "os.h", which would also be used by a file named "os.c". Both said
 * files should be in the same directory as this file.
 * "os.h" would declare shared functions and/or variables, while "os.c" would implement these functions:
 * 1) uint64_t alloc_page_frame(void) ; This function returns the physical page number of the allocated
 *    page. The content of said allocated page frame is all zeros.
 * 2) void* phys to virt(uint64 t phys addr) ; This function would be used to obtain a pointer to a
 *    physical address.
 * No pysical memory freeing would be done in this file.
 * 
 * Calculating how many levels we have here:
 * We are working with uint64_t, thus each entry's size is 64 bits = 8 bytes.
 * The page's size is 4KB = 4096 bytes = (512 * 8) bytes = 512 entries of uint64_t.
 * log_2 (512) = 9, thus each level handles 9 bits of the virtual address.
 * The virtual address uses 64 bits, the first 12 are the offset, and the last 7 are the sign.
 * Thus we are left with 64 - 12 - 7 = 45 bits that represent the levels.
 * 45 / 9 = 5, thus we have 5 levels here, indexing them from 0 to 4, where:
 * Level 0's bits are bits 12 through 20.
 * Level 1's bits are bits 21 through 29.
 * Level 2's bits are bits 30 through 38.
 * Level 3's bits are bits 39 through 47.
 * Level 4's bits are bits 48 through 56.
 * 
 * Here are some useful identities:
 * (111 1111) base 2 = (127) base 10 = (0x7F) base 16.
 * (1 1111 1111) base 2 = (511) base 10 = (0x1FF) base 16
 */


#include "os.h"


int validVPN(uint64_t vpn);
uint64_t *getLevelBits(uint64_t pt, uint64_t vpn, int level);
void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn);
uint64_t page_table_query(uint64_t pt, uint64_t vpn);


/** Validates that a given vpn is valid. A valid vpn consists of 64 bits, segmentated as follows:
 *  If we count the bits from right to left, where the right-most bit is the 0th bit, and the left most
 *  bit is the 63rd bit, then the 0th to 11th bits represent the offset, the 12th to the 56th bits
 *  represent the virtual page number, and the 57th to 63rd bit represent the sign, and would all be the
 *  same as the 56th bit.
 *  @param vpn The given vpn.
 *  @returns An integer denoting if the vpn is valid, where 1 means it is, and 0 means it isn't.  */
int validVPN(uint64_t vpn) {
    uint64_t bit_56, sign;

    bit_56 = (vpn >> 56) & 1;
    sign = vpn >> 57;

    return (bit_56 == 0 && sign == 0) || (bit_56 == 1 && sign == 0x7F);
}

/** Gets the 9 bits representing the current level's bits.
 *  @param pt The physical page number of the table root.
 *  @param vpn The virtual page number which the caller wishes to map or unmap.
 *  @param level The current level.
 *  @returns The bits representing the current level. */
uint64_t *getLevelBits(uint64_t pt, uint64_t vpn, int level) {
    uint64_t index, *vpt;

    index = (vpn >> (9 * level)) & 0x1ff;
    vpt = (uint64_t*)phys_to_virt(pt);

    return &vpt[index];
}

/** Creates or destroys virtual memory mappings in a page table.
 *  @param pt The physical page number of the table root.
 *  @param vpn The virtual page number which the caller wishes to map or unmap.
 *  @param ppn Either a NO_MAPPING number (declared in "os.h"), or the physical page number the vpn
 *    should be mapped to. If it is NO_MAPPING, the vpn's mapping should be destroyed (if it exists). */
void page_table_update(uint64_t pt, uint64_t vpn, uint64_t ppn) {
    uint64_t res, *entry, new_pt;
    int level;

    if (!validVPN(vpn)) { return; } /* Terminates the program immediately upon invalid vpn. */
    res = pt << 12;
    if (!res) { return; } /* NULL pointer handling. */

    for (level = 4; level > 0; level--) { /* Going through all levels 4 to 1: */
        entry = getLevelBits(res, vpn, level);

        if (!(*entry & 1)) { /* Invalid elem. */
        if (ppn == NO_MAPPING) { return; } /* No mapping to destroy. */
        new_pt = alloc_page_frame();
        *entry = (new_pt << 12) | 1;
        }
        res = *entry >> 12;  /* Removing the 12 first bits. */
        res = res << 12;  /* Adding 12 new 0 bits at the start. */
    }

    /* Do not forget about level 0: */
    entry = getLevelBits(res, vpn, 0);
    if (ppn == NO_MAPPING) { *entry = 0; }
    else { *entry = (ppn << 12) | 1; }
}

/** Checks if an address is mapped in a page table.
 *  @param pt The physical page number of the table root.
 *  @param vpn The virtual page number which the caller wishes to map or unmap.
 *  @returns The physical page number that vpn is mapped to, or NO_MAPPING if no mapping exists.  */
uint64_t page_table_query(uint64_t pt, uint64_t vpn) {
    uint64_t res;
    int level;

    if (!validVPN(vpn)) { return NO_MAPPING; }  /* Terminates the program immediately upon invalid vpn, and
                                                 * returns that no mapping exists. */
  
    res = pt << 12;
    for (level = 4; level >= 0; level--) { /* Going through all the levels: */
        res = *getLevelBits(res, vpn, level);

        if (!(res & 1)) { return NO_MAPPING; }  /* Invalid elem. */

        res = res >> 12;  /* Removing the 12 first bits. */
        res = res << 12;  /* Adding 12 new 0 bits at the start. */
    }

    return res >> 12;
}