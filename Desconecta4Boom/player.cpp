#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"

using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;

// Constructor
Player::Player(int jug){
    jugador_=jug;
    primer_turno=true;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
       return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
            return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
            return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
          return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar



/*
->Factores a tener en cuenta en la evaluacion:
1. se utilizara una heuristica combinada de ataque y defensa:
El objetivo sera tanto que no hagamos 4 en raya y provocar que el otro lo haga
2. el que ocupe las casillas centrales tiene mas posibilidades de hacer
los 4 en raya
3. habra que tener en cuenta el numero de casillas alineadas en cada momento

*/

double Player::evaluacionJugador(const Environment &estado, int jugador){
  double puntos=0;
  int bomba = 5;
  if(jugador==1)
    bomba = 4;

  //horizontales
  for(int i=0; i<7;i++){//recorremos por filas
    int seguidas = 0;
    bool haySeguidas = false;
    for(int j=0; j<7; j++){
      if(estado.See_Casilla(i,j)==jugador || estado.See_Casilla(i,j)==bomba){
        haySeguidas=true;
        seguidas++;
        if(seguidas==4){
          puntos +=10000;
        }
        else if(seguidas>=2){
          puntos+=seguidas;
        }
      }
      else{
        if(haySeguidas && estado.See_Casilla(i,j)==0){
          puntos+=seguidas;
        }
        haySeguidas=false;
        seguidas = 0;
      }
    }
  }

  //recorrido vertical
  for(int j=0; j<7;j++){//recorremos por filas
    int seguidas = 0;
    bool haySeguidas = false;
    for(int i=0; i<7; i++){
      if(estado.See_Casilla(i,j)==jugador || estado.See_Casilla(i,j)==bomba){
        haySeguidas=true;
        seguidas++;
        if(seguidas==4){
          puntos += 10000;
        }
        else if(seguidas>=2){
          puntos+=seguidas;
        }
      }
      else{
        if(haySeguidas && estado.See_Casilla(i,j)==0){//si habia seguidas y la siguiente esta descubierta
          puntos+=seguidas;
        }
        haySeguidas=false;
        seguidas = 0;
      }
    }
  }

  //recorrido diagonal ascendente derecha
  for(int i=0; i<6;i++){
    for(int j=0; j<6;j++){
      if(estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j+1)==jugador ||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j+1)==bomba ||
      estado.See_Casilla(i,j)==bomba && estado.See_Casilla(i+1,j+1)==jugador){
        puntos+=2;
      }
    }
  }
  //recorrido diagonal ascendente izquierda
  for(int i=0; i<6;i++){
    for(int j=1; j<7;j++){
      if(estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j-1)==jugador ||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j-1)==bomba ||
      estado.See_Casilla(i,j)==bomba && estado.See_Casilla(i+1,j-1)==jugador){
        puntos+=2;
      }
    }
  }
  //y si hay de 3
  for(int i=0; i<5;i++){
    for(int j=0; j<5;j++){
      if(estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j+1)==jugador && estado.See_Casilla(i+2,j+2)==jugador ||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j+1)==bomba  && estado.See_Casilla(i+2,j+2)==jugador ||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j+1)==jugador && estado.See_Casilla(i+2,j+2)==bomba ||
      estado.See_Casilla(i,j)==bomba && estado.See_Casilla(i+1,j+1)==jugador  && estado.See_Casilla(i+2,j+2)==jugador){
        puntos+=3;
      }
    }
  }
  //recorrido diagonal ascendente por la izquierda
  for(int i=0; i<5;i++){
    for(int j=2; j<7;j++){
      if(estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j-1)==jugador && estado.See_Casilla(i+2,j-2)==jugador||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j-1)==jugador && estado.See_Casilla(i+2,j-2)==bomba||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j-1)==bomba && estado.See_Casilla(i+2,j-2)==jugador||
      estado.See_Casilla(i,j)==bomba && estado.See_Casilla(i+1,j-1)==jugador && estado.See_Casilla(i+2,j-2)==jugador ){
        puntos+=3;
      }
    }
  }
  //lineas de 4
  //derecha
  for(int i=0; i<4;i++){
    for(int j=0; j<4;j++){
      if(estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j+1)==jugador && estado.See_Casilla(i+2,j+2)==jugador && estado.See_Casilla(i+3,j+3)==jugador||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j+1)==jugador && estado.See_Casilla(i+2,j+2)==jugador && estado.See_Casilla(i+3,j+3)==bomba||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j+1)==jugador && estado.See_Casilla(i+2,j+2)==bomba && estado.See_Casilla(i+3,j+3)==jugador||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j+1)==bomba  && estado.See_Casilla(i+2,j+2)==jugador && estado.See_Casilla(i+3,j+3)==jugador ||
      estado.See_Casilla(i,j)==bomba && estado.See_Casilla(i+1,j+1)==jugador  && estado.See_Casilla(i+2,j+2)==jugador && estado.See_Casilla(i+3,j+3)==jugador){
        puntos+=1000;
      }
    }
  }

  for(int i=0; i<4;i++){
    for(int j=3; j<7;j++){
      if(estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j-1)==jugador && estado.See_Casilla(i+2,j-2)==jugador && estado.See_Casilla(i+3,j-3)==jugador||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j-1)==jugador && estado.See_Casilla(i+2,j-2)==jugador && estado.See_Casilla(i+3,j-3)==bomba||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j-1)==jugador && estado.See_Casilla(i+2,j-2)==bomba && estado.See_Casilla(i+3,j-3)==jugador||
      estado.See_Casilla(i,j)==jugador && estado.See_Casilla(i+1,j-1)==bomba  && estado.See_Casilla(i+2,j-2)==jugador && estado.See_Casilla(i+3,j-3)==jugador ||
      estado.See_Casilla(i,j)==bomba && estado.See_Casilla(i+1,j-1)==jugador  && estado.See_Casilla(i+2,j-2)==jugador && estado.See_Casilla(i+3,j-3)==jugador){
        puntos+=1000;
      }
    }
  }



  return puntos;
}

double Player::evaluacion(const Environment &estado, int jugador){
  int adversario = 1;
  if(jugador == 1)
    adversario = 2;

    double eval1 = evaluacionJugador(estado,jugador);
    double eval2 = evaluacionJugador(estado,adversario);

    return eval2-eval1;
}

// Funcion heuristica (ESTA ES LA QUE TENEIS QUE MODIFICAR)
double Player::Valoracion(const Environment &estado, int jugador){
//en otro caso, el juego sigue y hay que hallar
    return evaluacion(estado,jugador);
}



// Esta funcion no se puede usar en la version entregable
// Aparece aqui solo para ILUSTRAR el comportamiento del juego
// ESTO NO IMPLEMENTA NI MINIMAX, NI PODA ALFABETA
void JuegoAleatorio(bool aplicables[], int opciones[], int &j){
    j=0;
    for (int i=0; i<8; i++){
        if (aplicables[i]){
           opciones[j]=i;
           j++;
        }
    }
}


//Poda alfa beta

//Usando la funcion GenerateAllMoves

double Player::Poda_AlfaBetaALL(const Environment & estado, int jugador, int profundidad, int PROFUNDIDAD_ALFABETA, Environment::ActionType & accion, double alpha, double beta) {
	 Environment::ActionType aux;
   bool posibles[8];
   int n = estado.possible_actions(posibles);
   double vj_k;

  //Si es un nodo terminal, devolver la valoracion
    if(profundidad==PROFUNDIDAD_ALFABETA || estado.possible_actions(posibles)==0){
      return Valoracion(estado, jugador);
    }

    //si no es nodo terminal...
    //si es MAX

		if (profundidad%2==0) {
      Environment acciones[8];
      int todos = estado.GenerateAllMoves(acciones);
			for(int i=0; i<todos; i++){
				vj_k = Poda_AlfaBetaALL(acciones[i], jugador, profundidad+1, PROFUNDIDAD_ALFABETA, aux, alpha, beta);

				if (alpha < vj_k){
					alpha = vj_k;
					accion = static_cast< Environment::ActionType > (i);
				}
				if (beta <= alpha){
					//return beta;
          break;
				}
			}
		  return alpha;
		}
    else { //si es MIN
      Environment acciones[8];
      int todos = estado.GenerateAllMoves(acciones);

			for(int i=0; i<todos; i++){
				vj_k = Poda_AlfaBetaALL(acciones[i], jugador, profundidad+1 , PROFUNDIDAD_ALFABETA, aux, alpha, beta);

				if (beta > vj_k){
				  beta = vj_k;
				  accion = static_cast< Environment::ActionType > (i);
				}

				if (beta <= alpha){
				  //return alpha;
          break;
				}
			}

		  return beta;

		}

}

//Usando la funcion GenerateNextMove
double Player::Poda_AlfaBetaNEXT(const Environment & estado, int jugador, int profundidad, int PROFUNDIDAD_ALFABETA, Environment::ActionType & accion, double alpha, double beta) {
    int winner = estado.RevisarTablero();
    if(winner==jugador){//gana el jugador que llama a la funcion
        return masinf;
    }
    else if(winner!=0){//gana el jugador contrario
        return menosinf;
    }
    else if(estado.Get_Casillas_Libres()==0){ //si el juego ha acabado, son tablas
        return 0;
    }

    Environment::ActionType aux;
	  //bool posibles[8];
	  //int n = estado.possible_actions(posibles);
    double vj_k;

    if(profundidad==PROFUNDIDAD_ALFABETA){
      return Valoracion(estado, jugador);
    }

    //si no es nodo terminal...
    //si es MAX
		if (profundidad%2==0) {
      int act = -1; //se le pasa en un inicio el parametro -1
      Environment siguiente = estado.GenerateNextMove(act);
			while(!(siguiente == estado)){
				vj_k = Poda_AlfaBetaNEXT(siguiente, jugador, profundidad+1, PROFUNDIDAD_ALFABETA, aux, alpha, beta);

				if (alpha < vj_k){
					alpha = vj_k;
					accion = static_cast< Environment::ActionType > (act);
				}
				if (alpha>=beta) {
					return beta; // Poda beta
          //break;
				}
				siguiente = estado.GenerateNextMove(act);
			}
		  return alpha;
		}
    else { //si es MIN
      int act = -1; //se le pasa en un inicio el parametro -1
      Environment siguiente = estado.GenerateNextMove(act);
		  while(!(siguiente == estado)){
				vj_k = Poda_AlfaBetaNEXT(siguiente, jugador, profundidad+1 , PROFUNDIDAD_ALFABETA, aux, alpha, beta);

				if (beta > vj_k){
				  beta = vj_k;
				  accion = static_cast< Environment::ActionType > (act);
				}
				if (beta <= alpha) {
				  return alpha;
          //break;
				}
				siguiente = estado.GenerateNextMove(act);
			}
		  return beta;
		}

}


// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // acci�n que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable



    double valor; // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alpha, beta; // Cotas de la poda AlfaBeta

    int n_act; //Acciones posibles en el estado actual


    n_act = actual_.possible_actions(aplicables); // Obtengo las acciones aplicables al estado actual en "aplicables"
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t=0; t<8; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;


    //--------------------- COMENTAR Desde aqui

    /*cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }*/

    //--------------------- COMENTAR Hasta aqui


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------


    // Opcion: Poda AlfaBeta
    // NOTA: La parametrizacion es solo orientativa

      cout << "Empezamos con la poda alfa beta" << endl;
      valor = Poda_AlfaBetaNEXT(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, menosinf, masinf);
      cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

      return accion;
}
