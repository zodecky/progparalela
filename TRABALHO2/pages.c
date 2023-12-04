#include <stdbool.h>

#define PAGE_SIZE 256

typedef struct {
    bool valida; // 0 = no disco, 1 = na memória RAM
    bool suja; // não modificada = 0, modificada = 1
    /* Isso serve para saber se a página foi modificada desde que foi carregada na memória RAM.
    Se sim, ela deve ser escrita no disco quando for descarregada da memória RAM.
    */

    int last_accessed; // timestamp da última vez que a página foi acessada

    /* Dados guardados na página */
    char data[PAGE_SIZE]; // dados
} Page;

typedef struct {
    Page *pages; // array de páginas
    /* Guarda todas as páginas */

    int num_pages; // número total de páginas
} PageTable; // tabela de páginas (mapeia endereços virtuais para endereços físicos)

typedef struct {
    Page *frames; // array fisico de páginas
    int num_frames; // número total de frames físicos (páginas)
} PhysicalMemory; // memória física

/*********************************************************
 * read_data(int virtual_address): 
 * Esta função é usada para ler dados de um endereço virtual. 
 * Ela deve traduzir o endereço virtual para um endereço físico usando a tabela de páginas,
 *  verificar se a página correspondente está na memória e, 
 * em caso afirmativo, retornar os dados nesse endereço.
*/
void read_data(int virtual_address);


/*******************************************************
 * write_data(int virtual_address, char data):
 *  Esta função é usada para escrever dados em um endereço virtual.
 *  Assim como a função read_data,
 *  ela deve traduzir o endereço virtual para um endereço físico usando a tabela de páginas.
 *  Em seguida, ela deve escrever os dados no endereço físico
 *  correspondente e marcar a página como "suja".
*/
void write_data(int virtual_address, char data);



/*********************************************************
Esta função é usada para carregar uma página na memória.
 Ela deve encontrar um frame livre na memória física,
  ler a página do disco para esse frame e atualizar a tabela de páginas para
   indicar que a página agora está na memória. */
void load_page(int page_number);

/*
Esta função é usada para descarregar uma página da memória.
Se a página estiver marcada como "suja", ela deve primeiro escrever a página de volta no disco.
Em seguida, ela deve atualizar a tabela de páginas
para indicar que a página não está mais na memória.
*/
void unload_page(int page_number);

/*
Esta função é usada para detectar falhas de página.
Se um programa tenta acessar um endereço virtual cuja página
correspondente não está na memória, isso é chamado de falha de página.
Esta função deve verificar a tabela de páginas para ver se a página
correspondente ao endereço virtual está na memória e, se não estiver,
deve retornar um valor indicando que ocorreu uma falha de página.
 */
int detect_page_fault(int virtual_address);