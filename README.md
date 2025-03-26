# Get the Physical Address of a Virtual Address of a process.

## Environment
>OS: Ubuntu 22.04
>ARCH: X86_64
>Source Version: 5.15.137
## Linux 的 Virtual Address 轉 Physical Address
![01](https://imgur.com/n9PrODm)  
![02](https://imgur.com/iPA3kPi)  
> 這四個 page table 為：
> * 頁全域目錄 (Page Global Directory)(PGD)
> * 頁上級目錄 (Page Upper Directory)(PUD)
> * 頁中級目錄 (Page Middle Directory)(PMD)
> * 頁表 (Page Table)(PTE)
> 每一級 table 有3個 key description marco: shift, size, mask。  
> PGD, PUD, PMD, PTE 會分別由 pgd_t, pud_t, pmd_t, pte_t 來做描述。  

## 連結
[筆記網站](https://hackmd.io/@Jyen024/HykY2ayeJl "顏呈安的hackmd")  
[專案網站](https://staff.csie.ncu.edu.tw/hsufh/COURSES/FALL2024/linux_project_1.html "專案網站")  



