#ifndef LUZ_LUZSPOT_HPP
#define LUZ_LUZSPOT_HPP

#include <cmath>
#include "algebra/Ponto3.hpp"
#include "algebra/Vetor3.hpp"
#include "luz/LuzPontual.hpp"

class LuzSpot : public LuzPontual {

    private:
        Vetor3 direcao;
        // Ângulo de abertura em radianos
        double abertura;

    public:

        // --- CONSTRUTORES ---

            LuzSpot(Vetor3 direcao = Vetor3(), double abertura = M_PI_2, Ponto3 posicao = Ponto3(), IntensidadeLuz intensidade = IntensidadeLuz(), double atenuacao_a = 0.0, double atenuacao_b = 0.0, double atenuacao_c = 1.0);

        // -- GETTERS E SETTERS ---

            Vetor3 get_direcao() const;
            void set_direcao(Vetor3 d);

            double get_abertura() const;
            void set_abertura(double a);

        // --- MÉTODOS ---

            // Retorna true se o ponto for capaz de ser iluminado pela fonte de luz no caso de não haver obstrução de luz.
            bool ponto_valido(const Ponto3& ponto) const override;

};

#endif