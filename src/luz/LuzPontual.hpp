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

            LuzPontual(Ponto3 posicao = Ponto3(), IntensidadeLuz intensidade = IntensidadeLuz(), double atenuacao_a = 0.0, double atenuacao_b = 0.0, double atenuacao_c = 1.0);

        // --- GETTERS E SETTERS ---

            Ponto3 get_posicao() const;
            void set_posicao(Ponto3 p);

        // --- OUTROS MÉTODOS ---

            // Retorna o vetor direção que aponta do ponto para a fonte de luz.
            Vetor3 direcao_ponto_luz(const Ponto3& ponto) const override;

            // Retorna a distância entre o ponto e a fonte de luz.
            double distancia_ponto_luz(const Ponto3& ponto) const override;

            // Retorna o fator de atenuação da luz num ponto.
            double fator_atenuacao(const Ponto3& ponto) const override;

};

#endif