////////////////////////////////////////////////////////////////////////////////
// Gozde DOGAN 131044019
// Homework 1
// 
// Description:
//      Girilen dosya icerisinde yine girilen stringi aradim.
//      String sayisini ve bulundugu konumun satir ve sutun numarasini 
//	ekrana yazdirdim
////////////////////////////////////////////////////////////////////////////////

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define DEBUG

//Function prototypes
void searchStringInFile(char *sFileName);
//Genel islemlerimi topladigim bir fonksiyon
char** readToFile(); 
//Dosyayi okuyup iki boyutlu stringe yazacak
void findLengthLineAndNumOFline();
//Dosyadaki satir sayisini ve en uzun satirin sütün sayisini hesapliyor.
int searchString(char* sFileName, char **sFile);
//string iki boyutlu string icinde arayacak
int copyStr(char **sFile, char* word, int iStartRow, int iStartCol, int *iRow, int *iCol);
//1 return ederse kopyalama yapti, 0 return ederse kopyalama yapamadi

int iNumOfLine = 0;
int iLengthLine = 0;

FILE *fPtrInFile;
#ifndef DEBUG
	FILE *fPtrLogFile;
#endif
char *sSearchStr;
int iSizeOfSearchStr = 0;

int iWordCount = 0;

int main(int argc, char *argv[]){

    if (argc != 3) {
        printf ("Usage>> \n");
        printf("./list \"searchString\" <fileName>\n");
        return 1;
    }
   
    iSizeOfSearchStr = strlen(argv[1]);
    if(argv[1] != NULL)
        sSearchStr = (char*)malloc(strlen(argv[1])*sizeof(char));
    
    strcpy(sSearchStr, argv[1]);
    //fprintf(stderr, "sSearchStr:%s\tsize:%d\n", sSearchStr, iSizeOfSearchStr);
    #ifndef DEBUG
		//Outputlarin yazilacagi dosyayi actim
		fPtrLogFile=fopen("gff.log", "a+");
		fprintf(fPtrLogFile, "input word >> %s\n\n", sSearchStr);
    #endif
    
    printf("\n*************************************************\n\n");
    searchStringInFile(argv[2]);
    printf("\n*************************************************\n");
    printf("  %d tane %s bulundu!\n", iWordCount, argv[1]);
    printf("*************************************************\n\n"); 
    
    free(sSearchStr);
    #ifndef DEBUG
		//Dosyalari kapattim
		fclose(fPtrLogFile);
    #endif
    
    return 0;
}

//Function Definitions
/**
    Yapilan islemler main de kafa karistirmasin diye hepsini bu fonksiyonda 
    topladim.

    sFileName : String, input, icinde arama yapilacak dosyanin adini tutuyor
*/
void searchStringInFile(char* sFileName){
    char **sStr;
    int i, j;

    //fprintf(stderr, "sFileName:%s\n", sFileName);
    //Burada adi verilen dosyanin acilip acilmadigina baktim
    //Acilamadiysa programi sonlandirdim.
    fPtrInFile = fopen (sFileName, "r");
    if (fPtrInFile == NULL) {
        perror (sFileName);
        exit(1);
    }


    //Dosyanin satir sayisini ve en uzun satirin 
    //column sayisini bulan fonksiyonu cagirdim.
    findLengthLineAndNumOFline();
    //Dosyayi tekrar kapatip acmak yerine dosyanin nerede oldugunu 
    //gosteren pointeri dosyanin basina aldim
    rewind(fPtrInFile);

    //Dosyayi string arrayine okudum ve bu string'i return ettim
    sStr=readToFile();

    #ifndef DEBUG //Dosyayi dogru okuyup okumadigimin kontrolü
        printf("File>>>>>>>\n");
        for(i=0; i<iNumOfLine; i++)
            printf("%s\n", sStr[i]);
    #endif
    
    
    //String arrayi icinde stringi aradim ve sayisini iWordCount'a yazdim
    iWordCount=searchString(sFileName, sStr);
    #ifndef DEBUG
		fprintf(fPtrLogFile, "\n*********************************\n");
		fprintf(fPtrLogFile, "        TotalWordCount:%d\n", iWordCount);
		fprintf(fPtrLogFile, "*********************************\n\n");
    #endif
    //Strin icin ayirdigim yeri bosalttim
    for(i=0; i<iNumOfLine; i++)
        free(sStr[i]);
    free(sStr);
    
    fclose(fPtrInFile);
}

/**
    Dosyadaki satir sayisini ve en uzun sutundaki karakter sayisini bulur.
    Burdan gelen sonuclara gore dynamic allocation yapilir.
*/
void findLengthLineAndNumOFline(){
	int iLenghtLine=0;
	int iMaxSize=0;
	char ch;

		while(!feof(fPtrInFile)){
			fscanf(fPtrInFile, "%c", &ch);
			iMaxSize++;
				if(ch == '\n'){
					iNumOfLine=iNumOfLine+1;
					if(iMaxSize >=(iLengthLine))
						iLengthLine=iMaxSize;
					iMaxSize=0;
				}
		}
		iNumOfLine-=1; //bir azalttim cunku dongu bir defa fazla donuyor ve iNumOfLine
                        //bir fazla bulunuyor.
        iLengthLine+=1;
        #ifndef DEBUG
            printf("iLengthLine:%d\tiNumOfLine:%d\n", iLengthLine, iNumOfLine);
        #endif
}

/**
    Dosya okunur ve iki boyutlu bir karakter arrayyine atilir.
    Karakter arrayi return edilir.    

    output: char**, okunan dosyayi iki boyutlu stringe aktardim ve 
            bu string arrayini return ettim.
*/
char** readToFile(){
    char **sFile;
    int i=0;

    //Ikı boyutlu string(string array'i) icin yer ayirdim
    sFile=(char **)malloc(iNumOfLine*iLengthLine*sizeof(char*));
    if( sFile == NULL ){ //Yer yoksa hata verdim
        #ifndef DEBUG
            printf("INSUFFICIENT MEMORY!!!\n");
        #endif
        exit(1);
    }
    //Ikı boyutlu oldugu ıcın her satir icinde yer ayirdim
    for(i=0; i<iNumOfLine; i++){
        sFile[i]=(char *)malloc(iLengthLine*sizeof(char));
        if( sFile[i] == NULL ){ //Yer yoksa hata verdim
            #ifndef DEBUG
                printf("INSUFFICIENT MEMORY!!!\n");
            #endif
            exit(1);
        }
    }

    i=0;
    do{ //Dosyayi okuyup string arrayine yazdim
    
        fgets(sFile[i], iLengthLine, fPtrInFile);
        #ifndef DEBUG
            printf("*-%s-*\n", sFile[i]);
        #endif
        i++;
    }while(!feof(fPtrInFile));

    return sFile;
}

/**
    String arama isleminin ve her yeni bir string bulundugunda bulunan 
    kelime sayisinin arttirildigi fonksiyon

    sFile     :String arrayi, input, ıcınde arama yapiacak string arrayi
    sFileName :String'in aranacagi dosya adi
    output degeri ise integer ve bulunan string sayisini return eder
*/
int searchString(char* sFileName, char **sFile){
    int i=0, j=0;
    int iRow=0, iCol=0;
    char *word;
    
    //string arrayinin her satirini sira ile str stringine kopyalayip inceleyecegim
    word=(char *)malloc(100*sizeof(char));


    for(i=0; i<iNumOfLine; i++){ //Satir sayisina gore donen dongu
        for(j=0; j<iLengthLine; j++){ //Sutun sayisina gore donen dongu
                //printf("i:%d\tj:%d\n", i, j);
            if((copyStr(sFile, word, i, j, &iRow, &iCol)) == 1){ //str stringine kopyalama yaptim
                //kopyalama ile sSearchStr esit mi diye baktim
                if(strcmp(word, sSearchStr) == 0){
                    //#ifndef DEBUG
                        printf("  [%d, %d] konumunda ilk karakter bulundu.\n", iRow, iCol);
                    //#endif
		    	#ifndef DEBUG
                	//Bulunan kelimenin satir ve sutun sayisi LogFile'a yazdim
                	fprintf(fPtrLogFile, "%s > [%d, %d]\n", sFileName, iRow, iCol);
				#endif
                    iWordCount++; //String sayisini bir arttirdim kelime buldugum icin
                }
            }
        }
    }
    
    free(word);
    return iWordCount; //Bulunan string sayisini return ettim
}

/**
   Aranmasi gereken stringin karakter sayisi kadar karakteri word stringine kopyalar.
   Kopyalama yaparken kopyalanan karakterin space(' '), enter('\n') ve tab('\t') 
   olmamasina dikkat ederek kopyalar.
   
   sFile    :Dosyadaki karakterlerin tutuldugu iki boyutlu karakter arrayi
   word     :Kopyalanan karakterlerin tutulacagi 1 karakter arrayi
   iStartRow:Aramanin baslayacagi satir indexi
   iStartCol:Aramanin baslayacagi sutun indexi
   iRow     :Bulunan kelimenin ilk karakterinin bulundugu satir numarasi
   iCol     :Bulunan kelimenin ilk karakterinin bulundugu sutun numarasi
*/
int copyStr(char **sFile, char* word, int iStartRow, int iStartCol, int *iRow, int *iCol){

    int k=0, i=0, j=0;
    //printf("iStartRowIndex:%d\tiStartColIndex:%d\n", iStartRow, iStartCol);
    
    if(sFile[iStartRow][iStartCol] == '\n' || sFile[iStartRow][iStartCol] == '\t' || sFile[iStartRow][iStartCol] == ' '){
        return 0;
    }  
    else{
        *iRow = iStartRow+1;
        *iCol = iStartCol+1;
	#ifndef DEBUG
    	printf("iRow:%d\tiCol:%d\n", *iRow, *iCol);
		printf("iStartRow:%d\tiStartCol:%d\n", iStartRow, iStartCol);
	#endif
        k=0;
        int jStart = *iCol-1;
        for(i=*iRow-1; i<iNumOfLine && k < iSizeOfSearchStr; i++){
            for(j=jStart; j<iLengthLine && k < iSizeOfSearchStr; j++){
		if(sFile[i][j] != '\n' && sFile[i][j] != '\t' && sFile[i][j] != ' ' && k < iSizeOfSearchStr){
                    word[k] = sFile[i][j];
                    k++;
                }                
		if(sFile[i][j] == '\n' && k < iSizeOfSearchStr){
                    j=iLengthLine;
                }
            }
	    jStart=0; //jnin bir alt satirda baslangic konumu 0 olarak ayarlandi
            
        }   
        
        if(k != iSizeOfSearchStr)
            return 0;
        else
            return 1;
    
    }
}
