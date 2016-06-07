#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
      double Poda_AlfaBetaALL(const Environment & estado, int jugador, int profundidad, int PROFUNDIDAD_ALFABETA, Environment::ActionType & accion, double alpha, double beta);
      double Poda_AlfaBetaNEXT(const Environment & estado, int jugador, int profundidad, int PROFUNDIDAD_ALFABETA, Environment::ActionType & accion, double alpha, double beta);
      double Valoracion(const Environment &estado, int jugador);
      double evaluacion(const Environment &estado, int jugador);
      double evaluacionJugador(const Environment &estado, int jugador);
    private:
      int jugador_;
      Environment actual_;
      bool primer_turno;
};
#endif
