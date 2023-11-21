#ifndef LUZ_LUZPONTUAL_HPP
#define LUZ_LUZPONTUAL_HPP

#include "luz/IntensidadeLuz.hpp"
#include "luz/FonteLuz.hpp"
#include "algebra/Ponto3.hpp"

class LuzPontual : public FonteLuz {

    private:
        // Posição do ponto de luz.
        Ponto3 posicao;

    public:

        // --- CONSTRUTORES ---

            LuzPontual();

            LuzPontual(IntensidadeLuz intensidade, Ponto3 posicao);

        // --- GETTERS E SETTERS ---

            Ponto3 get_posicao() const;
            void set_posicao(Ponto3 p);

        // --- MÉTODOS ---

            // Retorna true se o ponto for capaz de ser iluminado pela fonte de luz no caso de não haver obstrução de luz.
            bool ponto_valido(const Ponto3& ponto) const override;

            // Retorna o vetor direção que aponta do ponto para a fonte de luz.
            Vetor3 direcao_ponto_luz(const Ponto3& ponto) const override;

            // Retorna a distância entre o ponto e a fonte de luz pontual.
            double distancia_ponto_luz(const Ponto3& ponto) const override;

};

#endif