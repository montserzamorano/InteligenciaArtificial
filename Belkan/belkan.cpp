#include "belkan.h"
#include "conexion.h"
#include "environment.h"
#include <iostream>
#include <cstdlib>
#include <vector>
#include <utility>
#include <cstring>


using namespace std;

//-------------------------------------------------------------
void Agent::orientarMatriz(){
  cout << "Estoy aqui..." << endl;
  y_c = pk2.at(1).first;
  x_c = pk2.at(1).second;
  cout << "Y buena " << y_c << "X buena " << x_c << endl;
  int coord1_y = (pk1.at(0)).first - (pk2.at(0)).first;
  int coord1_x = (pk1.at(0)).second - (pk2.at(0)).second;
  int coord2_y = (pk1.at(1)).first - (pk2.at(1)).first;
  int coord2_x = (pk1.at(1)).second - (pk2.at(1)).second;
  pair <int,int> vector_direccion1(coord1_y,coord1_x); //vector direccion falso
  pair <int,int> vector_direccion2(coord2_y,coord2_x); //vector direccion real
  //bien orientada
  if(vector_direccion1.first == vector_direccion2.first &&
    vector_direccion1.second == vector_direccion2.second){
      orientacion_corregida = 0;
      int c1 = pk1.at(0).first - pk1.at(1).first;
      int c2 = pk1.at(0).second - pk1.at(1).second;
      for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
          mapa_solucion_[i][j]=mapa_entorno_[i+c1][j+c2];
        }
      }
  }
  //girar derecha
  else if(vector_direccion1.first == -vector_direccion2.second &&
    vector_direccion1.second == vector_direccion2.first){
      orientacion_corregida = 1; //MAGIA :D
      int c1 = pk1.at(0).first - pk1.at(1).second;
      int c2 = pk1.at(0).second + pk1.at(1).first;
      for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
          mapa_solucion_[i][j]=mapa_entorno_[j+c1][c2-i];
        }
      }
  }
  //180
  else if(vector_direccion1.first == -vector_direccion2.first &&
    vector_direccion1.second == -vector_direccion2.second){
      orientacion_corregida = 2;
      int c1 = pk1.at(0).first + pk1.at(1).first;
      int c2 = pk1.at(0).second + pk1.at(1).second;
      for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
          mapa_solucion_[i][j]=mapa_entorno_[c1-i][c2-j];
        }
      }
  }
  //giro izquierda
  else if(vector_direccion1.first == vector_direccion2.second &&
    vector_direccion1.second == -vector_direccion2.first){
      orientacion_corregida = 3; //MAGIA :D
      int c1 = pk1.at(0).first + pk1.at(1).second;
      int c2 = pk1.at(0).second - pk1.at(1).first;
      for(int i=0; i<100; i++){
        for(int j=0; j<100; j++){
          mapa_solucion_[i][j]=mapa_entorno_[c1-j][i+c2];
        }
      }
  }
}
//-------------------------------------------------------------

bool Agent::tengoZapatillas(){
  if(EN_USO_=='6'){
    return true;
  }
  return false;
}

bool Agent::tengoBikini(){
  if(EN_USO_=='0'){
    return true;
  }
  return false;
}


//-------------------------------------------------------------
bool Agent::esObstaculo(char c){
  if(c=='B' || c=='A' || c=='D' || c=='P' || c=='M'){
    if(tengoZapatillas() && c=='B'){
      return false;
    }
    else if(tengoBikini() && c=='A'){
      return false;
    }
    return true;
  }
  else{
    return false;
  }

}

bool Agent::esObstaculoMovil(char c){
  if(c>='a' && c<='z'){
    return true;
  }
  else{
    return false;
  }

}

bool Agent::esPersonaDeInteres(char c){
  if(c>='g' && c<='n'){
    return true;
  }
  else{
    return false;
  }

}
//-------------------------------------------------------------
bool Agent::esTransitable(pair <int,int> A,char m[200][200]){
  if(esObstaculo(m[A.first][A.second]) || esObstaculoMovil(mapa_objetos_[A.first][A.second])){
    return false;
  }
  else{
    return true;
  }
}

//-------------------------------------------------------------
bool Agent::enLista(pair <int,int> A, vector < pair <int,int> > lista){
  for(int i=0; i<lista.size(); i++){
    if(lista[i].first == A.first){
      if(lista[i].second== A.second){
        return true;
      }
    }
  }
  return false;
}

void Agent::setListaAbierta(vector < pair <int,int> > A, char m[200][200]){

  for(int i=0; i<A.size(); i++){
    if(!enLista(A.at(i),lista_abierta)&&!enLista(A.at(i),lista_cerrada)){
      if(esTransitable(A.at(i),m)){
        lista_abierta.push_back(A.at(i));
      }
    }
  }
  if(lista_abierta.empty()){
    lista_cerrada.clear();
    lista_cerrada.push_back(actual);
    //repetir
    for(int i=0; i<A.size(); i++){
      if(!enLista(A.at(i),lista_abierta)&&!enLista(A.at(i),lista_cerrada)){
        if(esTransitable(A.at(i),m)){
          lista_abierta.push_back(A.at(i));
        }
      }
    }
  }

}

//-------------------------------------------------------------
/*comprobamos las posiciones adyacentes al cuadro actual, empezando por arriba
a la izquierda y siguiendo hacia la derecha.
Si es desconocido, no se aniade a ninguna lista
Si es conocido y transitable, se aniade a la lista de cuadrados abiertos
Si es conocido y no transitable, se aniade a la lista de cuadrados cerrados*/
void Agent::setCuadrosAdyacentes(pair <int,int> A,char m[200][200]){
  vector < pair <int,int> > adyacentes;
  pair <int,int> c;
  c.first = A.first-1; //arriba
  c.second = A.second;
  adyacentes.push_back(c);
  c.first = A.first; //izquierda
  c.second = A.second-1;
  adyacentes.push_back(c);
  c.first = A.first+1;//abajo
  c.second = A.second;
  adyacentes.push_back(c);
  c.first = A.first; //derecha
  c.second = A.second+1;
  adyacentes.push_back(c);
  setListaAbierta(adyacentes,m);
}
//------------------------------------------------------------

int Agent::distanciaManhattan(pair <int,int> c1, pair <int,int> c2){
  return 10*(abs(c1.first-c2.first)+abs(c1.second-c2.second));
}

int Agent::buscaPK(){
  for(int i=1; i<10; i++){
    if(VISTA_[i]=='K'){
      return i; //devuelve el pk mas cercano
    }
  }
  return -1; //si no hay pks, devuelve -1
}

//------------------------------------------------------------
pair <int,int> Agent::devuelveCoordenadas(int pos_vista){
  pair <int,int> objetivo;
  switch (orientacion_) {
    case 0:
      if(pos_vista==0){
        objetivo.first=y_;
        objetivo.second=x_;
      }
      else if(pos_vista==1){
        objetivo.first=y_-1;
        objetivo.second=x_;
      }
      else if(pos_vista==2){
        objetivo.first=y_-2;
        objetivo.second=x_-1;
      }
      else if(pos_vista==3){
        objetivo.first=y_-2;
        objetivo.second=x_;
      }
      else if(pos_vista==4){
        objetivo.first=y_-2;
        objetivo.second=x_+1;
      }
      else if(pos_vista==5){
        objetivo.first=y_-3;
        objetivo.second=x_-2;
      }
      else if(pos_vista==6){
        objetivo.first=y_-3;
        objetivo.second=x_-1;
      }
      else if(pos_vista==7){
        objetivo.first=y_-3;
        objetivo.second=x_;
      }
      else if(pos_vista==8){
        objetivo.first=y_-3;
        objetivo.second=x_+1;
      }
      else if(pos_vista==9){
        objetivo.first=y_-3;
        objetivo.second=x_+2;
      }

      break;
    case 1:
      if(pos_vista==0){
        objetivo.first=y_;
        objetivo.second=x_;
      }
      else if(pos_vista==1){
        objetivo.first=y_;
        objetivo.second=x_+1;
      }
      else if(pos_vista==2){
        objetivo.first=y_-1;
        objetivo.second=x_+2;
      }
      else if(pos_vista==3){
        objetivo.first=y_;
        objetivo.second=x_+2;
      }
      else if(pos_vista==4){
        objetivo.first=y_+1;
        objetivo.second=x_+2;
      }
      else if(pos_vista==5){
        objetivo.first=y_-2;
        objetivo.second=x_+3;
      }
      else if(pos_vista==6){
        objetivo.first=y_-1;
        objetivo.second=x_+3;
      }
      else if(pos_vista==7){
        objetivo.first=y_;
        objetivo.second=x_+3;
      }
      else if(pos_vista==8){
        objetivo.first=y_+1;
        objetivo.second=x_+3;
      }
      else if(pos_vista==9){
        objetivo.first=y_+2;
        objetivo.second=x_+3;
      }
      break;
    case 2:
      if(pos_vista==0){
        objetivo.first=y_;
        objetivo.second=x_;
      }
      else if(pos_vista==1){
        objetivo.first=y_+1;
        objetivo.second=x_;
      }
      else if(pos_vista==2){
        objetivo.first=y_+2;
        objetivo.second=x_+1;
      }
      else if(pos_vista==3){
        objetivo.first=y_+2;
        objetivo.second=x_;
      }
      else if(pos_vista==4){
        objetivo.first=y_+2;
        objetivo.second=x_-1;
      }
      else if(pos_vista==5){
        objetivo.first=y_+3;
        objetivo.second=x_+2;
      }
      else if(pos_vista==6){
        objetivo.first=y_+3;
        objetivo.second=x_+1;
      }
      else if(pos_vista==7){
        objetivo.first=y_+3;
        objetivo.second=x_;
      }
      else if(pos_vista==8){
        objetivo.first=y_+3;
        objetivo.second=x_-1;
      }
      else if(pos_vista==9){
        objetivo.first=y_+3;
        objetivo.second=x_-2;
      }
      break;
    case 3:
      if(pos_vista==0){
        objetivo.first=y_;
        objetivo.second=x_;
      }
      else if(pos_vista==1){
        objetivo.first=y_;
        objetivo.second=x_-1;
      }
      else if(pos_vista==2){
        objetivo.first=y_+1;
        objetivo.second=x_-2;
      }
      else if(pos_vista==3){
        objetivo.first=y_;
        objetivo.second=x_-2;
      }
      else if(pos_vista==4){
        objetivo.first=y_-1;
        objetivo.second=x_-2;
      }
      else if(pos_vista==5){
        objetivo.first=y_+2;
        objetivo.second=x_-3;
      }
      else if(pos_vista==6){
        objetivo.first=y_+1;
        objetivo.second=x_-3;
      }
      else if(pos_vista==7){
        objetivo.first=y_;
        objetivo.second=x_-3;
      }
      else if(pos_vista==8){
        objetivo.first=y_-1;
        objetivo.second=x_-3;
      }
      else if(pos_vista==9){
        objetivo.first=y_-2;
        objetivo.second=x_-3;
      }
      break;
  }
  return objetivo;
}
//------------------------------------------------------------

int Agent::buscar(char c){
  for(int i=0; i<10; i++){
    if(SURFACE_[i]==c){
      return i;
    }
  }
  return -1;
}

int Agent::buscarObjetoQueSirva(){
  for(int i=0; i<10; i++){
    if(SURFACE_[i]=='4' ||SURFACE_[i]=='5' || SURFACE_[i]=='9' || SURFACE_[i]=='8'){
      return i;
    }
  }
  return -1;
}

bool Agent::recogerObjeto(pair <int,int> unCuadro){
  if(hayObjeto(unCuadro) && cuantosEnMochila() < 3 ||
  mapa_entorno_[unCuadro.first][unCuadro.second] == '6'
  || mapa_entorno_[unCuadro.first][unCuadro.second] == '0'){
    return true;
  }
  return false;
}


bool Agent::hayObjeto(pair <int,int> unCuadro){
  if(SURFACE_[0]=='4' ||SURFACE_[0]=='5' || SURFACE_[0]=='9' || SURFACE_[0]=='8' || SURFACE_[0]=='0' || SURFACE_[0]=='6')
      return true;
  return false;
}

int Agent::cuantosEnMochila(){
  int contador = 0;
  for(int i=0; i<4;i++){
    if(MOCHILLA_[i]!='-'){
      contador++;
    }
  }
  return contador;
}


vector <Agent::ActionType> Agent::guardarEnMochila(){
  cout << "Voy a guardarlo en la mochila" << endl;
  vector <Agent::ActionType> ruta;
  /*int objetos_mochila = cuantosEnMochila();
  cout << "Tengo " << objetos_mochila << endl;
  vector <Agent::ActionType> ruta;
  if(objetos_mochila==3){
    if(EN_USO_ != '-'){
      cout << "Hay 3 objetos y 1 en uso" << endl;
      ruta.pus_back(actPUSH);
      ruta.push_back(actPOP);
      vector <Agent::ActionType> ruta1(soltarObjeto());
    }
    else{
      cout << "Hay 3 objetos y ninguno en uso" << endl;
      ruta.push_back(actPUSH);
      ruta.push_back(actPOP);
      ruta.push_back(actPUTDOWN);
      ruta.push_back(actPOP);
      objetos_mochila = cuantosEnMochila();
      for(int i=0; i<objetos_mochila;i++){
        ruta.push_back(actPUSH);
        ruta.push_back(actPOP);
      }
    }
  }
  if(EN_USO_ != '-'){
    cout << "Ahora mismo tengo algo en uso" << endl;
    ruta.push_back(actPUSH); //guarda en la mochila lo que tengas en uso
    objetos_mochila = cuantosEnMochila();
    ruta.push_back(actPICKUP);
    for(int i=0; i<objetos_mochila;i++){
      ruta.push_back(actPUSH);
      ruta.push_back(actPOP);
    }
  }
  else{
    cout << "No tengo nada en uso" << endl;
    ruta.push_back(actPICKUP);
    ruta.push_back(actPUSH);
  }*/
  if(EN_USO_=='-'){
    char recoger = SURFACE_[0];
    if(recoger!='0' && recoger!='1' && recoger!='3' && recoger!='7' && cuantosEnMochila()<3){
      ruta.push_back(actPICKUP);
      ruta.push_back(actPUSH);
    }
  }
  else{
    if(EN_USO_=='0' && cuantosEnMochila()<3){
      objetos_mochila = cuantosEnMochila();
      ruta.push_back(actPUSH); //guarda en la mochila lo que tengas en uso
      ruta.push_back(actPICKUP);
      ruta.push_back(actPUSH);
      for(int i=0; i<objetos_mochila;i++){
        ruta.push_back(actPOP);
        ruta.push_back(actPUSH);
      }
      ruta.push_back(actPOP);
    }
    else if(EN_USO_=='6' && cuantosEnMochila()<3){
      objetos_mochila = cuantosEnMochila();
      ruta.push_back(actPUSH); //guarda en la mochila lo que tengas en uso
      ruta.push_back(actPICKUP);
      ruta.push_back(actPUSH);
      for(int i=0; i<objetos_mochila;i++){
        ruta.push_back(actPOP);
        ruta.push_back(actPUSH);
      }
      ruta.push_back(actPOP);
    }
  }

  cout << "Ya esta guardado" << endl;
  return ruta;
}

int Agent::enMochila(char c){
  cout << "esta en mochila" << endl;
  int objetos_mochila = cuantosEnMochila();
  for(int i=0; i<objetos_mochila;i++){
    if(MOCHILLA_[i]==c)
      return i;
  }
  return -1;
}

vector <Agent::ActionType> Agent::cogerDeMochila(int pos){
  vector <Agent::ActionType> ruta;
  cout << "Coger de mochila" << endl;
  for(int i=0; i<=pos+1;i++){
    ruta.push_back(actPUSH);
    ruta.push_back(actPOP);
  }
  return ruta;
}

bool Agent::hayObjetoEn(int pos){
  return (SURFACE_[pos]>='0' && SURFACE_[pos]<='9');
}


vector <Agent::ActionType> Agent::soltarObjeto(){
  vector <Agent::ActionType> ruta;
  if(!esObstaculo(VISTA_[1]) && !esObstaculoMovil(SURFACE_[1]) && !hayObjetoEn(1)){
    ruta.push_back(actFORWARD);
    ruta.push_back(actPUTDOWN);
    ruta.push_back(actTURN_R);
    ruta.push_back(actTURN_R);
    ruta.push_back(actFORWARD);
    ruta.push_back(actPICKUP);
  }
  else{
    ruta.push_back(actTURN_L);
  }
  return ruta;
}

int Agent::hayPersona(){
  if(cuantosEnMochila()>0){
    for(int i=0; i<10; i++){
      if(SURFACE_[i]>='g' && SURFACE_[i]<='n'){
        return i;
      }
    }
  }
  return -1;
}

//------------------------------------------------------------

pair <int,int> Agent::generarCuadroObjetivo(){
  pair <int,int> objetivo(n1,n2);
  pair <int,int> objetivoPasado(n1,n2);
  //si hay un pk a la vista y no esta yendo hacia otro
  if(buscaPK()!=-1 && !pkencontrado && !orientada){ //si no habiamos encontrado un pk y hay uno a la vista... a por el
    cout << "Un PK!!" << endl;
    if(!enLista(devuelveCoordenadas(buscaPK()),lista_pks)){
      objetivo = devuelveCoordenadas(buscaPK());
      n1 = objetivo.first;
      n2 = objetivo.second;
      cout << "PK no visitado en " << objetivo.first << " " << objetivo.second << endl;
      pkencontrado = true;
      objetoEncontrado = false;
      personaEncontrada = false;
      random = false;
      contador_paso++;
    }
    else{//si lo habiamos visitado, random
      cout << "El PK ya ha sido visitado" << endl;
      pkencontrado = false;
      cout << "Objetivo random" << endl;
      n1=rand()%200;
      n2=rand()%200;
      objetivo.first = n1;
      objetivo.second = n2;
      while(mapa_entorno_[n1][n2]!='?'){
        n1=rand()%200;
        n2=rand()%200;
        objetivo.first = n1;
        objetivo.second = n2;
        contador_paso=0;
        random = true;
      }
    }
  }
  else if(pkencontrado && !orientada){ //si la matriz no esta orientada y habiamos encontrado un pk, vamos a por el
    cout << "Mantengo mi objetivo... a por el PK!" << endl;
    contador_paso++;
  }
  else if(hayPersona()!=-1 && !personaEncontrada){
    cout << "Eso parece que es un princeso... a por el!!!!!" << endl;
    objetivo = devuelveCoordenadas(hayPersona());
    n1 = objetivo.first;
    n2 = objetivo.second;
    personaEncontrada = true;
    random = false;
    contador_paso++;
  }
  else if(personaEncontrada){
    contador_paso++;
    cout << "Mantengo mi objetivo... a por el princeso!!!" << endl;
  }
  else if(!objetoEncontrado && (cuantosEnMochila()<3 || EN_USO_!='6')){ //si no habiamos encontrado un objeto
    if(buscar('6')!=-1){ //en primer lugar ir a por las zapatillas
      cout << "A por las zapatillas!!!!!" << endl;
      objetivo = devuelveCoordenadas(buscar('6'));
      n1 = objetivo.first;
      n2 = objetivo.second;
      objetoEncontrado = true;
      contador_paso++;
      random = false;
    }
    else if(buscar('0')!=-1){ //si no, ir a por el bikini
      cout << "A por el bikini!!!!!" << endl;
      objetivo = devuelveCoordenadas(buscar('0'));
      n1 = objetivo.first;
      n2 = objetivo.second;
      objetoEncontrado = true;
      contador_paso++;
      random = false;
    }
    else if(buscarObjetoQueSirva()!=-1 && cuantosEnMochila()<3){//el objeto mas cercano que sirva
      cout << "Eso parece que sirve... a por el!!!!!" << endl;
      objetivo = devuelveCoordenadas(buscarObjetoQueSirva());
      n1 = objetivo.first;
      n2 = objetivo.second;
      objetoEncontrado = true;
      contador_paso++;
      random = false;
    }
    else if(random){
      cout << "Mantengo mi objetivo random" << endl;
      contador_paso++;
    }
    else{
      cout << "Objetivo random" << endl;
      n1=rand()%200;
      n2=rand()%200;
      objetivo.first = n1;
      objetivo.second = n2;
      while(mapa_entorno_[n1][n2]!='?'){
        n1=rand()%200;
        n2=rand()%200;
        objetivo.first = n1;
        objetivo.second = n2;
      }
      contador_paso=0;
      random = true;
    }
  }
  else if(objetoEncontrado){//si todavia no hemos llegado al objeto, a por el
    cout << "Mantengo mi objetivo... a por el objeto!!!" << endl;
    contador_paso++;
  }
  else if(random){
    cout << "Mantengo mi objetivo... a por el objetivo random!!!" << endl;
    contador_paso++;
  }
  else{
    cout << "Objetivo random" << endl;
    n1=rand()%200;
    n2=rand()%200;
    objetivo.first = n1;
    objetivo.second = n2;
    while(mapa_entorno_[n1][n2]!='?'){
      n1=rand()%200;
      n2=rand()%200;
      objetivo.first = n1;
      objetivo.second = n2;
    }
    contador_paso=0;
    objetoEncontrado=false;
    personaEncontrada=false;
    pkencontrado=false;
    random = true;
  }

  if(contador_paso>=75){
      cout << "Numero de pasos superado. Objetivo random" << endl;
      n1=rand()%200;
      n2=rand()%200;
      objetivo.first = n1;
      objetivo.second = n2;
      while(mapa_entorno_[n1][n2]!='?'){
        n1=rand()%200;
        n2=rand()%200;
        objetivo.first = n1;
        objetivo.second = n2;
      }
      contador_paso=0;
      random = true;
  }

  cout << "Ahora mi cuadro objetivo es: " << n1 << " " << n2 << endl;
  return objetivo;
}

//------------------------------------------------------------
//devuelve el cuadrado que nos permite avanzar al cuadro a menor distancia del objetivo
pair <int,int> Agent::funcionObjetivo(pair <int,int> unCuadro){
  int min = 1000000000;
  int indice=-1;
  for(int i=0; i<lista_abierta.size();i++){
    if(distanciaManhattan(lista_abierta.at(i),unCuadro)<min){
      min=distanciaManhattan(lista_abierta.at(i),unCuadro);
      indice = i;
    }
  }
  return lista_abierta.at(indice);
}

//------------------------------------------------------------

vector <Agent::ActionType> Agent::hallarRuta(pair <int,int> c1,pair <int,int> c2){
  vector <Agent::ActionType> ruta;

  int brujula = orientacion_;

  if(c1.first == (c2.first+1) && c1.second==c2.second){
    if(brujula == 0){
      ruta.push_back(actFORWARD);
    }
    else if(brujula == 1){
      ruta.push_back(actTURN_L);
      ruta.push_back(actFORWARD);
    }
    else if(brujula == 2){
      ruta.push_back(actTURN_L);
      ruta.push_back(actTURN_L);
      ruta.push_back(actFORWARD);
    }
    else{
      ruta.push_back(actTURN_R);
      ruta.push_back(actFORWARD);
    }
  }//arriba
  else if(c1.first == c2.first && c1.second==(c2.second+1)){
    if(brujula == 0){
      ruta.push_back(actTURN_L);
      ruta.push_back(actFORWARD);
    }
    else if(brujula == 1){
      ruta.push_back(actTURN_L);
      ruta.push_back(actTURN_L);
      ruta.push_back(actFORWARD);
    }
    else if(brujula == 2){
      ruta.push_back(actTURN_R);
      ruta.push_back(actFORWARD);
    }
    else{
      ruta.push_back(actFORWARD);
    }
  }//izquierda
  else if(c1.first == c2.first && c1.second==(c2.second-1)){
    if(brujula == 0){
      ruta.push_back(actTURN_R);
      ruta.push_back(actFORWARD);
    }
    else if(brujula == 1){
      ruta.push_back(actFORWARD);
    }
    else if(brujula == 2){
      ruta.push_back(actTURN_L);
      ruta.push_back(actFORWARD);
    }
    else{
      ruta.push_back(actTURN_L);
      ruta.push_back(actTURN_L);
      ruta.push_back(actFORWARD);
    }
  }//derecha
  else if (c1.first == (c2.first-1)  && c1.second == c2.second){
    if(brujula == 0){
      ruta.push_back(actTURN_L);
      ruta.push_back(actTURN_L);
      ruta.push_back(actFORWARD);
    }
    else if(brujula == 1){
      ruta.push_back(actTURN_R);
      ruta.push_back(actFORWARD);
    }
    else if(brujula == 2){
      ruta.push_back(actFORWARD);
    }
    else{
      ruta.push_back(actTURN_L);
      ruta.push_back(actFORWARD);
    }
  }//abajo
  return ruta;
}

vector <Agent::ActionType> Agent::regalarCositas(){
    cout << "Voy a regalarte mis cositas" << endl;
    vector <Agent::ActionType> ruta;
    bool tengoEnUso = false;
    if(EN_USO_ != '-'){
      tengoEnUso = true;
      ruta.push_back(actPUSH);
    }
    int num_mochila = cuantosEnMochila();
    for(int i=0; i<num_mochila; i++){
      ruta.push_back(actPOP);
      ruta.push_back(actGIVE);
    }
    if(tengoEnUso){
      ruta.push_back(actPOP);
    }
    return ruta;
}

//------------------------------------------------------------
vector <Agent::ActionType> Agent::AlgoritmoEstrella(char m[200][200]){
  vector <Agent::ActionType> ruta;
  if(SURFACE_[0]=='6'){ //soltar lo que tenga y coger zapatillas
    ruta.clear();
    if(EN_USO_!='-'){
      ruta = soltarObjeto();

      if(ruta.size()>1){
        ruta.push_back(actPICKUP);
        objetoEncontrado = false;
      }
    }
    else{
      ruta.push_back(actPICKUP);
      objetoEncontrado = false;
    }
  }
  else if(EN_USO_=='-' && SURFACE_[0]=='0'){ //si no tengo nada y el bikini esta en el suelo, cogerlo
    ruta.clear();
    ruta.push_back(actPICKUP);
    objetoEncontrado = false;
  }
  else if(recogerObjeto(actual)){//si hay un objeto y no tengo nada, coger
    ruta.clear();
    ruta = guardarEnMochila();
    objetoEncontrado = false;
  }
  else{//si no hay nada que hacer con los objetos
    setCuadrosAdyacentes(actual,m);
    //buscamos el cuadro al que avanzar

  pair <int,int> cuadro_objetivo = generarCuadroObjetivo();
  pair <int,int> cuadro_avanzar = funcionObjetivo(cuadro_objetivo);

  /*if(random && esTransitable(devuelveCoordenadas(VISTA_[1]),mapa_entorno_)){
    ruta.clear();
    ruta.push_back(actFORWARD);
    cout << "No tengo destino y puedo ir para delante" << endl;
  }
  else{*/
    ruta = hallarRuta(actual,cuadro_avanzar);

      if(cuadro_avanzar == cuadro_objetivo){
        if(pkencontrado==true){
          lista_pks.push_back(cuadro_objetivo);
          pkencontrado=false;
          contador_paso=0;
        }
        else if(objetoEncontrado==true){
          objetoEncontrado = false;
          contador_paso=0;
        }
        else{
          random = false;
        }
      }
      else if(esPersonaDeInteres(SURFACE_[1]) && cuantosEnMochila()>0){
        cout << "Tengo al princeso delante!" << endl;
        ruta.clear();
        ruta = regalarCositas();
        personaEncontrada = false;
        contador_paso = 0;
      }
      lista_abierta.clear(); //vaciar la lista para la siguiente iteracion
      lista_cerrada.push_back(cuadro_avanzar);
    }
  //}

  return ruta;
}

// -----------------------------------------------------------
void PasarVectoraMapaCaracteres(int fila, int columna, char m[200][200],
  char *v, int brujula){
  m[fila][columna]=v[0];

    switch(brujula){
        case 0: // Orientacion Norte

		m[fila-1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila-2][columna+i-1]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila-3][columna+j-2]=v[5+j];
		}
	        break;
	case 1: // Orientacion Este
		m[fila][columna+1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+i-1][columna+2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+j-2][columna+3]=v[5+j];
		}
	        break;
        case 2: // Orientacion Sur
		m[fila+1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+2][columna+1-i]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+3][columna+2-j]=v[5+j];
		}
		                break;
        case 3: // Orientacion Oeste
		m[fila][columna-1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+1-i][columna-2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+2-j][columna-3]=v[5+j];
		}

                break;
    }

}

void PasarVectoraMapaSolucion(int fila, int columna, char m[100][100],
  char *v, int brujula){
  m[fila][columna]=v[0];

    switch(brujula){
        case 0: // Orientacion Norte

		m[fila-1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila-2][columna+i-1]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila-3][columna+j-2]=v[5+j];
		}
	        break;
	case 1: // Orientacion Este
		m[fila][columna+1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+i-1][columna+2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+j-2][columna+3]=v[5+j];
		}
	        break;
        case 2: // Orientacion Sur
		m[fila+1][columna]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+2][columna+1-i]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+3][columna+2-j]=v[5+j];
		}
		                break;
        case 3: // Orientacion Oeste
		m[fila][columna-1]=v[1];
		for (int i=0; i<3; i++){
		  m[fila+1-i][columna-2]=v[2+i];
		}
		for (int j=0; j<5; j++){
		  m[fila+2-j][columna-3]=v[5+j];
		}

                break;
    }

}

// -----------------------------------------------------------
void Agent::Perceive(Environment &env)
{
	env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_, MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, false);

}


bool Agent::Perceive_Remote(conexion_client &Cliente, Environment &env)
{
	bool actualizado=false;


	actualizado = env.Perceive_Remote(Cliente);
	if (actualizado)
		env.SenSorStatus(VISTA_, SURFACE_, MENSAJE_, REINICIADO_, EN_USO_, MOCHILLA_, PUNTUACION_, FIN_JUEGO_, SALUD_, true);

    return actualizado;
}


// -----------------------------------------------------------
string ActionStr(Agent::ActionType accion)
{
	switch (accion)
	{
	case Agent::actFORWARD: return "FORWARD";
	case Agent::actTURN_L: return "TURN LEFT";
	case Agent::actTURN_R: return "TURN RIGHT";
	case Agent::actIDLE: return "IDLE";
	case Agent::actPICKUP: return "PICK UP";
	case Agent::actPUTDOWN: return "PUT DOWN";
	case Agent::actPUSH: return "PUSH";
	case Agent::actPOP: return "POP";
	case Agent::actGIVE: return "GIVE";
	case Agent::actTHROW: return "THROW";
	default: return "????";
	}
}

// -----------------------------------------------------------
void Agent::ActualizarInformacion(Environment *env){
	// Actualizar mi informacion interna
	if (REINICIADO_){
		// Lo que tengas que hacer si eres reposicionado en el juego
    for(int i=0; i<200; i++){
      for(int j=0; j<200; j++){
        mapa_entorno_[i][j]='?';
        mapa_objetos_[i][j]='?';
      }
      x_=99;
      y_=99;
      x_c=0;
      y_c=0;
      contador_paso=0;
      orientacion_=3;
      orientada = false;
      lista_abierta.clear();
      lista_cerrada.clear();
      lista_pks.clear();
      contador_npk=0;
      pkencontrado = false;
      objetoEncontrado = false;
      personaEncontrada = false;
      orientacion_corregida=-1;
      random = false;
      pk1.clear();
      pk2.clear();
    }
	}

	switch(last_accion_){
	  case 0: //avanzar
	  switch(orientacion_){
	    case 0: // norte
		    y_--;
		    break;
	    case 1: // este
		    x_++;
		    break;
	    case 2: // sur
		    y_++;
		    break;
	    case 3: // este
		    x_--;
		    break;
	  }

    //MAGIC :D
      if ((orientacion_+orientacion_corregida)%4 == 0){
        y_c--;
      }
      else if((orientacion_+orientacion_corregida)%4 == 1){
        x_c++;
      }
      else if((orientacion_+orientacion_corregida)%4 == 2){
        y_c++;
      }
      else if((orientacion_+orientacion_corregida)%4 == 3){
        x_c--;
      }


	  break;
	  case 1: // girar izq
		  orientacion_=(orientacion_+3)%4;
		  break;
	  case 2: // girar dch
		  orientacion_=(orientacion_+1)%4;
		  break;
	}

	// Comprobacion para no salirme del rango del mapa
	bool algo_va_mal=false;
	if (y_<0){
		y_=0;
		algo_va_mal=true;
	}
	else if (y_>199){
		y_=199;
		algo_va_mal=true;
	}
	if (x_<0){
		x_=0;
		algo_va_mal=true;
	}
	else if (x_>199){
		x_=199;
		algo_va_mal=true;
	}

	if (algo_va_mal){
		cout << "CUIDADO: NO ESTAS CONTROLANDO BIEN LA UBICACION DE TU AGENTE\n";
	}


	PasarVectoraMapaCaracteres(y_,x_,mapa_entorno_,VISTA_,orientacion_);
	PasarVectoraMapaCaracteres(y_,x_,mapa_objetos_,SURFACE_,orientacion_);

	env->ActualizarMatrizUsuario(mapa_entorno_);

  if(VISTA_[0]=='K'){
    if(contador_npk==0){
      contador_npk++;
      pair <int,int> falso(y_,x_);
      pk1.push_back(falso);
      int py, px;
      sscanf(MENSAJE_.c_str(),"%*[^:]: %i columna: %i",&py, &px);
      pair <int,int> verdadero(py,px);
      pk1.push_back(verdadero);
    }
    else if(contador_npk==1){
        if (y_ != pk1.at(0).first && x_ != pk1.at(0).second){
          pair <int,int> falso(y_,x_);
          pk2.push_back(falso);
          int py, px;
          sscanf(MENSAJE_.c_str(),"%*[^:]: %i columna: %i",&py, &px);
          pair <int,int> verdadero(py,px);
          pk2.push_back(verdadero);
          orientarMatriz();
          contador_npk++;
          orientada = true;
      }
    }
  }
  cout << y_c << " " << x_c << " " << y_ << " " << x_ << " " << (orientacion_+orientacion_corregida)%4 << endl;
  if(orientada){//si ya se ha orientado la matriz, copiar la vista en el mapa solucion
    PasarVectoraMapaSolucion(y_c,x_c,mapa_solucion_,VISTA_,(orientacion_+orientacion_corregida)%4);
  }

}

// ---------------------------MODIFICADAS Y ANIADIDAS--------------------------

Agent::ActionType Agent::Think()
{
  actual.first=y_;
  actual.second=x_;

  //if(!orientada){ //si no esta orientada, utilizamos el mapa entorno
  //}

  if(ruta.empty()){
    ruta = AlgoritmoEstrella(mapa_entorno_);
  }

  ActionType accion = ruta.at(0);

  if (accion == actFORWARD){
    if (!esObstaculo(VISTA_[1]) && !esObstaculoMovil(SURFACE_[1])){
      ruta.erase(ruta.begin());
    }
    else{
      ruta.clear();
      ruta = AlgoritmoEstrella(mapa_entorno_);
      accion = ruta.at(0);
      ruta.erase(ruta.begin());
    }
  }
  else{
    ruta.erase(ruta.begin());
  }

  last_accion_ = accion;

	return accion;

}
