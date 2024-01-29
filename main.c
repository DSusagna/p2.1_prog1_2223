/***********************************************************************
*
* @Propòsit: 	Simuli una possible interacció amb un metge per tal 
* 				d'indicar-nos informació rellevant pel que fa al nostre 
* 				estat de salut.
* @Autor:                        David Susagna Holgado
* @Data de creació:              13/03/2023
* @Data de l'última modificació: 15/03/2023
*
***********************************************************************/
// Llibreries del sistema
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define MAX_CHAR 		50		// Nombre màxim de caràcters d'una paraula.
#define MAX_MALATIES 	30		// Nombre màxim de caràcters.
#define MAX_SIMOTOMES 	10		// Nombre màxim de símptomes d'una malaltia.
#define MAX_LINE 		60		// Nombre màxim de caràcters d'una línia.
#define	USER_REQUEST 	1000	// Nombre màxim de caràcters dels símptomes introduïts per l'usuari.

// Tipus propis
typedef struct {
	char symptom_name[MAX_CHAR];
	int relevance;
} Symptom;

typedef struct {
		char disease_name[MAX_CHAR];
		int num_symptoms;
		int total_relevance;
		Symptom symptoms[MAX_SIMOTOMES];
} Diseases;

typedef struct {
	char disease[MAX_CHAR];
	char symptom[MAX_CHAR];
	int relevance;
} FileDisease;

typedef struct {
	char user_symptoms_name[MAX_CHAR];
} UserSymptoms;


// Procediments i funcions recursius
/***********************************************************************
*
* nº: 1
* @Finalitat:	Modifica una cadena de caràcters per retornar el seu 
				valor equivalent en un enter.
* @Paràmetres: 	int: i = Indicador per recórrer el bucle
* @Retorn:		Retorna un enter amb el valor.		
*
***********************************************************************/
int charToInt(char a[]){

	int num = 0, i = 0;

	while('\0' != a[i]){
		if(('0' <= a[i]) && ('9' >= a[i])){

			//num = (num + a[i] - '0');
			num = 10 * num + (a[i] - '0');
		}
		i++;
	}
	return(num);	
}

/***********************************************************************
*
* nº: 2
* @Finalitat:	Extreu una sèrie de caràcter d'una cadena i la guarda en una altra.
* @Paràmetres:  in: end = caràcter el qual indica que a cavat la cadena de dos caràcters.
*				in: origen[] = cadena de caràcters la qual es vol recorre.
*				in: destino[] = cadena de caràcters la qual es vol guardar la informació extreta.
*				in: i = indicador de la posició de la cadena en la qual començar i acaba.
* @Retorn:     	Retorna la posició en la qual es troba la cadena, per poder continuar recorrent-la.       
*
***********************************************************************/
int cpyString(char origen[], char destino[], int i, char end){

	int n = 0;

	n = 0;
	while(end != origen[i]){
		destino[n] = origen[i];
		i++;
		n++;
	}
	destino[n] = '\0';
	return(i);
}

/***********************************************************************
*
* nº: 3
* @Finalitat:	Obre un fitxer en funció de la cadena de caràcters que es passa per la capçalera.
* @Paràmetres:	in: format[MAX_CHAR] En quin format es vol obrir el fitxer.
*				in: file_name[MAX_CHAR] Cadena de caràcters amb el nom del fitxer.
* @Retorn:		Retorna un punter a fitxer amb la informació del document.
*
***********************************************************************/

FILE * openFile(char format[MAX_CHAR]){

	char file_name[MAX_CHAR];
	FILE *file = NULL;

	scanf("%s",file_name);
	file = fopen(file_name,format);
	
	return(file);
}

/***********************************************************************
*
* nº: 4
* @Finalitat:	Tancar el document i modificar on apunta el punter a NULL.
* @Paràmetres: 	in : *file Punter a fitxer.
* @Retorn:   	----.
*
***********************************************************************/
void closeFile(FILE **file){
	fclose(*file);
	*file = NULL;
}		

//** Procediments i funcions no recursius 

/***********************************************************************
*
* nº: 5
* @Finalitat:	Mosta per pantalla el menú inicial i sol·licita a l'usuari quina opció vol escollir.
* @Paràmetres:	in: char buffer[MAX_CHAR] Cadena de caràcters per recollir l'entrada de teclat.
* 				in: * opcio Punter d'enters el qual modifica el valor de la variable opció en el main.
* @Retorn:		----.
*
***********************************************************************/
void menu(int *opcio){
	
	char buffer[MAX_CHAR];

	printf("\n1. Show diseases | 2. Load research advances | 3. Help me! | 4. Quit \n");
	printf("\nEnter option: ");
	scanf("%s", buffer);
	if (0 == strcmp(buffer, "1")){
		*opcio = 1;
	}
	else{
		if (0 == strcmp(buffer, "2")){
			*opcio = 2;
		}
		else{
			if (0 == strcmp(buffer, "3")){
				*opcio = 3;
			}
			else{
				if (0 == strcmp(buffer, "4")){
					*opcio = 4;
				}
				else{
					//Output 3. Es produeixen diferents errades i s’informa a l’usuari que l’opció escollida és incorrecta
					printf("\nERROR: Wrong option\n");
					*opcio = 0;
				}
			}
		}
	}
}

/***********************************************************************
*
* nº: 6
* @Finalitat:	Llegeix tota la informació d'un document de text i el guarda en l'estructura "disease".
* @Paràmetres:	in: Diseases disease[] vector de l'estructura "Diseases" on s'emmagatzemarà tota la informació.
* 				in: int i = 0, k =0 --> Utilitzades com a índex per recórrer els bucles.
*				in: char num_simptomes_char[4]; Cadena de caràcters auxiliar per emmagatzemar el nombre de símptomes.
*				in: char line[MAX_LINE]; Cadena de caràcters auxiliar per emmagatzemar una línia del document de text.
*				in: char relevance[MAX_CHAR]; Cadena de caràcters auxiliar per emmagatzemar la rellevància de cada símptoma.
*				in: char buffer[MAX_LINE] Cadena de caràcters per netejar el buffer.
* @Retorn:		Retorna un enter el qual identifica la quantitat de malalties que tenim al document de text.
*
***********************************************************************/
int readDoc (Diseases disease[], FILE *doc){

	int i = 0, k =0;
	char num_simptomes_char[4];
	char line[MAX_LINE]; 
	char relevance[MAX_CHAR];
	char buffer[MAX_LINE];

	fgets(disease[k].disease_name, MAX_LINE, doc);
	disease[k].disease_name[strlen(disease[k].disease_name)-1] = '\0';  	// Eliminar '\n'.
	while(!feof(doc)){
		fscanf(doc, "%s", num_simptomes_char);
		disease[k].num_symptoms = charToInt(num_simptomes_char);
		if(0 < disease[k].num_symptoms){
			fgets(buffer, MAX_LINE, doc);
			for(i = 0; i < disease[k].num_symptoms; i++){

				fgets(line, MAX_LINE, doc);
				line[strlen(line)-1] = '\0';  	// Eliminar '\n'.

				cpyString(line, disease[k].symptoms[i].symptom_name, 0, '(');	
				disease[k].symptoms[i].symptom_name[strlen(disease[k].symptoms[i].symptom_name)-1] = '\0';  	// Eliminar ' '.

				cpyString(line, relevance, 0, '\0');
				disease[k].symptoms[i].relevance = charToInt(relevance); //Copy relevance
			}
		}
		k++;
		fgets(disease[k].disease_name, MAX_LINE, doc);
		disease[k].disease_name[strlen(disease[k].disease_name)-1] = '\0';  	// Eliminar '\n'.
	}
	return (k);	
}

/***********************************************************************
*
* nº: 7
* @Finalitat:	Imprimeix tota la informació que ens trobem de l'estructura de tipus Diseases
* @Paràmetres:	in: int i = 0, k =0 --> Utilitzades com a índex per recórrer els bucles.
* 				in: Diseases disease[] vector de l'estructura "Diseases" on s'emmagatzemarà tota la informació.
* 				in: disease_num nombre màxim de malalties
* @Retorn:		----.
*
***********************************************************************/
void printInfo (Diseases disease[], int disease_num){
	
	int k = 0, i = 0;

	for(k = 0; k< disease_num; k++){
		printf("%s ", disease[k].disease_name);
		printf("(");
		for(i = 0; i< disease[k].num_symptoms; i++){
			if(i != disease[k].num_symptoms-1){
				printf("%s#%d | ",disease[k].symptoms[i].symptom_name,disease[k].symptoms[i].relevance);
			}
			else{
				printf("%s#%d",disease[k].symptoms[i].symptom_name,disease[k].symptoms[i].relevance);
			}
		}
		printf(")\n");
	}
}

/***********************************************************************
*
* nº: 8
* @Finalitat:	En funció dels valors d'entrada d'un fitxer binari, és modificar el contingut i els valors de l'estructura disease.
* @Paràmetres:	in: FILE *bin informació del fitxer binari
* 				in: Diseases disease[] vector de l'estructura "Diseases" on s'emmagatzemarà tota la informació.
* 				in: disease_num nombre màxim de malalties
* 				in: int i = 0, k =0 --> Utilitzades com a índex per recórrer els bucles.
* 				int diseases_ignored = 0, symptoms_upgrade = 0, symptoms_added = 0; Utilitzades per imprimir l'informació que es requereix en aquest apartat.
*				int ignored = 0; Identifica quantes malalties han sigut ignorades.
*				int existeix = 0; identifica quants símptomes no existeixen.
*				FileDisease diseases; estructura auxiliar per comprar la informació del document binari i l'estructura disease.
* @Retorn:		----.
*
***********************************************************************/
void modificaInfo(Diseases disease[], FILE *bin, int disease_num){

	int diseases_ignored = 0, symptoms_upgrade = 0, symptoms_added = 0;
	int ignored = 0;
	int k = 0, j = 0;
	int existeix = 0;
	FileDisease diseases;

	fread(&diseases, sizeof(FileDisease), 1, bin);
		while(!feof(bin)){

			for(k = 0 ; k <disease_num; k++){
				if(0 != strcmp(disease[k].disease_name, diseases.disease)){
					ignored++;
				}	
				else{
					
					for(j = 0 ; j <disease[k].num_symptoms; j++){
						if(0 == strcmp(disease[k].symptoms[j].symptom_name, diseases.symptom)){
							
							if(disease[k].symptoms[j].relevance != diseases.relevance){
								disease[k].symptoms[j].relevance = diseases.relevance;
								symptoms_upgrade++;
							}
							existeix++;
						}
					}
					if(0 == existeix){
						disease[k].num_symptoms++;
						strcpy(disease[k].symptoms[disease[k].num_symptoms-1].symptom_name, diseases.symptom);
						disease[k].symptoms[disease[k].num_symptoms-1].relevance = diseases.relevance;
						symptoms_added++;
					}
					existeix = 0;
				}
			}
			if (ignored == disease_num){
				diseases_ignored++;
			}
			ignored = 0;
			fread(&diseases, sizeof(FileDisease), 1, bin);
		}
		// Output 6.
		printf("\n%d diseases ignored\n",diseases_ignored);
		printf("%d symptoms updated\n",symptoms_upgrade);
		printf("%d symptoms added\n",symptoms_added);
		diseases_ignored = 0;
		symptoms_upgrade = 0;
		symptoms_added = 0;
}

/***********************************************************************
*
* nº: 9
* @Finalitat:	Es sol·licita els símptomes que té l'usuari i es guarden en l'estructura user_symptoms per un futur processat.
* @Paràmetres:	in: UserSymptoms user_symptom estructura on es guardaran tots els símptomes.
*				in: char user[USER_REQUEST]; cadena de caràcters on es guarda tota la informació introduïda.
*				in: int i = 0,j = 0; index per recorre els bucles.
*				in: int num_user_symptoms = 1; comptador del nombre de símptomes.
*				in: char buffer; Caràcter per eliminar l'intró que bé guardada el buffer.
* @Retorn:		Retorna el nombre total de símptomes que ha introduït l'usuari.
*
***********************************************************************/
int UserSymptom(UserSymptoms user_symptoms[]){

	char user[USER_REQUEST];
	int i = 0,j = 0;
	int num_user_symptoms = 1;
	char buffer;

	printf("Enter your symptoms: ");
	scanf("%c",&buffer);
	fgets(user, USER_REQUEST, stdin);
	user[strlen(user)-1] = '\0';  	// Eliminar '\n'.

	while('\0' != user[i]){
		if ('+' == user[i]){
			num_user_symptoms++;
		}
		i++;
	}
	if(1 == num_user_symptoms){ 
		strcpy(user_symptoms[0].user_symptoms_name, user);
	}
	else{
		j = 0;
		for (i = 0; i<num_user_symptoms; i++){
			if(i != num_user_symptoms-1){
				j = cpyString(user, user_symptoms[i].user_symptoms_name, j, '+');
				j++;
			}
			else{
				j = cpyString(user, user_symptoms[i].user_symptoms_name, j, '\0');
			}
		}
	}
	return(num_user_symptoms);
}

/***********************************************************************
*
* nº: 10
* @Finalitat: 	Assigna el nombre de probabilitats de cada una de les malalties en funció dels símptomes indicats per l'usuari.
* @Paràmetres:	in: Diseases disease[] vector de l'estructura "Diseases" on s'emmagatzemarà tota la informació.
* 				in: disease_num nombre màxim de malalties
* 				in: num_user_symptoms el nombre total de símptomes que ha introduït l'usuari
*				in: UserSymptoms user_symptoms estructura on estan emmagatzemant tots els símptomes introduïts per l'usuari
* 				in: int k = 0,i = 0, j = 0 variables per recoure els bucles
* 				in: found = 0, already_found = 0,num_simptomes_txt = 0; identifica i conta els cops que s'ha trobat una malaltia.
* 
* @Retorn:		Retorna 1 o 0 en el cassó si s'ha trobat un del símptoma en l'estructura de tipus Diseases.
*
***********************************************************************/
int findProbable(int num_user_symptoms,int disease_num,UserSymptoms user_symptoms[],Diseases disease[]){


	int k = 0,i = 0, j = 0, found = 0, already_found = 0;
	int num_simptomes_txt = 0;

	for(k = 0; k < disease_num; k++){
		num_simptomes_txt = num_simptomes_txt + disease[k].num_symptoms;

		for(i = 0; i < disease[k].num_symptoms; i++){
			for(j = 0; j < num_user_symptoms; j++){
				if (0 == strcmp(disease[k].symptoms[i].symptom_name, user_symptoms[j].user_symptoms_name)){
					disease[k].total_relevance += disease[k].symptoms[i].relevance;
					already_found = 1;
				}
				else{
					found++;
				}
			}
		}
	}

	if ((found == (num_simptomes_txt*num_user_symptoms)) && (0 == already_found)){
		return(0);
	}
	else{
		return(1);
	}
}
/***********************************************************************
*
* nº: 11
* @Finalitat:	Ordena de major a menor un vector de tipus Diseases
* @Paràmetres:	in: int i = 0,j = 0, ord = 0; Variables auxiliar per recorre un bucle i per identificar si es continua o no.
* 				in: Diseases disease[] vector de l'estructura "Diseases" on s'emmagatzemarà tota la informació.
* 				in: disease_num nombre màxim de malalties
* 				in: Diseases aux Estructura de tipus Diseases per emmagatzemar l'estructura.
* @Retorn:		----.
*
***********************************************************************/
void ordenarProbable(int disease_num, Diseases disease[]){

	int i = 0,j = 0, ord = 0;
	Diseases aux;
	
	while(!ord){
		ord=1;
		for(j = 0; j < disease_num-1; j++){

			if(disease[j].total_relevance < disease[j+1].total_relevance){

				aux = disease[j];
				disease[j] = disease[j+1];
				disease[j+1] = aux;
				ord = 0;
			}
		}
		i++;
	}	
}
/***********************************************************************
*
* nº: 12
* @Finalitat:	Imprimeix per orde d'ascendent el nom de la malaltia en funció de total de rellevància.
* @Paràmetres:	in: Diseases disease[] vector de l'estructura "Diseases" on s'emmagatzemarà tota la informació.
* 				in: disease_num nombre màxim de malalties
* 				in: int k = 0; índex per recórrer el bucle.
* @Retorn:		
*
***********************************************************************/
void printProbable(int disease_num, Diseases disease[]){

	int k = 0;

	ordenarProbable(disease_num,disease);

	for(k = 0; k < disease_num; k++){
		if (0 < disease[k].total_relevance){
			printf("Total relevance %d: %s\n",disease[k].total_relevance, disease[k].disease_name);
			disease[k].total_relevance = 0;
		} 
	}
}

int main (){

	
	int opcio = 0;							// Indica l'opció del menú indicada per l'usuari.
	int disease_num = 0;					// Indica el nombre total de malalties que ens trobem en el document de text.
	FILE *doc = NULL;						// Punter a fitxer pel document de text.
	FILE *bin = NULL;						// Punter a fitxer pel document de binari.
	Diseases disease[MAX_MALATIES];
	UserSymptoms user_symptoms[MAX_SIMOTOMES];
	int num_user_symptoms = 0;				// Nombre de símptomes que introdueix l'usuari
	int found = 0;

	printf("Enter diseases file name: ");
	doc = openFile("r");

	if (NULL == doc){
		// Output 1. S'introdueix el nom d'un fitxer que no existeix. 
    	printf("\nERROR: Diseases file not found \n");
	}
	else{
		disease_num = readDoc(disease, doc);
		// Output 2. S'introdueix un fitxer correctament, es dóna la benvinguda i es mostra el menú principal.
		printf("Welcome to myDoctor! \n");

		do{
			menu(&opcio);
			switch(opcio){

				// Opció 1: Show diseases 
				case 1:
					printf("\nRegistered diseases: \n\n");
					printInfo(disease, disease_num);
					break;


				// Opció 2: Load research advances
				case 2:

					printf("Enter research file name: ");
					bin = openFile("rb");

					if(NULL != bin){	
						modificaInfo(disease, bin,disease_num);
						closeFile(&bin);
					}
					break;


				// Opció 3: Help me! 
				case 3:


					num_user_symptoms = UserSymptom(user_symptoms);
					found = findProbable(num_user_symptoms,disease_num,user_symptoms,disease);

					if(0 == found){
						printf("\nNo diseases found!\n");
					}
					else{
						printf("\nMost probable diseases: \n");
						printProbable(disease_num,disease);
						num_user_symptoms = 0;
					}				
					break;
			}
			


		}while(4 != opcio);

		// Output 11. L’opció 4 finalitza l’execució del programa.
		printf("\nStay healthy!\n");

		closeFile(&doc);
	}
	return(0);
}