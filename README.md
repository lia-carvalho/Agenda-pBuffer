# Agenda pbuffer
Trabalho da disciplina de Algoritmos e Estrutura de Dados I - semestre 2025/2

Este projeto implementa uma agenda em C com o desafio de gerenciar a memória manualmente dentro de um único buffer `void *pBuffer`.

## Menu:

1- Adicionar Pessoa (Nome, Idade, email)

2- Remover Pessoa

3- Buscar Pessoa

4- Listar todos

5- Sair
 
 ---

O desafio é não poder criar variáveis e ,em vez disso, gerenciar todos os dados do programa simulando o trabalho que o Sistema Operacional faz ao organizar a memória.
 
## Regras:
- Nenhuma variável pode ser declarada em todo o programa, **somente ponteiros**.

- Todos os dados do programa devem ser guardados dentro do pBuffer.

- Só é permitido usar ponteiros que apontam para dentro do pBuffer.

- Não é permitido usar struct em todo o programa.

### Exemplo do que não é permitido: 
```c
int c; 
char a; 
int v[10]; 
void Funcao(int parametro) {
    // ...
}