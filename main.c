/*
Autor: �gor Ferrazza Capeletti
Matr�cula: 1802440007

Solu��o do Problema:
    Primeiramente foi criado uma estrutura para cada pixel armazenando 3 valores inteiros e depois
    criei outra estrutura "arv" que ser� o conte�do de cada n�,
    em cada n� contem uma matriz da estrutura de pixels, valores do tamanho de cada matriz na horizontal e vertical,
    como tamb�m cont�m os 4 n�s filhos que tem a mesma estrutura deste n� Pai.
    A solu��o foi criar um �rvore raiz para ser o ponto de partida, verificar se o arquivo � tipo PPM e ent�o
    ler os valores no arquivo texto e armazenar no n� raiz os valores de cada linha virando pixels na matriz do n�.
    Foi inserido recursivamente 4 n�s filhos para cada n� e dividida a imagem de cada n� em 4 quadrantes filhos,
    cada quadrante filho armazena 1/4 dos pixels da matriz do n� Pai.
    Depois de inserido todos os pixels nos n�s filhos � realizada a m�dia da imagem de acordo com a profundidade
    escolhida e a imagem � gerada no n� Pai e salva no arquivo de texto.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//representa a estrutura do pixel(r,g,b) que s�o os pontos na tela, ex: 255 255 255;
struct px{
    int r; //cor vermelha
    int g; //cor verde
    int b; //cor azul
};
typedef struct px Pixel; //definido Pixel como nome do tipo de estrutura "px"

//estrutura de cada n� da �rvore
struct arv {
    Pixel **media;  //armazena os pixels de cada n�
    int dimx;       //dimx=largura
    int dimy;       //dimy=altura
    struct arv *q1; //n� filho representando quadrante 1
    struct arv *q2; //n� filho representando quadrante 2
    struct arv *q3; //n� filho representando quadrante 3
    struct arv *q4; //n� filho representando quadrante 4
};
typedef struct arv QuadTree; //definido QuadTree como nome do tipo de estrutura "arv"

//verificar se imagem � do tipo PPM
bool verificaArquivo(char nomeArquivoLer[]){
    int i=0, maxcolor=0, dimx=0, dimy=0;
    char tipoimagem[2], tipoimagem2[]={'P','3'};
    FILE *imagemLer = fopen(nomeArquivoLer, "r");
    if(imagemLer == NULL){
        printf("Arquivo nao existe!");
        exit(0);
    }else{
        fscanf(imagemLer, "%s", tipoimagem); //ler a primeira linha do arquivo e ver se � P3(significa que � PPM)
        while (tipoimagem[i]!="\n" && tipoimagem[i] == tipoimagem2[i]){ //compara caracter por caracter dos vetores e soma i++ se forem iguais em cada posi��o
            ++i;
        }
        if(i==2){ //significa que � imagem PPM, as 5 linhas abaixo � s� para exibir na tela informa��es da imagem
            fscanf(imagemLer,"%i %i", &dimx, &dimy);
            fscanf(imagemLer,"%i", &maxcolor);
            printf("%s\n",tipoimagem);
            printf("%d %d\n",dimx, dimy);
            printf("%i\n",maxcolor);
            fclose(imagemLer); //desaloca mem�ria para arquivo
            return true;  //esse retorno que importa para dizer se a imagem � PPM ou n�o
        }else{
            fclose(imagemLer);
            return false;
        }
    }
}

//cria �rvore nula
QuadTree *criarArvoreVazia(QuadTree *no){
    return NULL;
}

//Cria a �rvore QuadTree e uma unica imagem na raiz com todos os pixels do arquivo
QuadTree *criaArvore(char nomeArquivoLer[]){
    //recebe o arquivo que vai ser a imagem ppm e l� o cabe�alho do arquivo
    char tipoimagem[2];
    int maxcolor, dimx, dimy;
    FILE *imagemLer = fopen(nomeArquivoLer, "r");
    fscanf(imagemLer,"%s", &tipoimagem);
    fscanf(imagemLer,"%i %i", &dimx, &dimy);
    fscanf(imagemLer,"%i", &maxcolor);

    //aloca memoria dinamicamente na mem�ria de tamanho da estrutura QuadTree
    QuadTree *no = (QuadTree*)malloc(sizeof(QuadTree));
    if(no!=NULL){
        no->dimx= dimx;
        no->dimy= dimy;
        //aloca mem�ria dinamicamente para a matriz de pixels da imagem do n� e preenche a matriz lendo linha por linha do arquivo
        no->media= (Pixel**)malloc(dimy * sizeof(Pixel*)); //aloca mem�ria dinamicamente para um Ponteiro que aponta para Ponteiros
        for (int l=0; l < dimy; l++){
            no->media[l] = (Pixel*)malloc(dimx * sizeof(Pixel)); //aloca mem�ria dinamicamente para Ponteiros de Estrutura Pixels
            for (int c=0; c<dimx; c++){
                fscanf(imagemLer,"%i %i %i",&no->media[l][c].r,&no->media[l][c].g,&no->media[l][c].b);
                //printf("%d %d %d ",no->media[l][c].r,no->media[l][c].g,no->media[l][c].b);
            }//printf("\n");
        }
        no->q1= NULL;
        no->q2= NULL;
        no->q3= NULL;
        no->q4= NULL;
    }else{
        printf("Problema ao alocar memoria!");
        exit(-1);
    }
    fclose(imagemLer);
    printf("Arvore criada com sucesso!\n");
    return no;
}

//fun��o para inserir um n� na �rvore e os pixels para o determinado quadrante
QuadTree *insereNo(QuadTree *no,Pixel **img,int x_inicio,int y_inicio,int x_fim,int y_fim){
    if(no==NULL){
        //vai alocar dinamicamente o n� na mem�ria
        no=(QuadTree*)malloc(sizeof(QuadTree));
        no->dimx= x_fim - x_inicio;
        no->dimy= y_fim - y_inicio;
        no->media= (Pixel**)malloc((no->dimy)*sizeof(Pixel*)); //aloca ponteiros para ponteiros de estruturas Pixel
        for(int l=y_inicio,y=0;l < y_fim; y++,l++){
            no->media[y]= (Pixel*)malloc((no->dimx)*sizeof(Pixel)); //aloca ponteiros de estruturas Pixel
            for(int c=x_inicio,x=0;c < x_fim; x++, c++){
                no->media[y][x].r= img[l][c].r;
                no->media[y][x].g= img[l][c].g;
                no->media[y][x].b= img[l][c].b;
                //printf("%i %i %i ",no->media[y][x].r,no->media[y][x].g,no->media[y][x].b);
            }//printf("\n");
        }
        no->q1=NULL;
        no->q2=NULL;
        no->q3=NULL;
        no->q4=NULL;
        return no;
    }else{
        if(no->dimx > 1 && no->dimy > 1){ //se n� tiver pixels vai prosseguir
            no->q1= insereNo(no->q1,no->media,0,0,no->dimx/2, no->dimy/2);
            no->q2= insereNo(no->q2,no->media,no->dimx/2,0,no->dimx, no->dimy/2);
            no->q3= insereNo(no->q3,no->media,0,no->dimy/2,no->dimx/2, no->dimy);
            no->q4= insereNo(no->q4,no->media,no->dimx/2,no->dimy/2,no->dimx, no->dimy);
        }else{ //vai entrar aqui se n�o tiver mais pixels na imagem para dividir em quadrantes
            no->q1=NULL;
            no->q2=NULL;
            no->q3=NULL;
            no->q4=NULL;
        }
    }
}

//fun��o que faz a m�dia dos pixels nas mesmas posi��es dos 4 quadrantes filhos e
//armazena no n� pai dos mesmos a m�dia dos pixels
QuadTree *fazMedia(QuadTree *no){
    free(no->media);
    no->media= NULL;
    no->dimx= (no->dimx/2);
    no->dimy= (no->dimy/2);
    no->media= (Pixel**)malloc((no->dimy)*sizeof(Pixel*));
    for(int l=0; l < no->dimy; l++){
        no->media[l]= (Pixel*)malloc((no->dimx)*sizeof(Pixel));
        for(int c=0; c < no->dimx; c++){
            no->media[l][c].r= (no->q1->media[l][c].r + no->q2->media[l][c].r + no->q3->media[l][c].r + no->q4->media[l][c].r)/4;
            no->media[l][c].g= (no->q1->media[l][c].g + no->q2->media[l][c].g + no->q3->media[l][c].g + no->q4->media[l][c].g)/4;
            no->media[l][c].b= (no->q1->media[l][c].b + no->q2->media[l][c].b + no->q3->media[l][c].b + no->q4->media[l][c].b)/4;
        }
    }
    return no;
}

//inserir todos os n�s na �rvore subdividindo cada n� em 4 filhos e armazenando a
// subdivis�o dos pixels nos quadrantes
// at� n�o ter mais pixels no n� para distribuir nos quadrantes
QuadTree *insereRecursivo(QuadTree *noPai, Pixel **img){
    int x= noPai->dimx, y=noPai->dimy;
    while(noPai->dimx > 1 && noPai->dimy > 1){
        noPai->q1= insereNo(noPai->q1,img,0,0,noPai->dimx/2, noPai->dimy/2);
        noPai->q2= insereNo(noPai->q2,img,noPai->dimx/2,0,noPai->dimx, noPai->dimy/2);
        noPai->q3= insereNo(noPai->q3,img,0,noPai->dimy/2,noPai->dimx/2, noPai->dimy);
        noPai->q4= insereNo(noPai->q4,img,noPai->dimx/2,noPai->dimy/2,noPai->dimx, noPai->dimy);
        noPai->dimx/=2;
        noPai->dimy/=2;
    }
    noPai->dimx= x;
    noPai->dimy= y;
    return noPai;
}

//fun��o para imprimir os pixels de um determinado n�
void *imprime(QuadTree *no){
    if(no!=NULL){
        //vai percorrer a matriz de pixels do determinado n� e mostrar na tela
        for(int l=0; l < no->dimy; l++){
            for(int c=0; c < no->dimx; c++){
                printf("%i %i %i ", no->media[l][c].r,no->media[l][c].g,no->media[l][c].b);
            }printf("\n");
        }
    }
}

//fun��o para liberar mem�ria alocada da �rvore
QuadTree *liberaQuadTree(QuadTree *qual){
    if(qual != NULL){
        liberaQuadTree(qual->q1);
        liberaQuadTree(qual->q2);
        liberaQuadTree(qual->q3);
        liberaQuadTree(qual->q4);
        free(qual); //libera o no "qual" da mem�ria
    }
    return NULL;
}

//fun��o para salvar imagem num arquivo de texto
void *salvarImagem(QuadTree *no){
    FILE *imagemSalvar = fopen("imagemComprimida.ppm", "w");
    fprintf(imagemSalvar,"P3\n%i %i\n255",no->dimx, no->dimy);
    if(no!=NULL){
        for(int l=0; l < no->dimy; l++){
            for(int c=0; c < no->dimx; c++){
                fprintf(imagemSalvar,"\n%i %i %i",no->media[l][c].r,no->media[l][c].g,no->media[l][c].b);
                //printf("%d %d %d ",no->media[l][c].r,no->media[l][c].g,no->media[l][c].b);
            }//printf("\n");
        }
    }else{
        printf("Nao existe arvores!");
        exit(-1);
    }
    fclose(imagemSalvar);
    printf("Imagem salva com sucesso!\n");
}

//fun��o principal, onde vai haver as chamadas de opera��es e de outras fun��es
int main(void)
{
    bool verificar;
    int profundidade=1;
    char nomeArquivoLer[]="imagem.ppm";

    verificar=  verificaArquivo(nomeArquivoLer); //chama fun��o que verifica que o arquivo � uma imagem PPM

    if(verificar){
        printf("\nArquivo e PPM!\n\n");
    }else{
        printf("\nArquivo nao e PPM!");
    }
    printf("Digite a Profundidade de Compressao da Imagem!");
    scanf("%i",&profundidade);

    QuadTree *teste =criaArvore(nomeArquivoLer); //cria �rvore raiz com os pixels do arquivo

    insereRecursivo(teste, teste->media); //insere recursivamente os n�s filhos e pixels dos dos filhos na �rvore at� n�o ter mais pixels para serem divididos em quadrantes

    //printf("\n%i %i %i\n",teste->media[7][0].r,teste->media[7][0].g,teste->media[7][0].b);

    //comprime a imagem de acordo com a profundidade escolhida
    while(profundidade > 0){
        fazMedia(teste);
        profundidade--;
    }

    salvarImagem(teste); //chama fun��o para salvar imagem passando como par�metro a �rvore "teste"
    liberaQuadTree(teste); //desalocar toda �rvore da mem�ria do computador
    return 0;
}
