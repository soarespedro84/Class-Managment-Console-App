#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>
#include<time.h>
#define AulasPS "AulasPS.pfs"
//Cria o ficheiro com o nome e extensão definidos entre ""

struct aula { //Definição da aula

    int idAula;
    char data[20];
    char turma[10];
    int modulo;
    char sumario[100];

};
int menu(char *fmenu, int opMin, int opMax) {//Funcao para validar se as opcoes inseridas foram entre 1 e 5
    int op =0;
    system("cls"); //limpa o ecra
    printf("%s", fmenu);
    do{
       printf("\nOpcao:");
       scanf("%d", &op);
    } while (op < opMin || op > opMax); //Repete o ciclo enquanto não for introduzida uma opção válida
    return op;
}

//gerar o numero da aula
int nextid () {
  //gera um numero aleatorio que
  //não se repete mesmo quando reinicia o programa
  srand(time(0));
    int num= rand();
    return num;

}

void InserirAula (){ //Funcao para inserir registos na lista de sumarios
    struct aula a;
        FILE *fAula;
        fAula = fopen(AulasPS, "rb+");        ////Abre o ficheiro
        if (fAula == NULL) {
            fAula = fopen(AulasPS, "wb");   //Cria o ficheiro se este não existir
            fclose(fAula);
            fAula = fopen(AulasPS, "rb+");   //Pronto para acesso Read-Write
        }
            setbuf(stdin, NULL); //limpa a memoria para uma nova inserção de dados
            system("cls"); //limpa o ecra

            //Podemos comecar a inserir informação no ficheiro
            printf("\n********Nova Aula********\n");
            do{
            a.idAula = nextid();
            printf("\nDigite a data (DD/MM/AAAA): ");
            scanf("%s", &a.data);
            printf("\nDigite a turma: ");
            scanf("%s", &a.turma);
            printf("\nDigite o modulo: ");
            scanf("%d", &a.modulo);
            printf("\nDigite o sumario: ");
            fgets(a.sumario, 100, stdin);
            scanf("%[^\n]%*c", &a.sumario);
            setbuf(stdin, NULL); //limpa a memoria para uma nova inserção de dados
            } while (strlen(a.sumario) < 1 || strlen(a.sumario) > 100);

            //Aqui começa a gravar no ficheiro ////Buffer OK
            fseek(fAula, 0, SEEK_END); //Aponta para o inicio do ficheiro
            fwrite(&a, 1, sizeof(struct aula), fAula);
            fclose(fAula); //Fecha o ficheiro
}

void ListarAula(){ //Funcao para listar registos de aulas
    struct aula a;
    FILE *fAula;
    int cnt=0;
    fAula = fopen(AulasPS, "rb+"); //Abre ficheiro
    if (!fAula) printf("\n**********************************\n**Nao existem aulas registadas\n"); //se ficheiro não existir ficheiro devolve mensagem
    else {
            fseek(fAula, 0, SEEK_SET); //Aponta para o inicio do ficheiro
            do{
                fread(&a, 1, sizeof(struct aula), fAula);//le o ficheiro
                if (!feof(fAula)){//enquanto nao for o fim do ficheiro imprime os registos de aulas que encontrar
                    cnt++;
                    printf("\n**********************************\nID Aula: %d\nData: %s\nTurma: %s\nModulo: %d\nSumario: %s", a.idAula ,a.data, a.turma, a.modulo, a.sumario);
                }
            } while(!feof(fAula));; //enquanto nao for o fim do ficheiro
        }
        printf("\n\n**********************************\n**Foram encontrados %d aulas**\n", cnt);
        fclose(fAula); //Fecha o ficheiro
}

void PesquisarAula(){ //Funcao para pesquisar as aulas de uma turma
   struct aula a;
    char opcao[10];
    int cnt=0;
    FILE *fAula;
    fAula = fopen(AulasPS, "rb+"); //Abre ficheiro
    if (!fAula) printf("\n**********************************\n**Nao existem aulas guardadas\n");
    else {
            int procurador = 0;
            fseek(fAula, 0, SEEK_SET); //Aponta para o inicio do ficheiro
            setbuf(stdin, NULL); //limpa a memoria para uma nova inserção de dados
            printf("\nPesquisar por: ");
            scanf("%9[^\n]s", opcao); ////Deixa inserir palavras separadas por espaços
            do{
                fread(&a, 1, sizeof(struct aula), fAula);//le o ficheiro
                if (!feof(fAula)){ //enquanto nao for o fim do ficheiro
                    if (strstr(a.turma, opcao) || strstr(a.data, opcao) != 0) { //compara se existe no ficheiro algum valor igual ao digitado
                        procurador = 1;
                        cnt++; // Se encontrar registos coloca a flag a 1 e imprime os registos encontrados
                    printf("\n**********************************\nID Aula: %d\nData: %s\nTurma: %s\nModulo: %d\nSumario: %s", a.idAula ,a.data, a.turma, a.modulo, a.sumario);
                    }
                }
            } while(!feof(fAula));//faz enquanto nao encontra o fim do ficheiro

            if (procurador == 0){ // Se não encontrar registos a flag continua a 0 imprime a mensagem abaixo
                    printf("\n**********************************\n**Nao foram encontradas aulas**\n", cnt);
                }else{ //Se encontra registos pergunta se quer fazer edição
                    printf("\n**********************************\n**Foram encontradas %d aulas(s)**\n", cnt);
                    char ch;
                    printf("\n**********************************\nPretende editar alguma aula? (y/n)\n");
                    scanf("%s",&ch);
                    if(ch=='y'){
                        EditarAula();}
        }
        fclose(fAula); //Fecha o ficheiro
    }
}

void EditarAula(){ //Funcao para editar uma aula
    struct aula a;
    int opcao;
    FILE *fAula;
    fAula = fopen(AulasPS, "rb+"); //Abre ficheiro
    if (!fAula) printf("\n**********************************\n**Nao existem aulas guardadas\n");
    else {
        fseek(fAula, 0, SEEK_SET); //Aponta para o inicio do ficheiro
        setbuf(stdin, NULL); //limpa a memoria para uma nova inserção de dados
        printf("\nInsira o ID da turma a pesquisar: ");
        scanf("%d", &opcao);
        while (!feof(fAula)){
            fread(&a, sizeof(struct aula), 1, fAula);//le o ficheiro
            if (a.idAula == opcao){ //compara se existe i ID digitado é igual ao ID da Aula
                printf("\n**********************************\nID Aula: %d\nData: %s\nTurma: %s\nModulo: %d\nSumario: %s\n", a.idAula ,a.data, a.turma, a.modulo, a.sumario);//mostra
                char ch;
                //confirma se é para editar e edita campo a campo
                printf("\nPretende editar esta aula? (y/n)\n");
                scanf("%s",&ch);
                if(ch=='y'){
                    printf("\nAlterar a data? (y/n)\n");
                    scanf("%s",&ch);
                    if(ch=='y'){
                        printf("\nDigite a data (DD/MM/AAAA): ");
                        fflush(stdin);
                        scanf("%s", &a.data);}
                    printf("\nAlterar a turma? (y/n)\n");
                    scanf("%s",&ch);
                    if(ch=='y'){
                        printf("\nDigite a turma: ");
                        fflush(stdin);
                        scanf("%s", &a.turma);}
                    printf("\nAlterar o modulo? (y/n)\n");
                    scanf("%s",&ch);
                    if(ch=='y'){
                        printf("\nDigite o modulo: ");
                        fflush(stdin);
                        scanf("%d", &a.modulo);}
                    printf("\nAlterar o sumario? (y/n)\n");
                    scanf("%s",&ch);
                    if(ch=='y'){
                        printf("\nDigite o sumario: ");
                        fflush(stdin);
                        fgets(a.sumario, 100, stdin);
                        scanf("%[^\n]%*c", &a.sumario);}
                    setbuf(stdin, NULL);
                    fseek(fAula, -sizeof(struct aula), SEEK_CUR);
                    fwrite(&a, 1, sizeof(struct aula), fAula);
                    printf("\n**Registo inserido com sucesso!**");
                    fclose(fAula);
                    return;
                }
            }
        }
    } printf("\n**********************************\n**Nao foram encontradas aulas**\n");

}

void Ecra(){ //Funcao que mostra as opcoes do programa
int opcao = menu("\n**************************\n**   Gestao de Aulas    **\n**\t**MENU**\t**\n**************************\n**    1-Nova Aula\t**\n**    2-Mostrar Aulas\t**\n**    3-Pesquisar Aula\t**\n**    4-Editar Aula\t**\n**    5-Sair\t\t**\n**************************", 1,5);
        switch(opcao){ //comando de tomada de decisao
        case 1:
            InserirAula(); //Chama a função inserir
            return main(); //Retorna ao main
        case 2:
            ListarAula(); //Chama a função listar
            printf("\n**********************************\n*Enter para voltar para o menu*");
            getch(); //lê um caracter do teclado, não o apresenta no ecra e retorna para a função principal
            return main(); //Retorna ao main
        case 3:
            PesquisarAula(); //Chama a função pesquisar
            printf("\n**********************************\n*Enter para voltar para o menu*");
            getch(); //lê um caracter do teclado, não o apresenta no ecra e retorna para a função principal
            return main(); //Retorna ao main
        case 4:
            EditarAula(); //Chama a função editar
            printf("\n**********************************\n*Enter para voltar para o menu*");
            getch(); //lê um caracter do teclado, não o apresenta no ecra e retorna para a função principal
            return main(); //Retorna ao main
        case 5:
            printf("Pedro Soares, 2020\n");
            exit(0); //sai do programa
            }
    return 0;
}

int main(){
    Ecra(); //Chama a funcao ecra que mostra todas as opcoes disponiveis no programa
    }
