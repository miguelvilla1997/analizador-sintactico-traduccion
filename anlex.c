/*
 *  Analizador Léxico	
 *  Curso: Compiladores y Lenguajes de Bajo de Nivel
 *  Práctica de Programación Nro. 1
 *	
 *  Descripcion:
 *  Implementa un analizador léxico que reconoce números, literales 
 *  en cadena y signos de puntuación. para un lenguaje con sintaxis 
 *  tipo JSON.
*
 *  Autores
 *  - Miguel Villamayor
 *	
 *	
 */

/*********** Inclusión de cabecera **************/
#include "anlex.h"


/************* Variables globales **************/

token t;				// token global para recibir componentes del Analizador Lexico

// variables para el analizador lexico

FILE *fuente;			// Fuente json
char buff[2*TAMBUFF];	// Buffer para lectura de archivo fuente
char id[TAMLEX];		// Utilizado por el analizador lexico
int numLinea=1;			// Numero de Linea
int numAux=1; 			// Numero de Linea auxiliar utilizada para impresion

/**************** Funciones **********************/


// Rutinas del analizador lexico

void error(const char* mensaje)
{
    printf("Lin %d: Error Lexico. %s.\n",numLinea,mensaje);	
}


void siglex()
{
    int i=0;
    char c=0;
    int acepto=0;
    int estado=0;
    char msg[41];

    while((c=fgetc(fuente))!=EOF)
    {
        if (c==' ' || c=='\t'){

            continue;	//eliminar espacios en blanco
        }
        else if(c=='\n')
        {
            //incrementar el numero de linea
            numLinea++;
            continue;
        }
        else if (c==':')
        {
            t.compLex=DOS_PUNTOS;
            t.comp="DOS_PUNTOS";
			t.lexema=":";
            break;
        }
        else if (c=='{')
        {
            t.compLex=L_LLAVE;
            t.comp="L_LLAVE";
			t.lexema="{";
            break;
        }
        else if (c=='}')
        {
            t.compLex=R_LLAVE;
            t.comp="R_LLAVE";
			t.lexema="}";
            break;
        }
        else if (c=='[')
        {
            t.compLex=L_CORCHETE;
            t.comp="L_CORCHETE";
			t.lexema="[";
            break;
        }
        else if (c==']')
        {
            t.compLex=R_CORCHETE;
            t.comp="R_CORCHETE";
			t.lexema="]";
            break;
        }
        else if (c==',')
        {
            t.compLex=COMA;
            t.comp="COMA";
			t.lexema=",";
            break;
        }
        else if (c == '"')
        {
            
            //fgetc lee del archivo caracter por caracter a partir del puntero indicado en el archivo
            //es un STRING /*VERIFICA QUE SE INGRESA UN STRING VALIDO*/
            c=fgetc(fuente);
			i = 0;
			id[i]= c; 
            while(c!=EOF){
                if(c == '"'){
					id[i] = '\0';
                    t.compLex=STRING;
					t.comp="STRING";
					t.lexema = id;
                    break;
                }else if(i>=TAMLEX){
					ungetc(c, fuente);
				}
				else{
                    c=fgetc(fuente);
					id[++i] = c;
                }
            }
            if (c==EOF)
                error("Se llego al fin sin cerrar el String");
			else if(i>=TAMLEX){
				error("Longitud de Identificador excede tamaño de buffer");
			}
            break;
        }
                    
        else if (isdigit(c))//es un numero
        {
            i=0;
            estado=0;
            acepto=0;
            id[i]=c;

            while(!acepto)
            {
                switch(estado){
                    case 0: //una secuencia netamente de digitos
                        c=fgetc(fuente);
                        if (isdigit(c))
                        {
                            id[++i]=c;
                            estado=0;
                        }
                        else if(c=='.'){
                            id[++i]=c;
                            estado=1;
                        }
                        else if(tolower(c)=='e'){
                            id[++i]=c;
                            estado=3;
                        }
                        else{
                            estado=6;
                        }
                        break;

                    case 1://un punto, debe seguir un digito (caso especial de array, puede venir otro punto)
                        c=fgetc(fuente);						
                        if (isdigit(c))
                        {
                            id[++i]=c;
                            estado=2;
                        }
                        else if(c=='.')
                        {
                            i--;
                            fseek(fuente,-1,SEEK_CUR);
                            estado=6;
                        }
                        else{
                            sprintf(msg,"No se esperaba '%c'",c);
                            estado=-1;
                        }
                        break;
                    case 2://la fraccion decimal
                        c=fgetc(fuente);
                        if (isdigit(c))
                        {
                            id[++i]=c;
                            estado=2;
                        }
                        else if(tolower(c)=='e')
                        {
                            id[++i]=c;
                            estado=3;
                        }
                        else
                            estado=6;
                        break;
                    case 3://una e, puede seguir +, - o una secuencia de digitos
                        c=fgetc(fuente);
                        if (c=='+' || c=='-')
                        {
                            id[++i]=c;
                            estado=4;
                        }
                        else if(isdigit(c))
                        {
                            id[++i]=c;
                            estado=5;
                        }
                        else{
                            sprintf(msg,"No se esperaba '%c'",c);
                            estado=-1;
                        }
                        break;
                    case 4://necesariamente debe venir por lo menos un digito
                        c=fgetc(fuente);
                        if (isdigit(c))
                        {
                            id[++i]=c;
                            estado=5;
                        }
                        else{
                            sprintf(msg,"No se esperaba '%c'",c);
                            estado=-1;
                        }
                        break;
                    case 5://una secuencia de digitos correspondiente al exponente
                        c=fgetc(fuente);
                        if (isdigit(c))
                        {
                            id[++i]=c;
                            estado=5;
                        }
                        else{
                            estado=6;
                        }break;
                    case 6://estado de aceptacion, devolver el caracter correspondiente a otro componente lexico
                        if (c!=EOF)
                            ungetc(c,fuente);
                        else
                            c=0;
							id[++i]='\0';
							acepto=1;
							t.compLex=NUMBER;
							t.comp="NUMBER";
							t.lexema = id;
                        break;
                    case -1:
                        if (c==EOF)
                            error("No se esperaba el fin de archivo");
                        else
                            error(msg);
                    }
                }
            break;
        }
        else if (isalpha(c)) {
            i = 0;
            do{
                id[i] = c;
                i++;
                c = fgetc(fuente);
            }while (isalpha(c));
            id[i]='\0';
            if (c!=EOF)
                ungetc(c,fuente);
            if(strcmp(id, "true") == 0 || strcmp(id, "TRUE") == 0){
                t.compLex=PR_TRUE;
                t.comp="PR_TRUE";
				t.lexema = "true";
                break;
            }
            else if (strcmp(id, "false") == 0 || strcmp(id, "FALSE") == 0){
                t.compLex=PR_FALSE;
                t.comp="PR_FALSE";
				t.lexema = "false";
                break;
            }
            else if(strcmp(id, "null") == 0 || strcmp(id, "NULL") == 0){
                t.compLex=PR_NULL;
                t.comp="PR_NULL";
				t.lexema = "null";
                break;
            }
            else{
                sprintf(msg,"No se esperaba '%s'",id);
                error(msg);				
            }
        }
        else {
            sprintf(msg,"No se esperaba '%c'",c);
            error(msg);
        }
    }
    if (c==EOF)
    {
        t.compLex=EOF;
        t.comp="EOF";

    }
}
/*
int main(int argc,char* args[])
{
	// inicializar analizador lexico
	initTabla();
	initTablaSimbolos();
	
	if(argc > 1)
	{
        
		if (!(fuente=fopen(args[1],"rt")))
		{
			printf("Archivo no encontrado.\n");
			exit(1);
		}
		if (!(salida = fopen("salida.txt", "w"))) {
			printf("No se genera el archivo de salida.\n");
			exit(1);
		}        
		while (t.compLex!=EOF){
			sigLex();
			fprintf(salida, "%s",buscarCompLex(t.compLex));
		}
		fclose(fuente);
		fclose(salida);
        printf("Se ha generado el archivo 'salida.txt' \n");
	}else{
		printf("Debe pasar como parametro el path al Archivo fuente.\n");
		exit(1);
	}

	return 0;
}*/
