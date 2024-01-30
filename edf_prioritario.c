#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define poolsize 6
#define FAIL 0
#define SUCCESS 1
#define REPEAT 2

int head = 0, tail = 0;

typedef char (*ptfunction)(void);

typedef struct Process
{
    ptfunction function;
    int period;  
    int start;
    int priority;
    char *nome;
    bool run;
} Process;

Process *pool[poolsize];

char f1();
char f2();
char f3();
char f4();
char f5();
char fNull();
char KernelAddProc(Process *process);
void KernelInit();
void KernelLoop();
void KernelClock();
bool checkStartDraw();
void sortByPriority(int lookupValue);
void swap(Process **a, Process **b);
void debugPrint();
int period = 0;

    Process proc1 = {f1, 1, 0,0,"f1",true};
    Process proc2 = {f2, 1, 0,1,"f2",true};
    Process proc3 = {f3, 1, 0,2,"f3",true};
    Process proc4 = {f4, 4, 0,0,"f4",true};
    Process proc5 = {f5, 5, 0,0,"f5",true};
    Process procNull = {fNull, -1, -1,-1,"fNull",false};

int main()
{

    KernelInit();
    KernelAddProc(&proc1);
    KernelAddProc(&proc2);
    KernelAddProc(&proc3);
    KernelAddProc(&proc4);
    KernelAddProc(&proc5);
    
    KernelLoop();
    return 0;
}

void KernelInit()
{
    printf("\n KernelInit \n");
    Process procNull = {fNull, 0, 0, 0,"fNull",false};

    int i;
    for(i=0;i<poolsize;i++)
    {
        pool[i] = &procNull;
    }
}

char KernelAddProc(Process *newproc)
{
    printf("\n KernelAddProc \n");
    if ((tail + 1) % poolsize != head)
    {
        pool[tail] = newproc;
        pool[tail]->start += newproc->period;
        tail = (tail + 1) % poolsize;
        return SUCCESS;
    }
    else return FAIL;
}

void KernelClock()
{
    //printf("\n Clock \n");
    int i = head;
    while (i != tail)
    {
        pool[i]->start -= 1;
        i = (i + 1) % poolsize;
    }
}

void KernelLoop()
{
    printf("\n Kernel Loop \n");
    while(1)
    {
        
        if (head != tail)
        {
            debugPrint();
            int next = head;
            int adj = (next + 1) % poolsize;

            int ordIndex =0;
            while (adj != tail)
            {
                if (pool[next]->start > pool[adj]->start){
                    ordIndex +=1;
                    next = adj;
                    
                }
                adj = (adj + 1) % poolsize;
            }
                
            Process *temp;
            temp = pool[next];
            pool[next] = pool[head];
            pool[head] = temp;

            if(checkStartDraw()){
                printf("\nEmpate por start entre processos!\n");
                debugPrint();
            }
            if (pool[head]->start <= 0)
            {
                char ans = pool[head]->function();
                pool[head] = &procNull;
                printf("\n");
                if (ans == REPEAT)
                {
                    KernelAddProc(&proc5);
                    debugPrint();
                }
                head = (head + 1) % poolsize;
            }
        }
        KernelClock();
    }
}

//Troca processos de posição
void swap(Process **a, Process **b) {
    printf("\t swap");
    Process *temp = *a;
    *a = *b;
    *b = temp;
    return;
}
//Ordena os processos de acordo com o valor de prioridade
//Do maior para o menor, ou seja,
//Será executado primeiro aquele com o maior valor em prioridade
void sortByPriority(int lookupValue) {
    printf("\n sortByPriority: ");
    if(pool[head]->nome == "f5"){
        printf("ultima funcao f5");
    }
    for (int i = 0; i < poolsize - 1; i++) {
        for (int j = 0; j < poolsize - i - 1; j++) {
            if (    (pool[j]->start <= 0 && pool[j]->run) && 
                    (pool[j]->priority < pool[j + 1]->priority)  &&
                    (pool[j]->start == lookupValue)
                ) {
                swap(&pool[j], &pool[j + 1]);
            }
        }
    }
    return;
}

//Verifica se a existem processos empatados
//De acordo com o valor de start <= 0
//E que tenham a flag run = true
//Ou seja, processos ainda não executados ou em repeat
bool checkStartDraw() {
    //printf("\n checkStartDraw");
    int cont = 0;
    for (int i = 0; i < poolsize -1; i++) {
        if (pool[i]->start <= 0 && pool[i]->run) {
            for(int j = 0; j < poolsize -1; j++) {
                if(pool[j]->start == pool[i]->start){
                    cont++;
                    if (cont > 1) {
                        // Sort the pool array by priority from lowest to highest
                        sortByPriority(pool[j]->start);
                        return true;
                    }
                }
            }
        }
    }
    return false;
}

char fNull()
{
    return SUCCESS;
}

char f1()
{
    printf("\n******************************");
    printf("Executou funcao 1");
    printf("******************************\n");
    return SUCCESS;
}
char f2()
{
    printf("\n******************************");
    printf("Executou funcao 2");
    printf("******************************\n");
    return SUCCESS;
}
char f3()
{
    printf("\n******************************");
    printf("Executou funcao 3");
    printf("******************************\n");
    return SUCCESS;
}
char f4()
{
    printf("\n******************************");
    printf("Executou funcao 4");
    printf("******************************\n");
    return SUCCESS;
}
char f5()
{
    printf("\n******************************");
    printf("Executou funcao 5");
    printf("******************************\n");
    return SUCCESS;
}

void debugPrint(){
    printf("\n[head|tail]: [%d|%d]\n",head,tail);
    printf("================================================================================================");
    printf("\n nome \t | \t periodo \t | \t start \t | \t prioridade \t | \t executar? \t |\t");
    printf("\n______________________________________________________________________________________________");
    for (int i = 0; i < poolsize-1; i++)
    {
        printf("\n %s \t | \t %d \t \t | \t %d \t | \t %d \t \t | \t %d \t \t| \t", pool[i]->nome, pool[i]->period, pool[i]->start, pool[i]->priority, pool[i]->run);
    }
    printf("\n==============================================================================================");
    printf("\n");
    
}