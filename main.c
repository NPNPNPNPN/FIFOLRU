#include <iostream>
using namespace std;

void PrintMatriz(char Procesos[5][30], char Requests[30], double CambiosPagina, double Rendimiento, char Cambios[30]){//Proceso para imprimir la tabla
  for(int r=0;r<129;r++){
    cout << "-";}//Linea superior
  cout << endl << "Proceso | ";
  for(int f=0;f<30;f++){
    cout << Requests[f] << " | ";}
  cout << endl << endl;//Procesos
  for(int i=0;i<5;i++){
  cout << "Marco " << i << " | ";
  for(int n=0;n<30;n++){
    cout << Procesos[i][n] << " | ";}
  cout << endl;//Marcos de Pagina
  }
  cout << "Cambios | ";
  for(int n=0;n<30;n++){
    cout << Cambios[n] << " | ";}
  cout << endl;//Señal para cambios de pagina
  for(int r=0;r<129;r++){
    cout << "-";}
  cout << endl;//Linea inferior
  cout << "Cambios de Pagina = " << CambiosPagina << endl;
  cout << "Rendimiento = " << Rendimiento << " %" << endl;
}

//Funcion para generar 30 requests aleatorias
void Request_gen(char (&Requests)[30])
{
  srand (time(NULL));//random time seed
  for(int i=0;i<31;i++){
    Requests[i] = ('A' + rand()%10);//random character between a and j
  }
}

//Funcion para llenar de "Nulls" la matriz de procesos
void FillNullProcesos(char (&Procesos)[5][30]){
  for(int i=0;i<5;i++){
    for(int n=0;n<30;n++){
      Procesos[i][n] ='N';
    }
  }
}

//Proceso para actualizar la matriz de salida desde el marco en tiempo real
void EscribirMatriz(int R, char (&Procesos)[5][30], char MarcoReal[5]){
    for(int n=0;n<5;n++){
      Procesos[n][R] = MarcoReal[n];
    }
}
//Contar cuantas veces ha aparecido un caracter en el request stream y regresar el numero de la pagina que debe ser modificada
int CharCounter(char MarcoReal[5], char Requests[30], int R){
  int NumChar[5] = {0, 0, 0, 0, 0};
  int Valor = 0;
  int n;
  //Se cicla primero a traves de MarcoReal. Se toma un valor de MarcoReal y se compara con el array Requests hasta que se encuentra la ultima vez que se referencio ese proceso.
  //El numero de ciclos que le tomo llegar hasta el proceso se guarda en NumChar, donde su posicion es respectica a la posicion de MarcoReal
  for(int i=0;i>5;i++){
    n = 0;
    while(MarcoReal[i] != Requests[n]){
     n++;}
    NumChar[i] = n;
  }
  for(int n=0;n<5;n++){//Encontrar posicion en el array NumChar con el mayor valor y regresar la posicion
        if(NumChar[n] > Valor)
          Valor = n;}
  return Valor;
}


void FIFO(char (&Procesos)[5][30], char Requests[30], double& CambiosPagina, double& Rendimiento, char (&Cambios)[30], bool& ShowProcess)
{
  char MarcoReal[5] = {'N','N','N','N','N'};//Marco de paginas real 
  int Marco[5] = {0, 0, 0, 0, 0};//Contador de edad por marco
  int MarcoMayor, Valor;//MarcoMayor: guarda el valor del marco de pagina con mayor edad, valor es usada para determinar esto
  int Columna = 0;
  int Fila = 0;
  int R = 0;//Variable reloj
  CambiosPagina = 0;//Reset
  Rendimiento = 0;//Reset
  bool ProcesoExiste = false;//booleano para evitar time race

  for(int n=0;n<30;n++){
  Cambios[n] =' ';}

  while(Columna < 30 and R < 30){
    if (Fila == 5){
      Fila = 0;
      Columna++;
      }
    if (ShowProcess){
    cout << R << endl;
    PrintMatriz(Procesos, Requests, CambiosPagina, Rendimiento, Cambios);}
    for(int t=0;t<5;t++){
      if(Requests[R] == MarcoReal[t] and R<30){//Si el proceso ya se encuentra dentro de los marcos de pagina se aumenta el tiempo que han pasado sin cambio y ProcesoExiste se enciende para evitar que las otras 2 funciones precedan el nuevo proceso
        EscribirMatriz(R, Procesos, MarcoReal);
        ProcesoExiste = true;
        Marco[0]++;
        Marco[1]++;
        Marco[2]++;
        Marco[3]++;
        Marco[4]++;
        R++;
      }
    }
    if(MarcoReal[Fila] == 'N' and ProcesoExiste == false){//si el proceso esta vacio (= N), se le asigna un proceso del array de requests
    MarcoReal[Fila] = Requests[R];
    EscribirMatriz(R, Procesos, MarcoReal);
      Fila++;
      Cambios[R] = '^';
      R++;
      CambiosPagina++;
      ProcesoExiste = true;
      if(Fila == 0){//se resetea el valor del marco de pagina que se utilizo y se incrementa el del resto
        Marco[0]=0;
        Marco[1]++;
        Marco[2]++;
        Marco[3]++;
        Marco[4]++;}
      else if (Fila == 1){
        Marco[0]++;
        Marco[1]=0;
        Marco[2]++;
        Marco[3]++;
        Marco[4]++;}
      else if (Fila == 2){
        Marco[0]++;
        Marco[1]++;
        Marco[2]=0;
        Marco[3]++;
        Marco[4]++;}
      else if (Fila == 3){
        Marco[0]++;
        Marco[1]++;
        Marco[2]++;
        Marco[3]=0;
        Marco[4]++;}
      else{
        Marco[0]++;
        Marco[1]++;
        Marco[2]++;
        Marco[3]++;
        Marco[4]=0;}
      }
    else if(ProcesoExiste == false){//si el marco esta ocupado se identifica el marco con mayor tiempo sin modificaciones en la variable "MarcoMayor"
      MarcoMayor = 0;
      Valor = Marco[0];
      for(int n=0;n<5;n++){//Encontrar pagina con mas tiempo sin cambio
        if(Marco[n] > Valor){
          Valor = Marco[n];
          MarcoMayor = n;}
      }
      MarcoReal[Fila] = Requests[R];//Escribir proceso en el marco con mas tiempo sin cambio
      EscribirMatriz(R, Procesos, MarcoReal);
      Fila++;
      CambiosPagina++;
      Cambios[R] = '^';
      R++;
      if(MarcoMayor == 0){
      Marco[0]=0;
      Marco[1]++;
      Marco[2]++;
      Marco[3]++;
      Marco[4]++;}
      if(MarcoMayor == 1){
      Marco[0]++;
      Marco[1]=0;
      Marco[2]++;
      Marco[3]++;
      Marco[4]++;}
      if(MarcoMayor == 2){
      Marco[0]++;
      Marco[1]++;
      Marco[2]=0;
      Marco[3]++;
      Marco[4]++;}
      if(MarcoMayor == 3){
      Marco[0]++;
      Marco[1]++;
      Marco[2]++;
      Marco[3]=0;
      Marco[4]++;}
      if(MarcoMayor == 4){
      Marco[0]++;
      Marco[1]++;
      Marco[2]++;
      Marco[3]++;
      Marco[4]=0;}
    }
    ProcesoExiste = false;
  }
  Rendimiento = (1 - (CambiosPagina/30))*100;
}

void LRU(char (&Procesos)[5][30], char Requests[30], double& CambiosPagina, double& Rendimiento, char (&Cambios)[30], bool& ShowProcess)
{
  char MarcoReal[5] = {'N','N','N','N','N'};//Marco de paginas real 
  int Marco[5] = {0, 0, 0, 0, 0};//Contador de edad por marco
  int MarcoMayor, Valor;//MarcoMayor: guarda el valor del marco de pagina con mayor edad, valor es usada para determinar esto
  int Columna = 0;
  int Fila = 0;
  int R = 0;//Variable reloj
  CambiosPagina = 0;//Reset
  Rendimiento = 0;//Reset
  bool ProcesoExiste = false;//booleano para evitar time race

  for(int n=0;n<30;n++){
  Cambios[n] =' ';}

  while(Columna < 30 and R < 30){
    if (Fila == 5){
      Fila = 0;
      Columna++;
      }
    if (ShowProcess){
    cout << R << endl;
    PrintMatriz(Procesos, Requests, CambiosPagina, Rendimiento, Cambios);}
    for(int t=0;t<5;t++){
      if(Requests[R] == MarcoReal[t] and R<30){//Si el proceso ya se encuentra dentro de los marcos de pagina se aumenta el tiempo que han pasado sin cambio y ProcesoExiste se enciende para evitar que las otras 2 funciones precedan el nuevo proceso
        EscribirMatriz(R, Procesos, MarcoReal);
        ProcesoExiste = true;
        R++;
      }
    }
    if(MarcoReal[Fila] == 'N' and ProcesoExiste == false){//si el proceso esta vacio (= N), se le asigna un proceso del array de requests
    MarcoReal[Fila] = Requests[R];
    EscribirMatriz(R, Procesos, MarcoReal);
      Fila++;
      Cambios[R] = '^';
      R++;
      CambiosPagina++;
      ProcesoExiste = true;}
    else if(ProcesoExiste == false){//si el marco esta ocupado se identifica el marco con mayor tiempo sin modificaciones en la variable "MarcoMayor"
      Fila = CharCounter(MarcoReal, Requests, R);
      MarcoReal[Fila] = Requests[R];//Escribir proceso en el marco con mas tiempo sin cambio
      EscribirMatriz(R, Procesos, MarcoReal);
      Fila++;
      CambiosPagina++;
      Cambios[R] = '^';
      R++;
    }
    ProcesoExiste = false;
  }
  Rendimiento = (1 - (CambiosPagina/30))*100;
}


int main() 
{
//Procedimientos de FIFO y LRU
//5 marcos de pagina
//10 procesos (A...J)
//Stream de 30 solicitudes(generado aleatoriamente)
//matriz bidimensional[5][30] para marcos de pagina
//1 array para el stream de solicitudes
//FIFO retira del marco de pagina el "proceso" con mas edad
//para su implementacion se usaran counters por marco de pagina que incrementan con
//cada reasignacion de proceso, se comparan los counters y el que tenga mayor valor se //retira el valor de su margo de pagina respectivo.
//LRU retira el proceso con mas edad que menos se haya referenciado. Hace esto comparando los valores
//en el marco real y buscandolos en el ya ocurrido array de requests.
//Ambos parecen generar un efecto parecido, pero son conceptualmente distintos.

//declaracion de variables
char Requests[30];
int OpcionMenu;
double CambiosPagina = 0;
double Rendimiento = 0;
char Procesos [5][30]; //5 filas, 30 columnas
char Cambios[30];//Array con fines esteticos
bool ShowProcess = false;

Request_gen(Requests);

  do
  {
    cout << "\n \n Administrador de procesos" << endl;
    cout << "\n \n Elija el algoritmo que utilizara para ejecutar las tareas:" << endl;
    cout << "1.FIFO" << endl;
    cout << "2.LRU" << endl;
    cout << "3.Nuevo stream de procesos" << endl;
    cout << "4.Mostrar pasos(toggle)" << endl;
    cout << "5.Salir" << endl;
    cout << "Teclea la opcion => ";
    cin >> OpcionMenu;

    switch(OpcionMenu)
    {
      case 1: FillNullProcesos(Procesos);
              FIFO(Procesos, Requests, CambiosPagina, Rendimiento, Cambios, ShowProcess);
              PrintMatriz(Procesos, Requests, CambiosPagina, Rendimiento, Cambios);
              cout << endl << endl;
              break;
      case 2: FillNullProcesos(Procesos);
			        LRU(Procesos, Requests, CambiosPagina, Rendimiento, Cambios, ShowProcess);
              PrintMatriz(Procesos, Requests, CambiosPagina, Rendimiento, Cambios);
              break;
      case 3: Request_gen(Requests);
              cout << endl << "Proceso | ";
              for(int f=0;f<30;f++){
                cout << Requests[f] << " | ";}
              cout << endl << endl;
              break;
      case 4: if (ShowProcess)
              ShowProcess = false;
              else
              ShowProcess = true;
    }
  }while(OpcionMenu != 5);

return 0;
}

