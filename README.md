# Get the Physical Address of a Virtual Address of a process.

## Environment
>OS: Ubuntu 22.04  
>ARCH: X86_64  
>Source Version: 5.15.137  
## Linux �� Virtual Address �� Physical Address
![01](https://imgur.com/n9PrODm.png)  
![02](https://imgur.com/iPA3kPi.png)  
> �o�|�� page table ���G
> * ������ؿ� (Page Global Directory)(PGD)
> * ���W�ťؿ� (Page Upper Directory)(PUD)
> * �����ťؿ� (Page Middle Directory)(PMD)
> * ���� (Page Table)(PTE)
> �C�@�� table ��3�� key description marco: shift, size, mask�C  
> PGD, PUD, PMD, PTE �|���O�� pgd_t, pud_t, pmd_t, pte_t �Ӱ��y�z�C  

### current ����
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
>current �O�@�� macro�A�L�Q�w�q�b asm/current.h ���C  
>current ��ڤW�N�O get_current() ��ơC�o�Ө禡�|�^�ǲ{�b CPU �Ұ��� process �� task_struct�C  

>���� macro DECLARE_PER_CPU ���@�άO�N�@���ܼƳ]�w��C�@�� CPU ���C  
>�Ĥ@�檺 DECLARE_PER_CPU ���ڭ̦� struct task_struct* ���A�� current_task �i�H�ΡC  
>get_current() ���ҩI�s�� this_cpu_read_stable(current_task) �禡�A�@�άO�q CPU Ū�� current_task �ܼơC  
>�ڭ̫K����ϥ� current �Ӯ����e process �� task_struct �F�C  

> current ���Ь��x�s task ���U�ظ�ƥΪ��A�ӧڭ̻ݭn���O�̭��� mm �ѼơA�� mm ���c�ХܤF�@�� Process �� Memory �޲z�T���A�]�t�G  
> * page table ��T  
> * Stack, BSS, Data, Code�K�q����T  

## �s��
[���O����](https://hackmd.io/@Jyen024/HykY2ayeJl "�C�e�w��hackmd")  
[�M�׺���](https://staff.csie.ncu.edu.tw/hsufh/COURSES/FALL2024/linux_project_1.html "�M�׺���")  



