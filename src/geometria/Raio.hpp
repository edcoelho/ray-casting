#ifndef GEOMETRIA_RAIO_HPP
#define GEOMETRIA_RAIO_HPP

#include "algebra/Ponto3.hpp"
#include "algebra/Vetor3.hpp"

class Raio {

    private:
        // Ponto inicial.
        Ponto3 ponto_inicial;
        // Vetor direção.
        Vetor3 direcao;

    public:

        // --- CONSTRUTORES ---

        Raio();

        // ponto_x: outro ponto qualquer do raio
        Raio(Ponto3 ponto_inicial, Ponto3 ponto_x);

        Raio(Ponto3 ponto_inicial, Vetor3 direcao);

        // --- GETTERS E SETTERS ---

        Ponto3 get_ponto_inicial();
        void set_ponto_inicial(Ponto3 p);

        Vetor3 get_direcao();
        void set_direcao(Vetor3 d);

        // --- MÉTODOS ---

        // Retorna um ponto do raio que está a uma distância "escalar" do ponto inicial.
        Ponto3 ponto_do_raio(double escalar);

};

#endif