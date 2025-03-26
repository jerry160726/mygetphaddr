# Get the Physical Address of a Virtual Address of a process.

## Environment
>OS: Ubuntu 22.04  
>ARCH: X86_64  
>Source Version: 5.15.137  
## Linux 的 Virtual Address 轉 Physical Address
![01](https://imgur.com/n9PrODm.png)  
![02](https://imgur.com/iPA3kPi.png)  
> 這四個 page table 為：
> * 頁全域目錄 (Page Global Directory)(PGD)
> * 頁上級目錄 (Page Upper Directory)(PUD)
> * 頁中級目錄 (Page Middle Directory)(PMD)
> * 頁表 (Page Table)(PTE)
> 每一級 table 有3個 key description marco: shift, size, mask。  
> PGD, PUD, PMD, PTE 會分別由 pgd_t, pud_t, pmd_t, pte_t 來做描述。  

### current 指標
<https://elixir.bootlin.com/linux/v5.15.137/source/arch/x86/include/asm/current.h>  
```c
DECLARE_PER_CPU(struct task_struct *, current_task);
static __always_inline struct task_struct *get_current(void)
{
	return this_cpu_read_stable(current_task);
}
#define current get_current()
#endif /* __ASSEMBLY__ */
#endif /* _ASM_X86_CURRENT_H */
```
`#define current get_current()`
>current 是一個 macro，他被定義在 asm/current.h 內。  
>current 實際上就是 get_current() 函數。這個函式會回傳現在 CPU 所執行 process 的 task_struct。  

>首先 macro DECLARE_PER_CPU 的作用是將一個變數設定到每一個 CPU 內。  
>第一行的 DECLARE_PER_CPU 讓我們有 struct task_struct* 型態的 current_task 可以用。  
>get_current() 內所呼叫的 this_cpu_read_stable(current_task) 函式，作用是從 CPU 讀取 current_task 變數。  
>我們便能夠使用 current 來拿到當前 process 的 task_struct 了。  

> current 指標為儲存 task 的各種資料用的，而我們需要的是裡面的 mm 參數，而 mm 結構標示了一個 Process 的 Memory 管理訊息，包含：  
> * page table 資訊  
> * Stack, BSS, Data, Code…段的資訊  

## 連結
[筆記網站](https://hackmd.io/@Jyen024/HykY2ayeJl "顏呈安的hackmd")  
[專案網站](https://staff.csie.ncu.edu.tw/hsufh/COURSES/FALL2024/linux_project_1.html "專案網站")  



