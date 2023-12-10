#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "getargs.h"
#include "relatorio.h"

typedef struct
{
    bool valida; // se esta em memoria
    int last_accessed; // para LRU
    bool R;                   // para NRU
    bool M;                   // para NRU
} Page;

typedef struct
{
    int num_pages; // número total de páginas

    Page *pages; // array de páginas
    /* Guarda todas as páginas */
} PageTable;       // tabela de páginas (mapeia endereços virtuais para endereços físicos)

void reset_reference_bits(PageTable *pagetable, int max_tabela) {
    for (int i = 0; i < max_tabela; i++) {
        pagetable->pages[i].R = false;
    }
}

int lru(PageTable pagetable, int deslocamento) {
    int menor = pagetable.pages[0].last_accessed; // last_accessed da primeira página, para comparar com o resto
    int menor_idx = 0;

    for (int j = 1; j < pow(2, 32 - deslocamento); j++) {
        if (pagetable.pages[j].valida && pagetable.pages[j].last_accessed < menor) {
            menor = pagetable.pages[j].last_accessed;
            menor_idx = j;
        }
    }

    return menor_idx;
}

int nru(PageTable pagetable, int max_tabela) {
    int classe0 = -1;
    int classe1 = -1;
    int classe2 = -1;
    int classe3 = -1;

    for (int i = 0; i < max_tabela; i++) {
        if (pagetable.pages[i].valida == true) {
            if (pagetable.pages[i].R == false && pagetable.pages[i].M == false) {
                classe0 = i;
                break;
            }
        }
    }

    if (classe0 == -1) {
        for (int i = 0; i < max_tabela; i++) {
            if (pagetable.pages[i].valida == true) {
                if (pagetable.pages[i].R == false && pagetable.pages[i].M == true) {
                    classe1 = i;
                    break;
                }
            }
        }
    }

    if (classe0 == -1 && classe1 == -1) {
        for (int i = 0; i < max_tabela; i++) {
            if (pagetable.pages[i].valida == true) {
                if (pagetable.pages[i].R == true && pagetable.pages[i].M == false) {
                    classe2 = i;
                    break;
                }
            }
        }
    }

    if (classe0 == -1 && classe1 == -1 && classe2 == -1) {
        for (int i = 0; i < max_tabela; i++) {
            if (pagetable.pages[i].valida == true) {
                if (pagetable.pages[i].R == true && pagetable.pages[i].M == true) {
                    classe3 = i;
                    break;
                }
            }
        }
    }

    if (classe0 != -1) {
        return classe0;
        printf("classe0\n");
    } else if (classe1 != -1) {
        return classe1;
        printf("classe1\n");
    } else if (classe2 != -1) {
        return classe2;
        printf("classe2\n");
    } else if (classe3 != -1) {
        return classe3;
        printf("classe3\n");
    } else {
        printf("Erro ao escolher a página para trocar.\n");
        exit(EXIT_FAILURE);
    }
}

relatorio_t le_arquivo(args_t args)
{
    FILE *arquivo = fopen(args.input, "r");
    if (arquivo == NULL)
    {
        printf("Erro ao abrir o arquivo %s\n", args.input);
        exit(EXIT_FAILURE);
    }

    /********************************************
     *
     *          CALCULANDO OS PARÂMETROS
     *
     * args:
     *      args.algoritmo -> NRU, LRU
     *      args.input -> nome do arquivo
     *      args.memsize -> tamanho da memória fisica (varia entre 1MB e 4MB)
     *      args.pagsize -> tamanho da página (é 8KB e 16KB)
     *
     *
     * Os arquivos de entrada possuem 32 bits, então se o tamanho da página for 8KB,
     * 32 - 8 = 24 bits para o offset e 8 bits para o número da página.
     *
     * 8k = 2^13 -> page_idx = adrr >> 13
     * 16k = 2^14 -> page_idx = adrr >> 14
     * 1 M * 2 ^ 20 
     * 
     * 256k paginas de memoria virtual
    /********************************************/

    // tamanho memoria fisica (n de paginas)
    int maximo_paginas = (int)(args.memsize * pow(2, 20)) / (args.pagsize * pow(2, 10)); 

    printf("\nmemsize: %d\n", (int)args.memsize);
    printf("pagsize: %d\n", args.pagsize);

    printf("n\u00B0de p\u00E1ginas f\u00EDsicas: %d\n", maximo_paginas);

    int deslocamento;

    if (args.pagsize == 8)
    {
        deslocamento = 13; // 8KB = 2^13 bytes
    }
    else if (args.pagsize == 16)
    {
        deslocamento = 14; // 16KB = 2^14 bytes
    }
    else
    {
        printf("\033[1;31mTamanho de p\u00E1gina inv\u00E1lido. Deve ser 8 ou 16.\033[0m\n");
        exit(EXIT_FAILURE);
    }

    /********************************************
     *
     *            CRIANDO A PAGTABLE
     *                   e
     *          RESETANDO OS FLAGS   
     * 
    /********************************************/

    PageTable pagetable;

    // array de todas as paginas (virtual)
    pagetable.pages = malloc(sizeof(Page) * pow(2, 32 - deslocamento)); // esse é o array de páginas na memória
    if (pagetable.pages == NULL)
    {
        printf("Erro ao alocar memória para a pagetable.\n");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < maximo_paginas; i++)
    {
        pagetable.pages[i].valida = false;
        pagetable.pages[i].M = false;
        pagetable.pages[i].last_accessed = 0;
    }
    pagetable.num_pages = 0;

    
    /********************************************
     *
     *            LENDO O ARQUIVO
     *                   e
     *          EXECUTANDO O ALGORITMO   
     * 
     ********************************************/

    unsigned int endereco;
    char rw;
    long long time = -1;
    int page_faults = 0;
    int paginas_escritas = 0;
    int resets = 0;

    while (fscanf(arquivo, "%x %c", &endereco, &rw) != EOF)
    {
        time = (time + 1) % 100;

        unsigned int page_idx = endereco >> deslocamento;

        // printf("page_idx: %d\n", page_idx);



    /********************************************
     *
     *                  NRU
     * 
    /********************************************/

        if (strcmp(args.algoritmo, "NRU") == 0)
        {

                    
            if (time == 0)
            {
                // se for, zera o last_accessed de todas as páginas
                reset_reference_bits(&pagetable, pow(2, 32 - deslocamento));
                resets++;
            }

            // checa se a página está na memória
            if (pagetable.pages[page_idx].valida == true)
            {
                // printf("pagina %d esta na memoria\n", i);
                // se estiver, atualiza o last_accessed
                pagetable.pages[page_idx].R = true; // <-------------- aqui atualizamos o last_accessed

                if (rw == 'W')
                {
                    pagetable.pages[page_idx].M = 1;
                }
            }
            else if (pagetable.num_pages < maximo_paginas) // se não estiver, checa se a memória está cheia
            {
                //  nao esta cheia, basta inserir a pagina
                pagetable.pages[page_idx].valida = true;        // <-------------- aqui inserimos a página na memória, de forma representativa
                pagetable.pages[page_idx].R = true; // salvamos o tempo
                pagetable.num_pages++;                          // incrementamos o espaço ocupado na memória
                page_faults++;                                  // <-------------- page_fault, pois a página não estava na memória (mesmo que seja a primeira vez)

                if (rw == 'W')
                {
                    pagetable.pages[page_idx].M = 1;
                }
            }
            else // NRU
            {
                // printf("Memória cheia.\n");
                // a memoria esta cheia, precisamos remover a pagina menos recentemente usada
               
                int trocada_idx = 0;                            // indice da página menos recentemente usada

                trocada_idx = nru(pagetable, pow(2, 32 - deslocamento));


                // checa se a pagina menos recentemente usada foi escrita
                if (pagetable.pages[trocada_idx].M == 1)
                {
                    paginas_escritas++; // <-------------- incrementa escritas, pois M = 1
                    pagetable.pages[trocada_idx].M = 0;
                }


                // agora removemos a menor página
                pagetable.pages[trocada_idx].R = false;
                pagetable.pages[trocada_idx].valida = false;
                pagetable.pages[page_idx].R = true;
                pagetable.pages[page_idx].valida = true;

                if (rw == 'W')
                {
                    pagetable.pages[page_idx].M = 1;
                }

                page_faults++;

            }
        }
        else if (strcmp(args.algoritmo, "LRU") == 0)
        {

    /********************************************
     *
     *               LRU
     * 
    /********************************************/

            if (time == 0)
            {
                // se for, zera o last_accessed de todas as páginas
                for (int i = 0; i < maximo_paginas; i++)
                {
                    pagetable.pages[i].last_accessed = 0;
                }
                resets++;
            }

            // checa se a página está na memória
            if (pagetable.pages[page_idx].valida == true)
            {
                // printf("pagina %d esta na memoria\n", i);
                // se estiver, atualiza o last_accessed
                pagetable.pages[page_idx].last_accessed = time; // <-------------- aqui atualizamos o last_accessed

                if (rw == 'W')
                {
                    pagetable.pages[page_idx].M = 1;
                }
            }
            else if (pagetable.num_pages < maximo_paginas) // se não estiver, checa se a memória está cheia
            {
                //  nao esta cheia, basta inserir a pagina
                pagetable.pages[page_idx].valida = true;        // <-------------- aqui inserimos a página na memória, de forma representativa
                pagetable.pages[page_idx].last_accessed = time; // salvamos o tempo
                pagetable.num_pages++;                          // incrementamos o espaço ocupado na memória
                page_faults++;                                  // <-------------- page_fault, pois a página não estava na memória (mesmo que seja a primeira vez)

                if (rw == 'W')
                {
                    pagetable.pages[page_idx].M = 1;
                }
            }
            else
            {
                // printf("Memória cheia.\n");
                // a memoria esta cheia, precisamos remover a pagina menos recentemente usada
                int menor = pagetable.pages[0].last_accessed; // last_accessed da primeira página, para comparar com o resto
                int menor_idx = 0;                            // indice da página menos recentemente usada

                menor_idx = lru(pagetable, deslocamento);

                // checa se a pagina menos recentemente usada foi escrita
                if (pagetable.pages[menor_idx].M == 1)
                {
                    paginas_escritas++; // <-------------- incrementa escritas, pois M = 1
                    pagetable.pages[menor_idx].M = 0;
                }

                // agora removemos a menor página
                pagetable.pages[menor_idx].last_accessed = 0;
                pagetable.pages[menor_idx].valida = false;
                pagetable.pages[page_idx].last_accessed = time;
                pagetable.pages[page_idx].valida = true;
                page_faults++;

                if (rw == 'W')
                {
                    pagetable.pages[page_idx].M = 1;
                }
            }
        }
        else
        {
            printf("Algoritmo inválido. Deve ser NRU ou LRU.\n");
            exit(EXIT_FAILURE);
        }
    }

    if (arquivo == NULL) {
        printf("arquivo is NULL\n");
    } else {
        fclose(arquivo);
    }

    relatorio_t relatorio;

    relatorio.page_faults = page_faults;
    relatorio.paginas_escritas = paginas_escritas;
    relatorio.resets_do_timer = resets;

    return relatorio;
}