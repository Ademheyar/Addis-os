#include <Kernel.h>

#ifndef __MMU_PAGING_H
#define __MMU_PAGING_H


extern void* kernel_end;

// 2MB Pages
//#define PAGE_SIZE 0x200000 (2*1024*1024)
#define PAGE_SIZE (2*1024*1024)
//#define PAGE_SIZE 4096

#define PAGE_PRESENT_CPL0 0x83
#define PAGE_PRESENT_CPL3 0x87


// Alignment related macro
#define IS_ALIGN(addr) ((((uint32_t)(addr)) | 0xFFFFF000) == 0)
#define PAGE_ALIGN(addr) ((((uint32_t)(addr)) & 0xFFFFF000) + 0x1000)

// Defone some address calculation macro
#define PAGEDIR_INDEX(vaddr) (((uint32_t)vaddr) >> 22)
#define PAGETBL_INDEX(vaddr) ((((uint32_t)vaddr) >>12) & 0x3ff)
#define PAGEFRAME_INDEX(vaddr) (((uint32_t)vaddr) & 0xfff)

// Paging register manipulation macro
#define SET_PGBIT(cr0) (cr0 = cr0 | 0x80000000)
#define CLEAR_PSEBIT(cr4) (cr4 = cr4 & 0xffffffef)

// Err code interpretation
#define ERR_PRESENT     0x1
#define ERR_RW          0x2
#define ERR_USER        0x4
#define ERR_RESERVED    0x8
#define ERR_INST        0x10

typedef struct page_dir_entry {
    unsigned int present    : 1;
    unsigned int rw         : 1;
    unsigned int user       : 1;
    unsigned int w_through  : 1;
    unsigned int cache      : 1;
    unsigned int access     : 1;
    unsigned int reserved   : 1;
    unsigned int page_size  : 1;
    unsigned int global     : 1;
    unsigned int available  : 3;
    unsigned int frame      : 20;
}page_dir_entry_t;

typedef struct page {
	unsigned int present:1;
	unsigned int rw:1;
	unsigned int user:1;
	unsigned int writethrough:1;
	unsigned int cachedisable:1;
	unsigned int accessed:1;
	unsigned int dirty:1;
	unsigned int pat:1;
	unsigned int global:1;
	unsigned int unused:3;
	unsigned int frame:20;

    unsigned int reserved   : 2;
    unsigned int reserved2  : 2;
    unsigned int available  : 3;
} __attribute__((packed)) page_t;

typedef struct page_table {
	page_t pages[1024];
} page_table_t;

typedef struct page_directory
{
    // The actual page directory entries(note that the frame number it stores is physical address)
	page_table_t *tables[1024];	/* 1024 pointers to page tables... */
    // We need a table that contains virtual address, so that we can actually get to the tables
    page_table_t * ref_tables[1024];
    uint64_t1 physical_tables[1024];	/* Physical addresses of the tables */
	uint64_t1 physical_address;	/* The physical address of physical_tables */
	int32_t ref_count;
} page_directory_t;

// Defined in entry.asm
extern page_directory_t * TEMP_PAGE_DIRECTORY;

// Defined in pmm.c
extern uint8_t * bitmap;
extern uint32_t  bitmap_size;

// Defined in paging.c
extern page_directory_t * kpage_dir;


typedef union pml4e_struct {
  uint64_t all;
  struct {
    uint64_t present         : 1;   // Page present in memory
    uint64_t writable        : 1;   // Read-only if clear, readwrite if set
    uint64_t user            : 1;   // Supervisor level only if clear
    uint64_t write_through   : 1;   // Write through flag
    uint64_t disable_cache   : 1;   // Disable cache 
    uint64_t accessed        : 1;   // Has the page been accessed since last refresh?
    uint64_t ignore_1        : 1;   // Ignored
    uint64_t mbz_1           : 1;   // must be zero
    uint64_t mbz_2           : 1;   // must be zero
    uint64_t avail_1         : 3;   // Available for OS to use
    uint64_t address         : 40;  // Address
    uint64_t avail_2         : 11;  // Available for OS to use
    uint64_t no_exec         : 1;   // Frame address (shifted right 12 bits)
  } fields;
} __attribute__((packed)) pml4e_t;

typedef union pdpe_struct {
  uint64_t all;
  struct {
    uint64_t present         : 1;   // Page present in memory
    uint64_t writable        : 1;   // Read-only if clear, readwrite if set
    uint64_t user            : 1;   // Supervisor level only if clear
    uint64_t write_through   : 1;   // Write through flag
    uint64_t disable_cache   : 1;   // Disable cache 
    uint64_t accessed        : 1;   // Has the page been accessed since last refresh?
    uint64_t ignore_1        : 1;   // Ignored
    uint64_t mbz_1           : 1;   // must be zero
    uint64_t ignore_2        : 1;   // Ignored
    uint64_t avail_1         : 3;   // Available for OS to use
    uint64_t address         : 40;  // Address
    uint64_t avail_2         : 11;  // Available for OS to use
    uint64_t no_exec         : 1;   // Frame address (shifted right 12 bits)
  } fields;
} __attribute__((packed)) pdpe_t;

typedef union pde_struct {
  uint64_t all;
  struct {
    uint64_t present         : 1;   // Page present in memory
    uint64_t writable        : 1;   // Read-only if clear, readwrite if set
    uint64_t user            : 1;   // Supervisor level only if clear
    uint64_t write_through   : 1;   // Write through flag
    uint64_t disable_cache   : 1;   // Disable cache 
    uint64_t accessed        : 1;   // Has the page been accessed since last refresh?
    uint64_t dirty           : 1;   // Ignored
    uint64_t mbo_1           : 1;   // Must be one
    uint64_t global          : 1;   // Global
    uint64_t avail_1         : 3;   // Available for OS to use
    uint64_t pat             : 1;   // PAT translation mode
    uint64_t mbz_1           : 8;   // Must be zero
    uint64_t address         : 31;  // Address
    uint64_t avail_2         : 11;  // Available for OS to use
    uint64_t no_exec         : 1;   // Frame address (shifted right 12 bits)
  } fields;
} __attribute__((packed)) pde_t;

extern pml4e_t pml4e[512];
extern pdpe_t  pdpe[512];
extern pde_t   pde[512];
extern pdpe_t  pdpe_user[512];
extern pde_t   pde_user[512];
extern pdpe_t  pdpe_krnluser[512];
extern pdpe_t  pde_krnluser[512];

void* alloc_frame_temp(uint64_t *phys_out);
void init_kernel_paging();
/*
void * virtual2phys(page_directory_t * dir, void * virtual_addr);

void * dumb_kmalloc(uint32_t size, int align);

void allocate_region(page_directory_t * dir, uint32_t start_va, uint32_t end_va, int iden_map, int is_kernel, int is_writable);

void allocate_page(page_directory_t * dir, uint32_t virtual_addr, uint32_t frame, int is_kernel, int is_writable);

void free_region(page_directory_t * dir, uint32_t start_va, uint32_t end_va, int free);

void free_page(page_directory_t * dir, uint32_t virtual_addr, int free);

void paging_init();

void switch_page_directory(page_directory_t * page_dir, uint32_t phys);

void enable_paging();

void * ksbrk(int size);

void copy_page_directory(page_directory_t * dst, page_directory_t * src);

page_table_t * copy_page_table(page_directory_t * src_page_dir, page_directory_t * dst_page_dir, uint32_t page_dir_idx, page_table_t * src);

void page_fault_handler(register_t * reg);

*/
#endif
