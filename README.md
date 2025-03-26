# Get the Physical Address of a Virtual Address of a process.

## Environment
>OS: Ubuntu 22.04
>ARCH: X86_64
>Source Version: 5.15.137
## Linux �� Virtual Address �� Physical Address
![01](https://imgur.com/n9PrODm)  
![02](https://imgur.com/iPA3kPi)  
> �o�|�� page table ���G
> * ������ؿ� (Page Global Directory)(PGD)
> * ���W�ťؿ� (Page Upper Directory)(PUD)
> * �����ťؿ� (Page Middle Directory)(PMD)
> * ���� (Page Table)(PTE)
> �C�@�� table ��3�� key description marco: shift, size, mask�C  
> PGD, PUD, PMD, PTE �|���O�� pgd_t, pud_t, pmd_t, pte_t �Ӱ��y�z�C  

## �s��
[���O����](https://hackmd.io/@Jyen024/HykY2ayeJl "�C�e�w��hackmd")  
[�M�׺���](https://staff.csie.ncu.edu.tw/hsufh/COURSES/FALL2024/linux_project_1.html "�M�׺���")  



