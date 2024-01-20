# Desempenho de operações de entrada e saida

### Grupo D:
- Jonas Gabriel dos Santos Costa Fagundes
- Anna Carolina Brito Santos Faria
- Christian Coronel da S. Polli - 798083

## O objetivo deste trabalho é realizar testes de medida de tempo (benchmark) sobre operações de entrada e saída, sendo estas:
- Leitura Sequencial
- Leitura Randômica
- Escrita Sequencial
- Escrita Randômica

Para realização deste exercício é criado um arquivo utilizado o comando (com algumas breves alterações) indicado no enunciado da tarefa: $ dd if=/dev/zero of=...nome_do_arquivo... bs=4096 count=xxx 

São definidos então diversas opções de blocos para leitura e escrita que devem ser testados em arquivos que devem possuir pelo menos o dobro do tamanho da memória RAM disponível (utilizamos valores menores para teste).

# Exemplo de saída:
![image](https://github.com/Maracujacake/benchmark_SO/assets/74564883/e103b225-4482-4c8c-a2b7-d7acf5128629)

Para executar o programa, é recomendado compilar o código C disponível:

```
gcc teste.c -o benchmark3
```
Logo em seguida, execute o programa compilado

```
./benchmark3
```
