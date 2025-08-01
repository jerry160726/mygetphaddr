#include <linux/kernel.h>
#include <linux/syscalls.h>
#include <linux/mm.h>       // Data structure and functions related to memory management
#include <linux/pid.h>      // To obtain process info
#include <linux/sched.h>    // Definition of process control area
#include <linux/highmem.h>  // Functions related to page table operations
#include <asm/page.h>       // Functions related to page table operations

SYSCALL_DEFINE1(my_get_physical_addresses, void *, vaddr) {
    struct task_struct *task;   // 表示process的核心資料結構
    struct mm_struct *mm;       // 描述該process的Virtual Memory空間
    pgd_t *pgd;
    p4d_t *p4d;
    pud_t *pud;
    pmd_t *pmd;
    pte_t *pte;
    struct page *page;          // Physical Memory Page Frame
    unsigned long physical_address;     // 最後算出來的physical address（結果）

    task = current; // 取得目前在執行的process的task_struct
    mm = task->mm;  // 代表該process的Virtual Memory空間

    // 從virtual address中取出PGD Index並取得PGD entry
    pgd = pgd_offset(mm, (unsigned long)vaddr);
    if (pgd_none(*pgd) || pgd_bad(*pgd)) {
        return (void *)0;
    }
    printk("pgd address: %p\n", pgd);

    // 從vaddr中取出P4D index，並以PGD entry為起點，計算出對應的P4D entry
    p4d = p4d_offset(pgd, (unsigned long)vaddr);
    if (p4d_none(*p4d) || p4d_bad(*p4d)) {
        return (void *)0;
    }
    printk("p4d address: %p\n", p4d);

    // p4d是儲存的是下一層頁表（PUD Table）的起始位址
    // 從vaddr中取出PUD index，用index算出PUD entry的位置（base address + index * entry size）
    pud = pud_offset(p4d, (unsigned long)vaddr);
    if (pud_none(*pud) || pud_bad(*pud)) {
        return (void *)0;
    }
    printk("pud address: %p\n", pud);

    // 從虛擬位址vaddr 中擷取PMD index，搭配PUD entry中的Page Frame address（指向PMD Table），
    // 來計算對應的PMD entry位址
    pmd = pmd_offset(pud, (unsigned long)vaddr);
    if (pmd_none(*pmd) || pmd_bad(*pmd)) {
        return (void *)0;
    }
    printk("pmd address: %p\n", pmd);

    // 從pmd指定的Page Table base address，再用vaddr中的PTE index，找出PTE entry（指向實體page的entry）
    pte = pte_offset_map(pmd, (unsigned long)vaddr);
    if (!pte || pte_none(*pte)) {
        return (void *)0;
    }
    printk("pte address: %p\n", pte);

    // 從PTE中解析出struct page結構，代表此virtual address對應的實體Page Frame
    page = pte_page(*pte);
    if (!page) {
        return (void *)0;
    }
    printk("page address: %p\n", page);

    // 取得此Page Frame的PFN，左移PAGE_SHIFT（通常是12 bits）就變成此Page Frame的起始地址
    physical_address = page_to_pfn(page) << PAGE_SHIFT;
    
    // vaddr & ~PAGE_MASK：取出virtual address在頁內的offset（0~4095）
    // 將offset加回Page Frame base address，得到完整physical address。
    physical_address |= ((unsigned long)vaddr) & ~PAGE_MASK;

    return (void *)physical_address;
}
