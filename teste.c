// importação das dependencias/bibliotecas necessárias

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>

// criação de arquivo com o comando dd como descrito no ava
void create_file(const char *file_name, size_t size_mb) {
    char command[1024];
    snprintf(command, sizeof(command), "dd if=/dev/zero of=%s bs=4096 count=%lu >/dev/null 2>&1", file_name, size_mb * 256);
    system(command);
}

// função para leitura sequencial do arquivo
double sequential_read(const char *file_name, size_t block_size) {
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        perror("Erro de abertura de arquivo");
        exit(EXIT_FAILURE);
    }

    char *buffer = malloc(block_size);
    if (!buffer) {
        perror("Erro de alocação de buffer");
        exit(EXIT_FAILURE);
    }

    // obtenção do tempo atual
    struct timeval start, end;
    gettimeofday(&start, NULL);

    while (read(fd, buffer, block_size) > 0) {}
    
    // obtenção do tempo atual 2
    gettimeofday(&end, NULL);

    close(fd);
    free(buffer);

    // retorna o valor em segundos
    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

// função para leitura aleatória
double random_read(const char *file_name, size_t block_size) {
    int fd = open(file_name, O_RDONLY);
    if (fd == -1) {
        perror("Erro de abertura de arquivo");
        exit(EXIT_FAILURE);
    }

    char *buffer = malloc(block_size);
    if (!buffer) {
        perror("Erro de alocação de buffer");
        exit(EXIT_FAILURE);
    }

    off_t file_size = lseek(fd, 0, SEEK_END);
    struct timeval start, end;
    gettimeofday(&start, NULL);

    // variável para máximo de tentativas pois a função podia demorar muito/entrar em loop infinito
    int max_attempts = 30000;
    int attempt = 0;

    while (attempt < max_attempts) {
        off_t offset = rand() % file_size;
        lseek(fd, offset, SEEK_SET);

        if (read(fd, buffer, block_size) == 0) {
            break;
        }

        attempt++;
    }

    // caso o limite seja atingido 
    if (attempt == max_attempts) {
        fprintf(stderr, "Erro: Máximo de tentativas para RANDOM READ excedido\n");
    }

    gettimeofday(&end, NULL);

    close(fd);
    free(buffer);

    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

// função para escrita sequencial
double sequential_write(const char *file_name, size_t block_size) {
    int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Erro de abertura de arquivo");
        exit(EXIT_FAILURE);
    }

    char *buffer = malloc(block_size);
    if (!buffer) {
        perror("Erro de alocação de buffer");
        exit(EXIT_FAILURE);
    }

    memset(buffer, 'A', block_size);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    for (int i = 0; i < 2 * 1024 * 1024 / block_size; i++) {
        write(fd, buffer, block_size);
    }

    gettimeofday(&end, NULL);

    close(fd);
    free(buffer);

    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}


// função para escrita aleatória
double random_write(const char *file_name, size_t block_size) {
    int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);
    if (fd == -1) {
        perror("Erro de abertura de arquivo");
        exit(EXIT_FAILURE);
    }

    char *buffer = malloc(block_size);
    if (!buffer) {
        perror("Erro de alocação de buffer");
        exit(EXIT_FAILURE);
    }

    memset(buffer, 'A', block_size);

    struct timeval start, end;
    gettimeofday(&start, NULL);

    off_t file_size = 2 * 1024 * 1024;
    for (int i = 0; i < 2 * 1024 * 1024 / block_size; i++) {
        off_t offset = rand() % file_size;
        lseek(fd, offset, SEEK_SET);
        write(fd, buffer, block_size);
    }

    gettimeofday(&end, NULL);

    close(fd);
    free(buffer);

    return (end.tv_sec - start.tv_sec) + (end.tv_usec - start.tv_usec) / 1e6;
}

int main() {
    const char *file_name = "test_file"; // definição do nome do arquivo a ser criado

    // tamanho da ram a ser definido, utilizamos 1 para teste, descomentar abaixo para usar RAM atual do sistema
    size_t ram_size = 1; //DESCOMENTAR PARA UTILIZAR TAMANHO DA RAM sysconf(_SC_PAGE_SIZE) * sysconf(_SC_PHYS_PAGES) / (1024 * 1024); // Tamanho da RAM em MB

    // Tamanhos de bloco a serem testados, descomentar para maior variedade
    size_t block_sizes[] = {4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576}; // DESCOMENTAR PARA MAIS TAMANHOS DE BLOCO{1, 2, 4, 8, 16, 32, 64, 128, 256, 512, 1024, 2048, 4096, 8192, 16384, 32768, 65536, 131072, 262144, 524288, 1048576};

    // impressão dos resultados na tela de forma formatada
    printf("| Operação      | Block Size (bytes) | Tempo (segundos) |\n");
    printf("|------------------|--------------------|----------------|\n");

    for (size_t i = 0; i < sizeof(block_sizes) / sizeof(block_sizes[0]); i++) {
        size_t block_size = block_sizes[i];
        create_file(file_name, 2 * ram_size);

        // Leitura Sequencial
        double time_seq_read = sequential_read(file_name, block_size);
        printf("| Sequential Read  | %-18lu | %-14.6f |\n", block_size, time_seq_read);
        fflush(stdout); // garantir que a saída seja exibida no terminal

        // Leitura Aleatória
        double time_rand_read = random_read(file_name, block_size);
        printf("| Random Read      | %-18lu | %-14.6f |\n", block_size, time_rand_read);

        // Escrita Sequencial
        double time_seq_write = sequential_write(file_name, block_size);
        printf("| Sequential Write | %-18lu | %-14.6f |\n", block_size, time_seq_write);

        // Escrita Aleatória
        double time_rand_write = random_write(file_name, block_size);
        printf("| Random Write     | %-18lu | %-14.6f |\n", block_size, time_rand_write);

        printf("|------------------|--------------------|----------------|\n");
    }

    return 0;
}
