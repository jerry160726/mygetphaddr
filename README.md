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
>current 是一個 macro，他被定義在 asm/current.h 內。current 實際上就是 get_current() 函數。這個函式會回傳現在 CPU 所執行 process 的 task_struct。  

>首先 macro DECLARE_PER_CPU 的作用是將一個變數設定到每一個 CPU 內。  
>第一行的 DECLARE_PER_CPU 讓我們有 struct task_struct* 型態的 current_task 可以用。  
>get_current() 內所呼叫的 this_cpu_read_stable(current_task) 函式，作用是從 CPU 讀取 current_task 變數。  
>我們便能夠使用 current 來拿到當前 process 的 task_struct 了。  

> current 指標為儲存 task 的各種資料用的，而我們需要的是裡面的 mm 參數，而 mm 結構標示了一個 Process 的 Memory 管理訊息，包含：  
> * page table 資訊  
> * Stack, BSS, Data, Code…段的資訊  

## 流程
1. pgd_offset: 根據目前的 Virtual Address 和目前 Process 的資料結構 task_struct，存取其中的 mm point。
2. mm point 中儲存著 mm_struct 位置，而 mm_struct 儲存該 Process 虛擬位置資料的結構，在該結構中我們就可以找到 pgd 的初始位置。使用 pgd_offset 即可存取 pgd page 中的 pgd entry。   
(entry 內容為 pud table 的 base address)
3. pud_offset: 根據透過 pgd_offset 得到的 pgd entry 和 Virtual Address，可得到 pud entry。  
(entry 內容為 pmd table 的 base address)
4. pmd_offset: 根據 pud entry 的內容和 Virtual Address，可得到 pte table 的 base address。
5. pte_offset: 根據 pmd entry 的內容與 Virtual Address，可得到 pte 的 base address。
6. 將從 pte 得到的 Base Address 與 Mask(0xf…fff000)做 AND 運算，即可得到 Page 的 Base Physical Address。
7. Virtual Address 與 ~Mask(0x0…000fff)做 AND 運算得到 offset，再與 Page 的 base Physical Address 做 OR 運算即可得到轉換過後且完整的 Physical Address。

## 執行結果 - Question 1
![03](https://imgur.com/3poTqPE.jpeg)  

## 觀察結果
### 1. Before Fork (fork()之前的輸出)
![04](https://imgur.com/hc5z6dG.jpeg) 
-   **Virtual Address**：`0x563ab6a04010` 是全域變數 `global_a` 的 Virtual Address，即 Process 內存中的 Logical Memory 位置，由系統為該變數分配。
-   **Physical Address**：`0xcf696010` 是 `global_a` 對應的 Physical Address，即 Physical Memory 的實際存儲地址。
-   **PID**：`pid=4371` 是 Parent Process 的 pid，表示當前正在運行的 Parent Process。

在 `fork()` 之前，Parent Process 擁有 `global_a` 的 Virtual Address 和對應的 Physical Address。

### 2. After Fork by parent (fork()之後的輸出)
![05](https://imgur.com/M6fiGjS.jpeg) 
-   **Virtual Address 和 Physical Address 沒有變化**：Parent Process 再次獲得 `global_a` 的 Virtual Address 和 Physical Address，這兩者都沒有變化。
-   **PID**：`pid=4371`，Parent Process 的 pid 仍然是 `pid=4371`。

在 `fork()` 之後，Parent Process 仍然擁有對 `global_a` 的控制，並且它的 Virtual Address 和對應的 Physical Address 保持不變，代表 Parent Process 並沒有對 `global_a` 進行任何修改。

### 3. After Fork by child (fork()之後，child process的輸出)
![06](https://imgur.com/zqVVFJb.jpeg) 
- **Virtual Address 沒有變化**：Child Process 中的 `global_a`  的 Virtual Address 仍然是 `0x563ab6a04010`。
- **Physical Address 沒有變化**：Child Process 中的 `global_a` 對應的 Physical Address 仍然是 `0xcf696010`，與 Parent Process 共享同一個 Physical Address。
- **PID**：`pid=4373`，這是 Child Process 的 pid。

在 `fork()` 之後，Child Process 繼承了 Parent Process 的記憶體空間，Parent Process 與 Child Process 共享相同的 Virtual Address 和 Physical Address。這是因為在 `fork()` 之後，兩個 Process 暫時共享相同的 Physical Memory Page。

### 4. Test Copy on Write in child (觸發CoW時，child process 的輸出)
![07](https://imgur.com/20LVLdV.jpeg) 
- **Virtual Address 沒有變化**：Child Process 中的 `global_a` 的 Virtual Address 仍然是`0x563ab6a04010`。
- **Physical Address 發生變化**：當 Child Process 修改了 `global_a` 的值後，對應的 Physical Address 變成了`0x7001d010`，表示觸發了 **Copy-on-Write（CoW）機制**。
- **PID**：`pid=4373`，這是 Child Process 的 pid。

在 Child Process 中，當 `global_a` 被修改時，Linux 的 Copy-on-Write 觸發，Kernel 會為 Child Process 分配一個新的 Physical Address，這樣 Parent Process 和 Child Process 不再共享同一個 Physical Memory Page。因此 Child Process 的 Physical Address 變成了`0x7001d010`。  

### What is Copy-on-Write?
-   `fork()` 後 Parent Process 和 Child Process 共享 Physical Memory：在 `fork()` 調用之後，Parent Process 和 Child Process 共享相同的 Virtual Address 與 Physical Memory 而節省資源，因為如果記憶體沒有被修改，它們可以共享相同的 Physical Page。
-   **觸發Cow**：當 Child Process 嘗試修改共享的 Memory Page（例如 `global_a`）時，Kernel 會分配一個新的 Physical Page，這樣 Parent Process 和 Child Process 就會分別擁有自己獨立的 Physical Memory。這個過程叫做==寫入時複製（Copy-on-Write, CoW）==。而 Virtual Address 保持不變。
-   **Virtual Address 保持不變**：不管是 Parent Process  還是 Child Process，修改 `global_a` 時，它們的 Virtual Address 都不會變化。變化的是這個 Virtual Address 所對應的 Physical Address。  

[筆記網站](https://hackmd.io/@Jyen024/HykY2ayeJl "顏呈安的hackmd")  
[專案網站](https://staff.csie.ncu.edu.tw/hsufh/COURSES/FALL2024/linux_project_1.html "專案網站")  