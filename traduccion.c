/*
    Traduccion dirigida por sintaxis
*/

#include "ansin.c"

FILE *salida;

void arrayTrad();
void arrTrad();
void attributeTrad();
void attNameTrad();
void attValueTrad();
void attributeListTrad();
void attrListTrad();
void elementTrad();
void objectTrad();
void objTrad();
void elementListTrad();
void elmListTrad();
void jsonTrad();
int tabulacion=0;

void print_tab(){
    int i=0;
    for (i=1;i<=tabulacion;i++){
		fprintf(salida," "); 
    }
}


void jsonTrad(){
	elementTrad();
}

void elementTrad(){
	
	if(t.compLex == L_LLAVE){
		objectTrad();
	}
	else if(t.compLex == L_CORCHETE){
		arrayTrad();
	}
}

void objectTrad(){
	if(t.compLex == L_LLAVE){
		match(L_LLAVE);
		objTrad();
	}
}

void objTrad(){
	if(t.compLex == STRING){
        attributeListTrad();
        match(R_LLAVE);

    }
    else if(t.compLex == R_LLAVE){
        match(R_LLAVE);
    }
}

void arrayTrad(){
	if(t.compLex == L_CORCHETE){   
        match(L_CORCHETE);
        arrTrad();
		print_tab();
    }	
}

void arrTrad(){
	if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
		fprintf(salida,"\n"); 
        print_tab();
		fprintf(salida,"<item>\n"); 
        tabulacion+=4;
        elementListTrad();
        print_tab();
		fprintf(salida,"</item>\n"); 
		tabulacion-=4; 
    }
    else if(t.compLex == R_CORCHETE){
        tabulacion-=4;
        match(R_CORCHETE);
    }
}

void attributeListTrad() {
	if(t.compLex == STRING){
        attributeTrad();
        attrListTrad();
    }
}

void attrListTrad(){
	if (t.compLex == R_LLAVE){
		tabulacion-=4;
    }
  
    if(t.compLex == COMA){
        match(COMA);
        attributeTrad();
        attrListTrad();
    }
}

void elementListTrad(){
	if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
        elementTrad();
        elmListTrad();   
    }
}

void elmListTrad(){
	if(t.compLex == R_CORCHETE){ 
        match(R_CORCHETE);
    }
	if(t.compLex == COMA){  
        match(COMA);
        print_tab();
		fprintf(salida,"</item>\n"); 
        print_tab();
		fprintf(salida,"<item>\n"); 
        tabulacion+=4;
        elementTrad();
        elmListTrad();
    }
}

void attributeTrad(){
	if(t.compLex == STRING){
        print_tab();
        char lexema[TAMLEX];
        strcpy(lexema,t.lexema);
		fprintf(salida,"<");
        attNameTrad();
		fprintf(salida,">"); 
        match(DOS_PUNTOS);
        attValueTrad();
		fprintf(salida,"</"); 
        fprintf(salida,"%s",lexema); 
		fprintf(salida,">\n");   
    }
}

void attNameTrad(){
	if(t.compLex == STRING){    
        char lexema[TAMLEX];
        strcpy(lexema,t.lexema);
        fprintf(salida,"%s",lexema); 
        match(STRING);
    }
}

void attValueTrad(){
	if(t.compLex == L_CORCHETE || t.compLex == L_LLAVE){
        tabulacion +=4;
        elementTrad();
    }
    else if(t.compLex == STRING){
		fprintf(salida," %s",t.lexema);
        match(STRING);
    }
    else if(t.compLex == NUMBER){
		fprintf(salida," %s",t.lexema);
        match(NUMBER);
    }
    else if(t.compLex == PR_TRUE){
		fprintf(salida," %s",t.lexema);
        match(PR_TRUE);
    }
    else if(t.compLex == PR_FALSE){
		fprintf(salida," %s",t.lexema);
        match(PR_FALSE);
    }
    else if(t.compLex == PR_NULL){
		fprintf(salida," %s",t.lexema);
        match(PR_NULL);
    }
}

int main (int argc,char* args[]){
    if(argc > 1)
    {
        if (!(fuente=fopen(args[1],"rt")))
        {
            printf("Archivo no encontrado.\n");
            exit(1);
        }
        siglex();
        json();
		salida=fopen("salida.xml","a");
        if(aceptar){
            fclose(fuente);
            fuente=fopen(args[1],"rt");
	    
            siglex();
            if (aceptar==1){
                 jsonTrad();
                 printf("El fuente es sintacticamente correcto\n");}
            else{
                  jsonTrad();
                  printf("El fuente tiene errores sintacticos.\n");
                  fprintf(salida,"\nEl fuente tiene errores sintacticos");
            }

        }
	else{	
		fprintf(salida,"\nEl fuente tiene errores sintacticos.\n");
		
	}
	fclose(salida);
    }else{
        printf("Debe pasar como parametro el path al archivo fuente.\n");
        exit(1);
    }

    return 0;
}
