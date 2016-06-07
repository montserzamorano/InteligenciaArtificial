
#ifndef AGENT__
#define AGENT__

#include <string>
#include <vector>
#include <utility>
#include <iostream>
#include <cstdlib>
#include "conexion.h"
using namespace std;

// -----------------------------------------------------------
//				class Agent
// -----------------------------------------------------------
class Environment;
class Agent
{
public:
	Agent(){
		orientada=false; //la matriz no se ha orientado correctamente
		x_= 99;
		y_= 99;
		orientacion_=3;
		conocido = -1;
		pkencontrado = false;
		objetoEncontrado = false;
		personaEncontrada = false;
		random = false;
		n1 = -1;
		n2 = -1;
		contador_npk = 0;
		objetos_mochila = 0;
		contador_paso = 0;
		role_="PlYR";
		last_accion_=3;
		REINICIADO_=false;
		size_=200;
		for (int i=0;i<200;i++)
		    for(int j=0;j<200;j++){
		      mapa_entorno_[i][j]='?';
		      mapa_objetos_[i][j]='?';
		    }
		//inicializar toda la matriz menos los bordes a suelo
		for (int i=3;i<97;i++){
		    for(int j=3;j<97;j++){
		      mapa_solucion_[i][j]='S';
				}
		}
		//inicializar las tres filas de arriba a precipicio
		for (int i=0;i<3;i++){
				for(int j=0;j<100;j++){
					mapa_solucion_[i][j]='P';
				}
		}
		//inicializar las tres filas de la derecha a precipicio
		for (int i=97;i<100;i++){
				for(int j=0;j<100;j++){
					mapa_solucion_[i][j]='P';
				}
		}
		//inicializar las tres filas de la izquierda a precipicio
		for (int i=0;i<100;i++){
				for(int j=0;j<3;j++){
					mapa_solucion_[i][j]='P';
				}
		}
		//inicializar las tres filas de la abajo a precipicio
		for (int i=97;i<100;i++){
				for(int j=0;j<100;j++){
					mapa_solucion_[i][j]='P';
				}
		}
	}

	enum ActionType
	{
	    actFORWARD, // avanzar
	    actTURN_L,  // Girar Izquierda
	    actTURN_R,  // Girar Derecha
	    actIDLE,    // No hacer nada
	    actPICKUP,  // Recoger un objeto
	    actPUTDOWN, // Soltar un objeto
	    actPUSH,    // Meter en la mochila
	    actPOP,     // Sacar de la mochila
	    actGIVE,	// Dar un objeto a un personaje
	    actTHROW	// Lanzar un objeto

	};

	void Perceive(Environment &env);
	bool Perceive_Remote(conexion_client &Cliente, Environment &env);
	void ActualizarInformacion(Environment *env);
	ActionType Think();
	void FixLastAction(Agent::ActionType x){last_accion_=x;};

	char mapa_entorno_[200][200]; // mapa que muestra el tipo de terreno
	char mapa_objetos_[200][200]; // mapa que muestra los objetos que estan encima del terreno
	char mapa_solucion_[100][100]; // Mapa que almacena la solucion que el alumno propone
	// Funciones de acceso a los datos
	void GetCoord(int &fila, int &columna, int &brujula){fila=y_;columna=x_;brujula=orientacion_;};
	//Aniadidas por mi
	void setListaAbierta(vector < pair <int,int> > A, char m[200][200]);
	bool esObstaculo(char c);
	bool esTransitable(pair <int,int> A,char m[200][200]);
	bool enLista(pair <int,int> A, vector < pair <int,int> > lista);
	void setCuadrosAdyacentes(pair <int,int> A,char m[200][200]);
	int distanciaManhattan(pair <int,int> c1, pair <int,int> c2);
	pair <int,int> funcionObjetivo(pair <int,int> unCuadro);
	vector <Agent::ActionType> hallarRuta(pair <int,int> c1,pair <int,int> c2);
	vector <Agent::ActionType> AlgoritmoEstrella(char m[200][200]);
	bool esObstaculoMovil(char c);
	pair <int,int> generarCuadroObjetivo();
	void orientarMatriz();
	bool tengoZapatillas();
	bool tengoBikini();
	int buscaPK();
	pair <int,int> devuelveCoordenadas(int pos_vista);
	int buscar(char c);
	bool recogerObjeto(pair <int,int> unCuadro);
	bool hayObjeto(pair <int,int> unCuadro);
	int buscarObjetoQueSirva();
	vector <Agent::ActionType> guardarEnMochila();
	int enMochila(char c);
	vector <Agent::ActionType> cogerDeMochila(int pos);
	int cuantosEnMochila();
	vector <Agent::ActionType> soltarObjeto();
	bool hayObjetoEn(int pos);
	int hayPersona();
	vector <Agent::ActionType> regalarCositas();
	bool esPersonaDeInteres(char c);
private:
	//Variables de interaccion con el entorno grafico
	int size_;

	//SENSORES
	char VISTA_[10];
	char SURFACE_[10];
	bool REINICIADO_;
	string MENSAJE_;
	char EN_USO_;
	char MOCHILLA_[5];
	char PUNTUACION_[9];
	bool FIN_JUEGO_;
	char SALUD_;

	//Variables de estado
	int x_,y_, x_c, y_c, orientacion_;
	int last_accion_;
	string role_;

	//ANIADIDO
	bool orientada; //variable que almacena si la matriz se ha reorientado o no
	bool pkencontrado;
	bool objetoEncontrado;
	//ANIADIDOS
	vector < pair <int,int> > lista_abierta; //cuadros que faltan por recorrer
	vector < pair <int,int> > lista_cerrada;
	vector < pair <int,int> > lista_pks;
	pair <int,int> actual;
	vector <ActionType> ruta;
	int contador_npk;
	vector < pair <int,int> > pk1, pk2;
	int conocido;
	int n1;
	int n2;
	int orientacion_corregida;
	int objetos_mochila;
	bool personaEncontrada;
	bool random = false;
	int contador_paso;
};

string ActionStr(Agent::ActionType);

#endif
