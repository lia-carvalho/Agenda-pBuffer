#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define NOME 40     //(40* sizeof(char))
#define IDADE sizeof( int )
#define EMAIL 50
#define TAM_PESSOA ( ( NOME ) + ( sizeof(int) ) + ( EMAIL ) ) //40 + 4 + 50 = 94 bytes

void *pbuffer;  //ponteiro global

void adicionarPessoa();
void removerPessoa();
void buscarPessoa();
void listarTodos();

int main(){
    
    pbuffer= malloc ( 2 * sizeof(int) );     //aloca memória para dois inteiros: menu e quantidade de pessoas
    if ( pbuffer == NULL ){
        printf( " Erro ao alocar memória\n ");
        return 1;
    }
    *( int* )pbuffer = 0;           //inicializa a opção do menu(1º int) com 0
    *((int *)pbuffer + 1)= 0;       //inicializa a quantidade de pessoas(2º int) com 0

    int *op= ( int* )pbuffer;         //o ponteiro op aponta para o inicio do buffer(onde está o menu)
    int *qntd= ( int * )pbuffer + 1 ;   //ponteiro para quantidade atual de pessoas

    printf("Agenda inicializada com %d pessoas.\n", *qntd);

    //Menu
    while( *op !=5 ){
        printf ("\n_____________________________\n" );
        printf ( "|          Menu               |\n" );
        printf ( "| 1. Adicionar Pessoa         |\n" );
        printf ( "| 2. Remover Pessoa           |\n" );
        printf ( "| 3. Buscar Pessoa na agenda  |\n" );
        printf ( "| 4. Listar todos na agenda   |\n" );
        printf ( "| 5. Sair                     |\n" );
        printf ( "|_____________________________|\n" );
        printf( "Digite escolha:\n" );
        scanf( "%d",op) ;   //coloca a escolha no endereço guardado por op
        getchar();
        

        switch( *op ){
            //ajustar os ponteiros depois das funções que usam realloc!!!
            case 1:
            adicionarPessoa();
            op = ( int* )pbuffer;        //op recebe o novo endereço do pbuffer
            qntd = ( int* )pbuffer + 1;
            break;

            case 2:
            removerPessoa();
            op = ( int* )pbuffer;
            qntd = ( int * )pbuffer + 1;
            break;

            case 3:
            buscarPessoa();
            op = ( int* )pbuffer;
            qntd = ( int* )pbuffer + 1;
            break;

            case 4:
            listarTodos();
            break;

            case 5:
            printf ( "Encerrando Programa\n" );
            exit( 1 );
            break;

            default:
            printf ( "Opção inválida! Digite novamente\n");
            break;
        }
    }
    free( pbuffer );
    return 0;
}

void adicionarPessoa(){
    int *qntd = ( int * )pbuffer + 1;
    //incrementa o contador de pessoas- aponta para onde a próxima pessoa deve ir 
    (*qntd)++;  
    //realoca memória
    void *novoBuffer= realloc( ( void*)pbuffer,( ( 2 * sizeof( int ) ) + ( *qntd ) * TAM_PESSOA ) );  
    if ( novoBuffer==NULL ){
        printf( "Erro ao alocar memória!\n" );
        ( *qntd )--;
        return;
    }
    pbuffer= novoBuffer; // se deu certo a alocação, atualiza o ponteiro
 
    //calcula o endereço da nova pessoa: endereço base(pbuffer)+deslocamento
    //início da agenda: 2*sizeof(int)
    //nova pessoa: TAM_PESSOA * (qntd -1) 

    void *novaPessoa= ( char* )pbuffer + ( 2 * sizeof ( int ) ) + ( TAM_PESSOA * ( *qntd - 1) );

    char *ptr_Nome= ( char* )novaPessoa;                 //aponta para o início de novaPessoa
    int *ptr_Idade= ( int* )( ( char* )ptr_Nome + NOME );       //aponta para novaPessoa pulando NOME
    char *ptr_Email= ptr_Nome + NOME + sizeof ( int );   //aponta para novaPessoa pulando NOME e idade 

printf( "Nome:\n" );
fgets( ptr_Nome, NOME, stdin );
ptr_Nome[ strcspn(ptr_Nome,"\n" ) ]=0;

printf( "Idade:\n" );
scanf( "%d", ptr_Idade ); 
getchar();

printf( "E-mail:\n" );
fgets(ptr_Email, EMAIL, stdin );
ptr_Email[ strcspn( ptr_Email,"\n" ) ]=0;

printf( "Nova pessoa adicionada com sucesso!\n" );
}

void removerPessoa(){
    int *op = ( int* )pbuffer;         //ponteiro para o índice
    int *qntd = ( int* )pbuffer + 1;   //quantidade de pessoas

    if( *qntd == 0){
        printf( "Nenhuma pessoa na agenda\n" );
        return;
    }
    listarTodos();
    printf( "Digite o índice da pessoa a ser removida:\n" );
    scanf( "%d",op );
    getchar();

    if( *op < 1 || *op > *qntd ){
        printf("Indice inválido\n");
        return;
    }
    (*op)--;   //decrementa o ponteiro do índice
   
    char *pessoaRemovida = ( char* )pbuffer + ( 2 * sizeof( int ) ) + ( TAM_PESSOA * ( *op ) );     //Cálculo do endereço de memória da pessoa a ser removida
    char *proxPessoa = pessoaRemovida + TAM_PESSOA;
    //coloca a próxPessoa no lugar da removida, e o resto da lista vem após próxPessoa:
    memmove(pessoaRemovida, proxPessoa, ( ( *qntd - *op - 1 ) * TAM_PESSOA ) );      
    ( *qntd )--;     //decrementa a quantidade de pessoas

    void *novoBuffer = realloc ( pbuffer,( ( 2* sizeof( int ) ) + ( *qntd ) * TAM_PESSOA ) );
    if ( novoBuffer == NULL ){
        printf( " Erro ao alocar memória\n" );
        ( *qntd ) ++;  //se falhar a alocação, o decremento é desfeito
        return;
    }
    pbuffer = novoBuffer;
    printf( "Pessoa removida com sucesso!" );
    return;
}

void buscarPessoa(){
 int *op = ( int* )pbuffer;            //ponteiro para o índice
 int *qntd = ( int * )pbuffer + 1;

 if ( *qntd == 0 ){
        printf( "Nenhuma pessoa na agenda!\n" );
        return;
    }
//realoca o buffer para armazenar o nomeBuscado e o ptr_Encontrado
void *novoBuffer= realloc( pbuffer, ( 2 * sizeof ( int ) ) + ( ( *qntd ) * TAM_PESSOA ) + NOME + sizeof ( int ) );
if ( novoBuffer == NULL){
    printf( "Erro ao alocar memória\n");
    return;
}
pbuffer = novoBuffer;

//atualiza os ponteiros 
op = ( int* )pbuffer;          
qntd = ( int * )pbuffer + 1;

//Local no buffer pra armazenar o nome a ser buscado(após a última pessoa):
char *nomeBuscado = ( char* )pbuffer + 2 * sizeof( int ) + ( ( *qntd )*TAM_PESSOA );
printf( "Digite o nome da pessoa a ser buscada:\n" );
fgets ( nomeBuscado, NOME, stdin );
nomeBuscado[strcspn(nomeBuscado,"\n")] = 0;

int *ptr_Encontrado = ( int* )( ( char* )nomeBuscado + NOME );     //ponteiro para a contagem- aponta para o endereço após nomeBuscado no buffer
*ptr_Encontrado = 0;    //inicializa ptr_Encontrado com 0

for ( *op = 0; *op < *qntd ; ( *op )++){
    char *pessoaAtual = ( char* )pbuffer + ( 2 * sizeof( int ) ) + ( ( *op ) * TAM_PESSOA );

    if ( strcmp( pessoaAtual, nomeBuscado ) == 0 ){
        printf( " Pessoa encontrada!\n" );
        printf( " %s está no índice %d\n ",pessoaAtual, *op + 1);
        ( *ptr_Encontrado)++;
    }
}
if ( *ptr_Encontrado == 0 ){
    printf( "Pessoa não encontrada.\n" );

}
//reduz o tamanho do buffer antes de sair 
void *bufferReduzido= realloc(pbuffer, ( 2 * sizeof( int ) ) + ( ( *qntd ) * TAM_PESSOA ) );
if ( bufferReduzido == NULL ){
    printf( "Não foi possível reduzir a memória após a busca\n" );
} else{
pbuffer= bufferReduzido;
}

return;
}

void listarTodos(){
    int *op= ( int * )pbuffer;     //ponteiro para o índice
    int *qntd= ( int* )pbuffer + 1;

    if ( *qntd == 0 ){
        printf( "Nenhuma pessoa na agenda!\n" );
        return;
    }

    printf("\n_______________________________________\n");
    printf( "\n      LISTA DE CONTATOS: %d pessoas   \n", *qntd);
    for( *op=0; *op< *qntd; (*op)++){
        //cálculo do endereço de início da pessoa atual:
        char *pessoa= ( char* )pbuffer + ( 2 * sizeof ( int ) ) + ( TAM_PESSOA * (*op) );
        char *ptr_Nome = pessoa;
        int *ptr_Idade = ( int* )( ( char* )ptr_Nome + NOME );
        char *ptr_Email = ptr_Nome + NOME + sizeof ( int );

        printf( "--------Pessoa %d --------\n", *op + 1);
        printf( "Nome: %s\n", ptr_Nome );
        printf( "Idade: %d\n", *ptr_Idade );
        printf( "E-mail:%s\n", ptr_Email );
        printf( "--------------------------\n");
    }
    printf("\n_______________________________________\n");
    return;
}