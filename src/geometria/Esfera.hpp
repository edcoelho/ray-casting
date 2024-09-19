#ifndef GEOMETRIA_ESFERA_HPP
#define GEOMETRIA_ESFERA_HPP

#include "utils/Material.hpp"
#include "geometria/Solido.hpp"

class Esfera : public Solido {

    private:
        Ponto3 centro;
        double raio;

    public:

        // --- CONSTRUTORES ---

        Esfera(Material material = Material());

        Esfera(Ponto3 centro, double raio, Material material = Material());

        // --- GETTERS E SETTERS ---

        Ponto3 get_centro() const;
        void set_centro(Ponto3 centro);

        double get_raio() const;
        void set_raio(double raio);

        // --- MÉTODOS ---

        // Se a esfera for intersectada pelo Raio "raio", retorna o escalar que é a distância entre o ponto inicial do raio e o ponto de intersecção mais próximo.
        // Se não houver intersecção, retorna -1.
        double escalar_interseccao(Raio& raio) override;

        // Retorna o vetor unitário normal a superfície da esfera num ponto.
        Vetor3 vetor_normal_ponto(Ponto3 ponto) const override;

        // Retorna cor em formato de IntensidadeLuz do pixel da textura correspondente a um ponto.
        IntensidadeLuz cor_textura(Ponto3 ponto) override;

        // Retorna o ponto mínimo e ponto máximo da esfera.
        std::pair<Ponto3, Ponto3> pontos_min_max () const override;

};

#endif