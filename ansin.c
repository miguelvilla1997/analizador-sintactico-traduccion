/*
 *	Analizador sintactico descendente recursivo
*/

#include "anlex.c"

void attribute(int sincro[], int tam);
void obj(int sincro[], int tam);
void array(int sincro[], int tam);
void attributeList(int sincro[], int tam);
void attrList(int sincro[], int tam);
void elementList(int sincro[], int tam);
void elmList(int sincro[], int tam);
void attName(int sincro[], int tam);
void object(int sincro[], int tam);
void element(int sincro[], int tam);
void arr(int sincro[], int tam);
void attValue(int sincro[], int tam);
int aceptar = 1;

void errorS(){
    aceptar = 0;
    printf("\nLin %d: Error Sintactico. No se esperaba %s. ", numLinea, t.comp);
}

void match (int expToken) {
    if (t.compLex = expToken) {
    	siglex();
    }
}

int in(int vector[], int tam, int comp){
    int i = 0;
    while (i < tam) {
        if (vector[i] == comp) {
            return 1;
        }
        i++;
    }
    return 0;
}

void scanTo(int sincro[], int tam) {
    while (in(sincro, tam, t.compLex) == 0 && t.compLex != EOF) {
    	siglex();
    }
    siglex();
}

void checkInput(int primero[], int siguiente[], int tam1, int tam2) {
    int vunion[tam1+tam2];
    int i = 0, j = 0;
    while (i < tam1) {
        if (t.compLex == primero[i]) { return;}
        i++;
    }
    i = 0;
    while (i < tam1) {
        vunion[i] = primero[i];
        i++;
    }
    while (j < tam2) {
        vunion[i] = siguiente[j];
        i++;
        j++;
    }
    errorS();
    scanTo(vunion, tam1+tam2);
}


void json(){
    int primero[2] = {L_CORCHETE, L_LLAVE};
    int siguiente[1] = {EOF};
    checkInput(primero, siguiente, 2, 1);
    element(siguiente, 1);
}


void element(int sincro[], int tam){
    int primero[2] = {L_CORCHETE, L_LLAVE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, sincro, 2, tam);
    while (i < tam) {
        if (t.compLex == sincro[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_LLAVE) {
            object(siguiente, 4);
        }
        else if (t.compLex == L_CORCHETE) {
            array(siguiente, 4);
        }
    }
    else {
        errorS();
    }
    checkInput(siguiente, primero, 4, 2);
}


void object(int sincro[], int tam){
    int primero[1] = {L_LLAVE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, sincro, 1, tam);
    while (i < tam) {
        if (t.compLex == sincro[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_LLAVE) {
            match(L_LLAVE);
            obj(siguiente, 4);
        }
    }
    else {
        errorS();
    }
    checkInput(siguiente, primero, 4, 1);
}


void obj(int sincro[], int tam){
    int primero[2] = {R_LLAVE, STRING};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, sincro, 2, tam);
    while (i < tam) {
        if (t.compLex == sincro[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == R_LLAVE) {
            match(R_LLAVE);
        }
        else if (t.compLex == STRING) {
            attributeList(siguiente, 4);
            match(R_LLAVE);
        }
    }
    else {
        errorS();
    }
    checkInput(siguiente, primero, 4, 2);
}


void array(int sincro[], int tam){
    int primero[1] = {L_CORCHETE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    int i = 0, is = 0;
    checkInput(primero, sincro, 1, tam);
    while (i < tam) {
        if (t.compLex == sincro[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_CORCHETE) {
            match(L_CORCHETE);
            arr(siguiente, 4);
        }
    }
    else {
        errorS();
    }
    checkInput(siguiente, primero, 4, 1);
}


void arr(int sincro[], int tam){
    int primero[3] = {R_CORCHETE, L_CORCHETE, L_LLAVE};
    int siguiente[4] = {COMA, R_CORCHETE, R_LLAVE, EOF};
    checkInput(primero, sincro, 3, tam);
    if (t.compLex == R_CORCHETE) {
        match(R_CORCHETE);
    }
    else if (t.compLex == L_LLAVE || t.compLex == L_CORCHETE) {
        elementList(siguiente, 4);
        match(R_CORCHETE);
    }
    checkInput(siguiente, primero, 4, 3);
}


void attributeList(int sincro[], int tam){
    int primero[1] = {STRING};
    int siguiente[1] = {R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, sincro, 1, tam);
    while (i < tam) {
        if (t.compLex == sincro[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == STRING) {
            attribute(siguiente, 1);
                    attrList(siguiente, 1);
        }
    }
    else {
        errorS();
    }
    checkInput(siguiente, primero, 1, 1);
}


void attrList(int sincro[], int tam){
    if (t.compLex == R_LLAVE){
        return;
    }

    int primero[1] = {COMA};
    int siguiente[1] = {R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, sincro, 1, tam);
    while (i < tam) {
        if (t.compLex == sincro[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == COMA) {
            match(COMA);
            attribute(siguiente, 1);
            attrList(siguiente, 1);
        }
    }
    checkInput(siguiente, primero, 1, 1);
}

void elementList(int sincro[], int tam){
    int primero[2] = {L_LLAVE, L_CORCHETE};
    int siguiente[1] = {R_CORCHETE};
    int i = 0, is = 0;
    checkInput(primero, sincro, 2, tam);
    while (i < tam) {
        if (t.compLex == sincro[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_CORCHETE || t.compLex == L_LLAVE) {
            element(siguiente, 1);
            elmList(siguiente, 1);
        }
    }
    else {
        errorS();
    }
    checkInput(siguiente, primero, 1, 2);
}

void elmList(int sincro[], int tam){
    if(t.compLex == R_CORCHETE){ 
       return;
    }
    int primero[1] = {COMA};
    int siguiente[1] = {R_CORCHETE};
    checkInput(primero, sincro, 1, tam);
    if (t.compLex == COMA) {
        match(COMA);
        element(siguiente, 1);
        elmList(siguiente, 1);
    }
    checkInput(siguiente, primero, 1, 1);
}

void attribute(int sincro[], int tam){
    int primero[1] = {STRING};
    int siguiente[2] = {COMA, R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, sincro, 1, tam);
    while (i < tam) {
        if (t.compLex == sincro[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == STRING) {
            attName(siguiente, 2);
            match(DOS_PUNTOS);
            attValue(siguiente, 2);
        }
    }
    else {
        errorS();
    }
    checkInput(siguiente, primero, 2, 1);
}

void attName(int sincro[], int tam){
    int primero[1] = {STRING};
    int siguiente[1] = {DOS_PUNTOS};
    int i = 0, is = 0;
    checkInput(primero, sincro, 1, tam);
    while (i < tam) {
        if (t.compLex == sincro[i]) {
            is = 1;
        }
        i++;
}
    if (is == 0) {
        if (t.compLex == STRING){
            match(STRING);
        }
    }
    else {
        errorS();
    }
    checkInput(siguiente, primero, 1, 1);
}


void attValue(int sincro[], int tam){
    int primero[7] = {L_LLAVE, L_CORCHETE, STRING, NUMBER, PR_TRUE, PR_FALSE, PR_NULL};
    int siguiente[2] = {COMA, R_LLAVE};
    int i = 0, is = 0;
    checkInput(primero, sincro, 7, tam);
    while (i < tam) {
        if (t.compLex == sincro[i]) {
            is = 1;
        }
        i++;
    }
    if (is == 0) {
        if (t.compLex == L_LLAVE || t.compLex == L_CORCHETE) {
            element(siguiente, 2);
        }
        else if (t.compLex == STRING) {
            match(STRING);
        }
        else if (t.compLex == NUMBER) {
            match(NUMBER);
        }
        else if (t.compLex == PR_TRUE) {
            match(PR_TRUE);
        }
        else if (t.compLex == PR_FALSE) {
            match(PR_FALSE);
        }
        else if (t.compLex == PR_NULL) {
            match(PR_NULL);
        }
}
    else {
        errorS();
    }
    checkInput(siguiente, primero, 2, 7);
}

/*
int main(int argc,char* args[]){

    if(argc > 1){
        if (!(fuente=fopen(args[1],"rt"))){
            printf("Archivo no encontrado.\n");
            exit(1);
        }
        siglex();
        json();
        if (aceptar == 1) {
            printf("El fuente es sintacticamente correcto\n");
        }
        fclose(fuente);
    }
    else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }
    return 0;
}*/

