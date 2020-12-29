/*
Autor: Ígor Ferrazza Capeletti
Matrícula: 1802440007

Solução do Problema:
    Primeiramente foi criado uma estrutura para cada pixel armazenando 3 valores inteiros e depois
    criei outra estrutura "arv" que será o conteúdo de cada nó,
    em cada nó contem uma matriz da estrutura de pixels, valores do tamanho de cada matriz na horizontal e vertical,
    como também contém os 4 nós filhos que tem a mesma estrutura deste nó Pai.
    A solução foi criar um árvore raiz para ser o ponto de partida, verificar se o arquivo é tipo PPM e então
    ler os valores no arquivo texto e armazenar no nó raiz os valores de cada linha virando pixels na matriz do nó.
    Foi inserido recursivamente 4 nós filhos para cada nó e dividida a imagem de cada nó em 4 quadrantes filhos,
    cada quadrante filho armazena 1/4 dos pixels da matriz do nó Pai.
    Depois de inserido todos os pixels nos nós filhos é realizada a média da imagem de acordo com a profundidade
    escolhida e a imagem é gerada no nó Pai e salva no arquivo de texto.
*/

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

//representa a estrutura do pixel(r,g,b) que são os pontos na tela, ex: 255 255 255;
struct px{
    int r; //cor vermelha
    int g; //cor verde
    int b; //cor azul
};
typedef struct px Pixel; //definido Pixel como nome do tipo de estrutura "px"

//estrutura de cada nó da árvore
struct arv {
    Pixel **media;  //armazena os pixels de cada nó
    int dimx;       //dimx=largura
    int dimy;       //dimy=altura
    struct arv *q1; //nó filho representando quadrante 1
    struct arv *q2; //nó filho representando quadrante 2
    struct arv *q3; //nó filho representando quadrante 3
    struct arv *q4; //nó filho representando quadrante 4
};
typedef struct arv QuadTree; //definido QuadTree como nome do tipo de estrutura "arv"

//verificar se imagem é do tipo PPM
bool verificaArquivo(char nomeArquivoLer[]){
    int i=0, maxcolor=0, dimx=0, dimy=0;
    char tipoimagem[2], tipoimagem2[]={'P','3'};
    FILE *imagemLer = fopen(nomeArquivoLer, "r");
    if(imagemLer == NULL){
        printf("Arquivo nao existe!");
        exit(0);
    }else{
        fscanf(imagemLer, "%s", tipoimagem); //ler a primeira linha do arquivo e ver se é P3(significa que é PPM)
        while (tipoimagem[i]!="\n" && tipoimagem[i] == tipoimagem2[i]){ //compara caracter por caracter dos vetores e soma i++ se forem iguais em cada posição
            ++i;
        }
        if(i==2){ //significa que é imagem PPM, as 5 linhas abaixo é só para exibir na tela informações da imagem
            fscanf(imagemLer,"%i %i", &dimx, &dimy);
            fscanf(imagemLer,"%i", &maxcolor);
            printf("%s\n",tipoimagem);
            printf("%d %d\n",dimx, dimy);
            printf("%i\n",maxcolor);
            fclose(imagemLer); //desaloca memória para arquivo
            return true;  //esse retorno que importa para dizer se a imagem é PPM ou não
        }else{
            fclose(imagemLer);
            return false;
        }
    }
}

//cria árvore nula
QuadTree *criarArvoreVazia(QuadTree *no){
    return NULL;
}

//Cria a árvore QuadTree e uma unica imagem na raiz com todos os pixels do arquivo
QuadTree *criaArvore(char nomeArquivoLer[]){
    //recebe o arquivo que vai ser a imagem ppm e lê o cabeçalho do arquivo
    char tipoimagem[2];
    int maxcolor, dimx, dimy;
    FILE *imagemLer = fopen(nomeArquivoLer, "r");
    fscanf(imagemLer,"%s", &tipoimagem);
    fscanf(imagemLer,"%i %i", &dimx, &dimy);
    fscanf(imagemLer,"%i", &maxcolor);

    //aloca memoria dinamicamente na memória de tamanho da estrutura QuadTree
    QuadTree *no = (QuadTree*)malloc(sizeof(QuadTree));
    if(no!=NULL){
        no->dimx= dimx;
        no->dimy= dimy;
        //aloca memória dinamicamente para a matriz de pixels da imagem do nó e preenche a matriz lendo linha por linha do arquivo
        no->media= (Pixel**)malloc(dimy * sizeof(Pixel*)); //aloca memória dinamicamente para um Ponteiro que aponta para Ponteiros
        for (int l=0; l < dimy; l++){
            no->media[l] = (Pixel*)malloc(dimx * sizeof(Pixel)); //aloca memória dinamicamente para Ponteiros de Estrutura Pixels
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

//função para inserir um nó na árvore e os pixels para o determinado quadrante
QuadTree *insereNo(QuadTree *no,Pixel **img,int x_inicio,int y_inicio,int x_fim,int y_fim){
    if(no==NULL){
        //vai alocar dinamicamente o nó na memória
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
        if(no->dimx > 1 && no->dimy > 1){ //se nó tiver pixels vai prosseguir
            no->q1= insereNo(no->q1,no->media,0,0,no->dimx/2, no->dimy/2);
            no->q2= insereNo(no->q2,no->media,no->dimx/2,0,no->dimx, no->dimy/2);
            no->q3= insereNo(no->q3,no->media,0,no->dimy/2,no->dimx/2, no->dimy);
            no->q4= insereNo(no->q4,no->media,no->dimx/2,no->dimy/2,no->dimx, no->dimy);
        }else{ //vai entrar aqui se não tiver mais pixels na imagem para dividir em quadrantes
            no->q1=NULL;
            no->q2=NULL;
            no->q3=NULL;
            no->q4=NULL;
        }
    }
}

//função que faz a média dos pixels nas mesmas posições dos 4 quadrantes filhos e
//armazena no nó pai dos mesmos a média dos pixels
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

//inserir todos os nós na árvore subdividindo cada nó em 4 filhos e armazenando a
// subdivisão dos pixels nos quadrantes
// até não ter mais pixels no nó para distribuir nos quadrantes
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

//função para imprimir os pixels de um determinado nó
void *imprime(QuadTree *no){
    if(no!=NULL){
        //vai percorrer a matriz de pixels do determinado nó e mostrar na tela
        for(int l=0; l < no->dimy; l++){
            for(int c=0; c < no->dimx; c++){
                printf("%i %i %i ", no->media[l][c].r,no->media[l][c].g,no->media[l][c].b);
            }printf("\n");
        }
    }
}

//função para liberar memória alocada da árvore
QuadTree *liberaQuadTree(QuadTree *qual){
    if(qual != NULL){
        liberaQuadTree(qual->q1);
        liberaQuadTree(qual->q2);
        liberaQuadTree(qual->q3);
        liberaQuadTree(qual->q4);
        free(qual); //libera o no "qual" da memória
    }
    return NULL;
}

//função para salvar imagem num arquivo de texto
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

//função principal, onde vai haver as chamadas de operações e de outras funções
int main(void)
{
    bool verificar;
    int profundidade=1;
    char nomeArquivoLer[]="imagem.ppm";

    verificar=  verificaArquivo(nomeArquivoLer); //chama função que verifica que o arquivo é uma imagem PPM

    if(verificar){
        printf("\nArquivo e PPM!\n\n");
    }else{
        printf("\nArquivo nao e PPM!");
    }
    printf("Digite a Profundidade de Compressao da Imagem!");
    scanf("%i",&profundidade);

    QuadTree *teste =criaArvore(nomeArquivoLer); //cria árvore raiz com os pixels do arquivo

    insereRecursivo(teste, teste->media); //insere recursivamente os nós filhos e pixels dos dos filhos na árvore até não ter mais pixels para serem divididos em quadrantes

    //printf("\n%i %i %i\n",teste->media[7][0].r,teste->media[7][0].g,teste->media[7][0].b);

    //comprime a imagem de acordo com a profundidade escolhida
    while(profundidade > 0){
        fazMedia(teste);
        profundidade--;
    }

    salvarImagem(teste); //chama função para salvar imagem passando como parâmetro a árvore "teste"
    liberaQuadTree(teste); //desalocar toda árvore da memória do computador
    return 0;
}
