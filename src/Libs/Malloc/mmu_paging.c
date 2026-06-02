#include <Libs/Malloc/Mmu_paging.h>
#include <Libs/Malloc/Mmu_frames.h>
#include <Libs/Malloc/Mmu_heap.h>
#include <x86.h>
#include <Kernel.h>
#include <Addis/Libs/String/String.h>



pml4e_t pml4e[512] __attribute__((aligned(4096)));
pdpe_t  pdpe[512] __attribute__((aligned(4096)));
pde_t   pde[512] __attribute__((aligned(4096)));

pdpe_t  pdpe_user[512] __attribute__((aligned(4096)));
pde_t   pde_user[512] __attribute__((aligned(4096)));

pdpe_t  pdpe_krnluser[512] __attribute__((aligned(4096)));
pdpe_t  pde_krnluser[512] __attribute__((aligned(4096)));

static void init_paging_kernel() {
  // Setup new PAGING DIRECTORY
  memset(&pml4e, 0, sizeof(pml4e_t) * 512);
  memset(&pdpe, 0, sizeof(pdpe_t) * 512);
  memset(&pde, 0, sizeof(pde_t) * 512);
  memset(&pdpe_krnluser, 0, sizeof(pdpe_t) * 512);
  memset(&pde_krnluser, 0, sizeof(pde_t) * 512);

  // Kernel space
  pml4e[0].all = TO_PHYS_U64(&pdpe) | 3;
  pml4e[0x100].all = TO_PHYS_U64(&pdpe) | 3;    // KERNEL_VMA: Present + Write (0x100 KERNEL_START_MEMORY)
  pml4e[0x1C0].all = TO_PHYS_U64(&pdpe_krnluser) | 3; // KERNEL_USER TO KRNLVMA: Present + Write (0x1C0 KERNEL_USER_VIEW_START_MEMORY)
  pdpe[0].all = TO_PHYS_U64(&pde) | 3; 
  pdpe_krnluser[0].all = TO_PHYS_U64(&pde_krnluser) | 3; 

  uint64_t num_pages = TO_PHYS_U64(heap_end) / PAGE_SIZE;
  DEBUG("MMU[paging]: Krnl space pages needed: %i\n", num_pages);

  for (uint64_t i=0; i<=num_pages; i++) {
    pde[i].all = (i * PAGE_SIZE) | PAGE_PRESENT_CPL0; // Presetn + Write + Large (2MB)
  }
}

//static 
void init_paging_user() {
  // User space
  memset(&pdpe_user, 0, sizeof(pdpe_t) * 512);
  memset(&pde_user, 0, sizeof(pde_t) * 512);

  pml4e[0].all = TO_PHYS_U64(&pdpe_user) | 0x7; // USER_VMA: Present + Write + CPL3
  pdpe_user[0].all = TO_PHYS_U64(&pde_user) | 0x07; 
}

void init_kernel_paging() {
  DEBUG("MMU[paging] PAGING: init ...");
  init_mmu_frames(1024 * 1024 * 1024); // 1024Megs(1GB) for now - TODO : autodetect);
  
  init_paging_kernel();
  init_paging_user();

  x86_set_cr3(TO_PHYS_U64(pml4e));

  init_mmu_heap();
}
