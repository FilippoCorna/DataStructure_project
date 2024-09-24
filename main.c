#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

enum COLOR { RED, BLACK };

/*
struct pos{
    char lettera;
    int* posizione;
};

struct zero{
    char lettera;
    int zeroOcc;
};

struct num{
    char lettera;
    int numero;
    int ugualeOMin;
    int attivo;
};*/

struct pos{
    int lettera;
    int* posizione;
};

struct zero{
    int lettera;
    int zeroOcc;
};

struct num{
    int lettera;
    int numero;
    int ugualeOMin;
    int attivo;
};

struct RBNode {
  char* data;
  enum COLOR color;
  int filtrato;
  struct RBNode *parent;
  struct RBNode *left;
  struct RBNode *right;
};

static struct RBNode * createNode(char* const parola, int k){
    struct RBNode * newNode = (struct RBNode *) malloc(sizeof(struct RBNode));
    newNode -> data = (char*)malloc(k+1);
    strcpy(newNode->data , parola);
    newNode -> data[k] = '\0';
    newNode->color = RED;
    newNode->filtrato = 1;
    newNode->parent = NULL;
    newNode->left = NULL;
    newNode->right = NULL;
    return newNode;
}

struct RBNode* createNodeDaNew(char* parola, int k, struct RBNode * newN, char* newC, int i){
    struct RBNode * node;
    newN[i].data= &newC[i*(k+1)];
    strncpy(newN[i].data, parola, k);
    newN[i].data[k]='\0';
    newN[i].filtrato= 1;
    
    /*
    strcpy(newC[i] , parola);
    newN[i]->data = newC[i];
    newN[i] -> data[k] = '\0';*/
    newN[i].color = RED;
    newN[i].parent = NULL;
    newN[i].left = NULL;
    newN[i].right = NULL;
    
    node = &newN[i];
    
    return node;
}

bool notNull(struct RBNode * const rbNode){
    return rbNode != NULL;
}

struct RBNode *leftChildOf(struct RBNode *root) {
    return notNull(root) ? root->left  : NULL;
}
struct RBNode *rightChildOf(struct RBNode *root) {
    return notNull(root) ? root->right : NULL;
}

struct RBNode *parentOf(struct RBNode * const root) {
    return notNull(root) && notNull(root->parent) ? root->parent : NULL;
}
struct RBNode *grandParentOf(struct RBNode * const root) {
    return parentOf(parentOf(root));
}

bool isLeftChildOf(struct RBNode * const parentNode, struct RBNode * const childNode) {
  return notNull(parentNode) && leftChildOf(parentNode) == childNode;
}

bool isRightChildOf(struct RBNode * const parentNode, struct RBNode * const childNode){
  return notNull(parentNode) && rightChildOf(parentNode) == childNode;
}

struct RBNode * searchNode(struct RBNode * root, char* data, int k){
    /*if(!root || strncmp(root -> data, data, k)== 0)
        return root;
    
    if(strncmp(root -> data , data, k) < 0){
        return  searchNode(rightChildOf(root), data, k);
    }else{
        return searchNode(leftChildOf(root), data, k);
    }*/
    
    while(root!= NULL && strncmp(data, root ->data, k)!=0) {
        if(strcmp(data, root ->data)<0) {
            root=root->left;
        } else {
            root=root->right;
        }
    }
    return root;
}

int nodeNumber(struct RBNode * root){
    if (root == NULL) {
        return 0;
    }
    return 1 + nodeNumber(root->left) + nodeNumber(root->right);
}

void makeParentOf(struct RBNode * const childNode, struct RBNode * const parentNode){
  if(childNode) childNode->parent = parentNode;
}
void makeLeftChildOf(struct RBNode * const parentNode, struct RBNode * const leftChild){
  if(parentNode) parentNode->left = leftChild;
}
void makeRightChildOf(struct RBNode * const parentNode, struct RBNode * const rightChild){
  if(parentNode) parentNode->right = rightChild;
}

struct RBNode *uncleOf(struct RBNode * const root){ //se passi un nodo ti restituisce il fratello del padre
  struct RBNode * const rootParent = parentOf(root);
  if(notNull(rootParent)){
    struct RBNode * const rootGrandParent = grandParentOf(root);
    if(notNull(rootGrandParent)) {
      return isLeftChildOf(rootGrandParent, rootParent)
             ? rightChildOf(rootGrandParent)
             : leftChildOf(rootGrandParent);
    }
  }
  return NULL;
}

struct RBNode * getRoot(struct RBNode * node){
    struct RBNode * root;
    root = node;
    while(root -> parent != NULL){
        root = root -> parent;
    }
    return root;
}

void leftRotate(struct RBNode * const rbNode){
  struct RBNode * parent;
  if(notNull(rbNode) && notNull(parent = parentOf(rbNode))){

    struct RBNode * const grandParent = parentOf(parent);
    
    struct RBNode * const leftChildOfCurrentNode = leftChildOf(rbNode);
    
    if(notNull(grandParent)){
      if(isLeftChildOf(grandParent, parent)) makeLeftChildOf(grandParent, rbNode);
      else makeRightChildOf(grandParent, rbNode);
    }

    makeParentOf(rbNode, grandParent);
    makeParentOf(parent, rbNode);

    makeLeftChildOf(rbNode, parent);
    makeRightChildOf(parent, leftChildOfCurrentNode);
  }
  return;
}

void rightRotate(struct RBNode * const rbNode){
  struct RBNode * parent;

  if(notNull(rbNode) && notNull(parent = parentOf(rbNode))) {
      
    struct RBNode * const grandParent  = parentOf(parent);

    struct RBNode * const rightChildOfCurrNode = rightChildOf(rbNode);
    
    if(notNull(grandParent)) {
      if(isLeftChildOf(grandParent, parent)) makeLeftChildOf(grandParent, rbNode);
      else makeRightChildOf(grandParent, rbNode);
    }

    makeParentOf(rbNode, grandParent);
    makeParentOf(parent, rbNode);

    makeLeftChildOf(parent, rightChildOfCurrNode);
    makeRightChildOf(rbNode, parent);
  }
  return;
}

void freeRBTree(struct RBNode *node) {
    if (node == NULL) {
        return;
    }
    freeRBTree(node->left);
    freeRBTree(node->right);
    free(node->data);
    free(node);
}

void organizeTreeAfterInsertion(struct RBNode * rbNode){
    struct RBNode * parent = parentOf(rbNode);
    if(rbNode && !(parent && parent->color == BLACK)){ //se il nodo passato come parametro è != NULL e o suo padre è NULL o il colore di suo padre non è BLACK
        if(!parent) { //se il padre è NULL
            rbNode->color = BLACK;
        }else if(!grandParentOf(rbNode)){
            return;
           //non fa nulla per riorganizzarlo
        }
        else { //altrimenti (quindi se il colore di suo padre è RED)
           //check uncle
            struct RBNode * uncle = uncleOf(rbNode);
            struct RBNode * grandParent = grandParentOf(rbNode);
            if(uncle && uncle->color == RED){ //CASO 1 -> se lo zio non è NULL ed è ROSSO
               parent->color = BLACK;
               uncle->color  = BLACK;
               grandParent->color = RED;
               organizeTreeAfterInsertion(grandParent);
            }else{
               struct RBNode * grandParent = grandParentOf(rbNode);
               
                //se rbNode -> parent == leftChild(rbNode -> parent -> parent)
               if(parent == grandParent -> left){
                       if(rbNode == rightChildOf(parent)){//CASO 2 e 3 a SX se rbNode == rightChild(rbNode -> parent)
                           leftRotate(rbNode);
                           rbNode -> color = BLACK;
                           parent -> color = RED;
                           rightRotate(rbNode);
                       }else{ //CASO 3 se rbNode == leftChild(rbNode -> parent)
                           if(grandParent){
                               parent -> color = BLACK;
                               grandParent -> color = RED;
                               rightRotate(parent);
                           }
                   }
               }else{ //se rbNode -> parent == rightChild(rbNode -> parent -> parent)
                   if(rbNode == leftChildOf(parent)){ //CASO 2 e 3 a DX se rbNode == leftChild(rbNode -> parent)
                       rightRotate(rbNode);
                       rbNode -> color = BLACK;
                       parent -> color = RED;
                       leftRotate(rbNode);
                   }else{ //CASO 3 se rbNode == rightChild(rbNode -> parent)
                       if(grandParent){
                           parent -> color = BLACK;
                           grandParent -> color = RED;
                           leftRotate(parent);
                       }
                   }
               }
            }
        }
    }
    return;
}

void insertRBNode(struct RBNode * root, struct RBNode * newNode, int k){
    if(root){
        struct RBNode * previousNode = NULL;
        struct RBNode * currentNode  = root;
        //printf("%s\n", currentNode ->data);
        
        while(currentNode) { //finchè cur != NULL
           //value is already there
           if(strncmp (currentNode->data, newNode -> data, k) == 0) return;

           previousNode = currentNode;
           currentNode  = (strncmp(currentNode->data , newNode -> data, k) > 0) ? currentNode->left : currentNode->right;
            //printf("%s\n", currentNode ->data);
        }
            
        if(strncmp (previousNode->data , newNode -> data, k) > 0)
            previousNode->left = newNode;
        else
            previousNode->right = newNode;
        
        newNode->parent = previousNode;
        
        //organizeTreeAfterInsertion(newNode);
        
    }
    
    return;
}






















void filtraVincoli(struct RBNode * node, char* riferimento, int k, struct pos* vPos, struct zero* vZero, struct num* vNum) {
    int indice = 0;
    int numInP;
    
    for(int i = 0; i < k; i++){
        indice = (int)node -> data[i];
        if(vZero[indice].zeroOcc == 1){
            node -> filtrato = 0;
            return; //vincolo 2
        }else if (vPos[indice].posizione[i] == 2){
            node -> filtrato = 0;
            return; //vincolo 3 e vincolo 1
        }
    }
    
    for(int i = 0; i < 123; i++){
        if(vNum[i].attivo == 1){
            numInP = 0;
            for(int j = 0; j < k; j++){
                if((int)node ->data[j] == vNum[i].lettera){
                    numInP = numInP + 1;
                }
            }
            if(vNum[i].ugualeOMin == 0 && vNum[i].numero != numInP){
                node -> filtrato = 0;
                return; //vincolo 5
            }
            if(vNum[i].ugualeOMin == 1 && vNum[i].numero > numInP){
                //printf("\nFILTRO vNum su %s\n\n", node -> data);
                node -> filtrato = 0;
                return; //vincolo 4
            }
        }
    }
    
    
    return;
}

void inserisciFiltrando(struct RBNode * parAmmissibiliRoot, struct pos* vPos, struct zero* vZero, char* riferimento, char* string, struct num* vNum, int k){
    struct RBNode* s;
    s = createNode(string, k);
    filtraVincoli(s, riferimento, k, vPos, vZero, vNum);
    insertRBNode(parAmmissibiliRoot, s, k);
}

void inserisciFiltrandoNew(struct RBNode * parAmmissibiliRoot, struct pos* vPos, struct zero* vZero, char* riferimento, char* string, struct num* vNum,struct RBNode * newN, char* newC, int k, int *occupazioneMemoria){
    struct RBNode* s;
    if(*occupazioneMemoria == 100){
        newN = (struct RBNode*)malloc(sizeof(struct RBNode)*100);
        newC = (char*)malloc((k+1)*100);
        *occupazioneMemoria = 0;
    }
    s = createNodeDaNew(string, k, newN, newC, *occupazioneMemoria);
    filtraVincoli(s, riferimento, k, vPos, vZero, vNum);
    insertRBNode(parAmmissibiliRoot, s, k);
    *occupazioneMemoria = *occupazioneMemoria + 1;
    
}


void stampaFiltrate(struct RBNode *root) {
    if (root == NULL) {
        return;
    }
    stampaFiltrate(root -> left);
    if (root->filtrato == 1) {
        printf("%s\n", root -> data);
    }
    stampaFiltrate(root -> right);
}

void stampaAlbero(struct RBNode * root){
    if (root == NULL) {
        return;
    }
    stampaAlbero(root -> left);
    printf("%s\n", root -> data);
    stampaAlbero(root -> right);
    
}

void aggiungiVincoloPos(struct pos* vPos, char c, int indice, int k){
    for(int i = 0; i < 123; i++){
        if(i == (int)c){
            vPos[i].posizione[indice] = 1;
        } else {
            vPos[i].posizione[indice] = 2;
        }
    }
}

void aggiungiVincoloNoPos(struct pos* vPos, char c, int indice, int k){
    vPos[(int)c].posizione[indice] = 2;
    return;
}

void aggiungiVincoloZero(struct zero* vZero, char c, int i, int k){
    vZero[(int)c].zeroOcc = 1;
    return;
}

void aggiungiVincoloUguale(struct num* vNum, char c, int indice, int k, int ci){
    vNum[(int)c].numero = ci;
    vNum[(int)c].attivo = 1;
    vNum[(int)c].ugualeOMin = 0;
    return;
}

void aggiungiVincoloMinimo(struct num* vNum, char c, int indice, int k, int ci){
    if(vNum[(int)c].attivo == 0 || (vNum[(int)c].attivo == 1 && vNum[(int)c].ugualeOMin == 1 && vNum[(int)c].numero < ci)){
        vNum[(int)c].numero = ci;
        vNum[(int)c].attivo = 1;
        vNum[(int)c].ugualeOMin = 1;
        return;
    }
}

int nodeNumberFiltro(struct RBNode * root, char* riferimento, int k, struct pos* vPos, struct zero* vZero, struct num* vNum){
    if (root == NULL) {
        return 0;
    }
    if(root -> filtrato == 0){
        return nodeNumberFiltro(root->left, riferimento, k, vPos, vZero, vNum) + nodeNumberFiltro(root->right, riferimento, k, vPos, vZero, vNum);
    } else {
        filtraVincoli(root, riferimento, k, vPos, vZero, vNum);
        if(root -> filtrato == 0){
            return nodeNumberFiltro(root->left, riferimento, k, vPos, vZero, vNum) + nodeNumberFiltro(root->right, riferimento, k, vPos, vZero, vNum);
        } else {
            //filtraVincoli(root, riferimento, k, vPos, vZero, vNum);
            return 1 + nodeNumberFiltro(root->left, riferimento, k, vPos, vZero, vNum) + nodeNumberFiltro(root->right, riferimento, k, vPos, vZero, vNum);;
        }
    }
}

void traduzione(char* r, char* p, int k, struct RBNode *root, struct pos* vPos, struct zero* vZero, struct num* vNum, int *numFil){
    char * ris;
    int ni;
    int ci;
    int di;
    int min;
    int ugu;
    int f = 0;
    
    ris = (char*) malloc(k * sizeof(char));
    
    for(int i = 0; i < k; i++){
        if(*(p+i) == *(r+i)){
            *(ris+i) = '+';
            if(vPos[(int)(*(p+i))].posizione[i] == 0){
                aggiungiVincoloPos(vPos, *(p+i), i, k);
                f = 1;
            }
        }else{
            ni = 0;
            ci = 0;
            for(int j = 0; j < k; j++){
                if(*(r+j) == *(p+i)){
                    ni = ni + 1;
                }
                if(*(r+j) == *(p+i) && *(r+j) == *(p+j)){
                    ci = ci + 1;
                }
            }
            if(ni == 0){
                *(ris+i) = '/';
                
                if(vZero[(int)(*(p+i))].zeroOcc == 0){
                    aggiungiVincoloZero(vZero, *(p+i), i, k);
                    f = 1;
                }
            }else{
                di = 0;
                for(int j = 0; j < i; j++){
                    if((*(p+j) == *(p+i)) && (*(p+j) != *(r+j))){
                        di = di + 1;
                    }
                }
                if(di >= ni - ci){
                    *(ris+i) = '/';
                    
                    if(vPos[(int)(*(p+i))].posizione[i] == 0){
                        aggiungiVincoloNoPos(vPos, *(p+i), i, k);
                        f = 1;
                    }
                    if(ni != 0){
                        ugu = 0;
                        for(int h = 0; h < k; h++){
                            if(*(r+h) == *(p+i)){
                                ugu = ugu + 1;
                            }
                        }
                        if(vNum[(int)(*(p+i))].attivo == 0 || vNum[(int)(*(p+i))].ugualeOMin == 1){
                            aggiungiVincoloUguale(vNum, *(p+i), i, k, ugu);
                            f = 1;
                        }
                        
                        //aggiungiVincoloUguale(vNum, *(p+i), i, k, ci);
                    }
                      
                }else{
                    *(ris+i) = '|';
                    if(vPos[(int)(*(p+i))].posizione[i] == 0){
                        aggiungiVincoloNoPos(vPos, *(p+i), i, k);
                        f = 1;
                    }
                    /*
                    for(int h = 0; h < k; h++){
                        if(*(p+i) == *(p+h) && *(ris+h) == '+'){
                            aggiungiVincoloMinimo(vNum, *(p+i), i, k, ci);
                        }
                    }*/
                }
            }
        }
    }
    
    //ris stringa risultato
    for(int i = 0; i < k; i++){
        min = 0;
        for(int j = 0; j < k; j++){
            if((*(ris+j) == '|' || *(ris+j) == '+') && *(p+j) == *(p+i)){
                min = min + 1;
            }
        }
        if(min > 0){
            if(vNum[(int)(*(p+i))].attivo == 0 || (vNum[(int)(*(p+i))].ugualeOMin == 1 && vNum[(int)(*(p+i))].numero < min)){
                aggiungiVincoloMinimo(vNum, *(p+i), i, k, min);
                f = 1;
            }
            
            //printf("min > 0, per %c\n", *(p+i));
        }
        printf("%c", *(ris+i));
    }
    if(f == 1){
        *numFil = nodeNumberFiltro(root, r, k, vPos, vZero, vNum);
        printf("\n%d\n", *numFil);
    }else{
        printf("\n%d\n", *numFil);
    }
    free(ris);
}

void filtroAUno(struct RBNode *root) {
    if (root == NULL) return;

    filtroAUno(root->left);
    root->filtrato = 1;
    filtroAUno(root->right);
}

int notExists(char* p, struct RBNode* parAmmissibiliRoot, int k){
    /*if(notNull(searchNode(parAmmissibiliRoot, p)))
        return 0;
    else
        return 1;*/
    if (searchNode(parAmmissibiliRoot, p, k) == NULL) {
        return 1;
    } else {
        return 0;
    }
}

void resettoVincoli(struct pos* vPos, struct zero* vZero, struct num* vNum, int k){
    /*
    for(int i = 0; i < 64; i++){
        if(i < 26){
            vPos[i].lettera = (char)65+i;
            for(int s = 0; s < k; s++){
                vPos[i].posizione[s] = 0;
            }
            
            vZero[i].lettera = (char)65+i;
            vZero[i].zeroOcc = 0;
            
            vNum[i].lettera = (char)65+i;
            vNum[i].numero = 0;
            vNum[i].ugualeOMin = 0;
            vNum[i].attivo = 0;
        }
        if(i < 26+26 && i > 25){
            vPos[i].lettera = (char)97+i-26;
            for(int s = 0; s < k; s++){
                vPos[i].posizione[s] = 0;
            }
            
            vZero[i].lettera = (char)97+i-26;
            vZero[i].zeroOcc = 0;
            
            vNum[i].lettera = (char)97+i-26;
            vNum[i].numero = 0;
            vNum[i].ugualeOMin = 0;
            vNum[i].attivo = 0;
        }
        
        if(i < 26+26+10 && i > 26+26-1){
            vPos[i].lettera = (char)48+i-26-26;
            for(int s = 0; s < k; s++){
                vPos[i].posizione[s] = 0;
            }
            
            vZero[i].lettera = (char)48+i-26-26;
            vZero[i].zeroOcc = 0;
            
            vNum[i].lettera = (char)48+i-26-26;
            vNum[i].numero = 0;
            vNum[i].ugualeOMin = 0;
            vNum[i].attivo = 0;
        }
        
        if(i == 26+26+10){
            vPos[i].lettera = '-';
            for(int s = 0; s < k; s++){
                vPos[i].posizione[s] = 0;
            }
            
            vZero[i].lettera = '-';
            vZero[i].zeroOcc = 0;
            
            vNum[i].lettera = '-';
            vNum[i].numero = 0;
            vNum[i].ugualeOMin = 0;
            vNum[i].attivo = 0;
        }
        if(i == 26+26+10+1){
            vPos[i].lettera = '_';
            for(int s = 0; s < k; s++){
                vPos[i].posizione[s] = 0;
            }
            
            vZero[i].lettera = '_';
            vZero[i].zeroOcc = 0;
            
            vNum[i].lettera = '_';
            vNum[i].numero = 0;
            vNum[i].ugualeOMin = 0;
            vNum[i].attivo = 0;
        }
    }*/
    
    for(int i = 0; i < 123; i++){
        vPos[i].lettera = i;
        for(int s = 0; s < k; s++){
            vPos[i].posizione[s] = 0;
        }
        
        vZero[i].lettera = i;
        vZero[i].zeroOcc = 0;
        
        vNum[i].lettera = i;
        vNum[i].numero = 0;
        vNum[i].ugualeOMin = 0;
        vNum[i].attivo = 0;
    }
}


int main(int argc, const char * argv[]) {
    struct RBNode* parAmmissibiliRoot;
    //struct RBNode* tutteLeParole;
    //struct RBNode* parFiltrateRoot;
    char stringa[256];
    char* riferimento;
    int k; //lunghezza parole
    long int n; //numero parole da confrontare
    int ok;
    struct pos* vPos;
    struct zero* vZero;
    struct num* vNum;
    struct RBNode* newN;
    char* newC;
    int occupazioneMemoria;
    int numFil;
    
    /*
    FILE* fp;

        fp = fopen("/Users/filippocorna/Desktop/prova finale (progetto di algoritmi e strutture dati)/open_testcases/test3.txt", "r");
        if (fp == NULL) {
            perror("Error opening file");
            return(-1);
        }*/
    
    //leggo k
    if(fscanf(stdin,"%d", &k)== EOF) {
    return 0;
    }

    //inizializzazione
    vPos = (struct pos*)malloc(sizeof(struct pos)*123);
    vZero = (struct zero*)malloc(sizeof(struct zero)*123);
    vNum = (struct num*)malloc(sizeof(struct num)*123);
    for(int i = 0; i < 123; i++){
        vPos[i].posizione = (int*)malloc(sizeof(int)*k);
    }
    resettoVincoli(vPos, vZero, vNum, k);
    
    parAmmissibiliRoot = NULL;
    
    
    newN = (struct RBNode*)malloc(sizeof(struct RBNode)*100);
    newC = (char*)malloc((k+1)*100);
    occupazioneMemoria = 0;
    
    if(fgets(stringa, 256, stdin)==NULL) {
        return 0;
    }
    
     if(fgets(stringa, 256, stdin)==NULL) {
         return 0;
     }
    
    numFil = 0;
         
    //raccolgo parole ammissibili
    if(stringa[0]!='+'){
        
        if(occupazioneMemoria == 100){
            newN = (struct RBNode*)malloc(sizeof(struct RBNode)*100);
            newC = (char*)malloc((k+1)*100);
            occupazioneMemoria = 0;
        }
        stringa[k] = '\0';
        parAmmissibiliRoot = createNodeDaNew(stringa, k, newN, newC, occupazioneMemoria);
        occupazioneMemoria = occupazioneMemoria + 1;
        /*
        stringa[k] = '\0';
        parAmmissibiliRoot = createNode(stringa, k);*/
        
        if(fgets(stringa, 256, stdin)==NULL) {
            return 0;
        }
    }
    while(stringa[0]!='+') {
        
        
        if(occupazioneMemoria == 100){
            newN = (struct RBNode*)malloc(sizeof(struct RBNode)*100);
            newC = (char*)malloc((k+1)*100);
            occupazioneMemoria = 0;
        }
        
        stringa[k] = '\0';
        insertRBNode(parAmmissibiliRoot, createNodeDaNew(stringa, k, newN, newC, occupazioneMemoria), k);
         occupazioneMemoria = occupazioneMemoria + 1;
        /*
        stringa[k] = '\0';
        insertRBNode(parAmmissibiliRoot, createNode(stringa, k), k);*/
        
        
        if(fgets(stringa, 256, stdin)==NULL) {
            return 0;
        }
    }
    
    //stampaFiltrate(parAmmissibiliRoot);
    numFil = nodeNumber(parAmmissibiliRoot);
    
    do{
        if(stringa[0] == '+' && stringa[1] == 'n') {
            if(fgets(stringa, 256, stdin)==NULL) {
                return 0;
            }
            stringa[k]='\0';
            riferimento = (char*)malloc(k+1);
            strcpy(riferimento, stringa);
            //printf("RIFERIMENTO = %s\n", riferimento);
            
            if(fscanf(stdin, "%ld", &n)==EOF) {
                return 0;
            }
            
            if(fgets(stringa, 256, stdin)==NULL) {
                return 0;
            }
            stringa[k]='\0';
            ok = 0;
            while(n>0 && ok==0) {
                if(fgets(stringa, 256, stdin)==NULL) {
                    return 0;
                }
                if(stringa[0] == '+' && stringa[1] == 'i') {
                    if(fgets(stringa, 256, stdin)==NULL) {
                        return 0;
                    }
                    while(!(stringa[0] == '+' && stringa[1] == 'i')){
                        
                        if(occupazioneMemoria == 100){
                            newN = (struct RBNode*)malloc(sizeof(struct RBNode)*100);
                            newC = (char*)malloc((k+1)*100);
                            occupazioneMemoria = 0;
                        }
                        stringa[k]='\0';
                        inserisciFiltrandoNew(parAmmissibiliRoot, vPos, vZero, riferimento, stringa, vNum, newN, newC, k, &occupazioneMemoria);
                        /*
                        stringa[k]='\0';
                        inserisciFiltrando(parAmmissibiliRoot, vPos, vZero, riferimento, stringa, vNum, k);*/
                        
                        if(fgets(stringa, 256, stdin)==NULL) {
                            return 0;
                        }
                    }
                } else if (stringa[0] == '+' && stringa[1] == 's'){
                    stampaFiltrate(parAmmissibiliRoot);
                } else if(strncmp(stringa, riferimento, k)==0) {
                    printf("ok\n");
                    ok = 1;
                } else {
                    if(notExists(stringa, parAmmissibiliRoot, k) == 1) {
                        printf("not_exists\n");
                    } else {
                        traduzione(riferimento, stringa, k, parAmmissibiliRoot, vPos, vZero, vNum, &numFil);
                        n = n - 1;
                    }
                }
            }
            if(ok == 0) {
                printf("ko\n");
            }
            
            filtroAUno(parAmmissibiliRoot);
            resettoVincoli(vPos, vZero, vNum, k);
            
        }  else {
            if(fgets(stringa, 256, stdin)==NULL) {
                return 0;
            }
            while(stringa[0]!='+') {
                
                if(occupazioneMemoria == 100){
                    newN = (struct RBNode*)malloc(sizeof(struct RBNode)*100);
                    newC = (char*)malloc((k+1)*100);
                    occupazioneMemoria = 0;
                }
                stringa[k] = '\0';
                insertRBNode(parAmmissibiliRoot, createNodeDaNew(stringa, k, newN, newC, occupazioneMemoria), k);
                occupazioneMemoria = occupazioneMemoria + 1;
                /*
                stringa[k] = '\0';
                insertRBNode(parAmmissibiliRoot, createNode(stringa, k), k);*/
                
                if(fgets(stringa, 256, stdin)==NULL) {
                    return 0;
                }
            }
            
        }
        numFil = nodeNumber(parAmmissibiliRoot);
    }while(fgets(stringa, 256, stdin)!=NULL);
    
    for(int i=0; i < 123; i++){
        free(vPos[i].posizione);
    }
    
    //freeRBTree(parAmmissibiliRoot);
    free(vPos);
    free(vZero);
    free(vNum);
    free(newN);
    free(newC);
    
    
    return 0;
}

