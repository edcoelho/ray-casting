#ifndef GEOMETRIA_SOLIDO_HPP
#define GEOMETRIA_SOLIDO_HPP

#include "geometria/Raio.hpp"
#include "luz/LuzPontual.hpp"
#include "utils/Material.hpp"
#include "algebra/Ponto3.hpp"
#include "algebra/Vetor3.hpp"
#include <string>

class Solido {

    private:
        Material material;
        // Nome do sólido.
        std::string nome;

    public:

        // --- CONSTRUTORES ---

        Solido(const char* nome, Material material = Material());

        // --- GETTERS E SETTERS ---

        Material get_material() const;
        void set_material(Material material);

        std::string get_nome() const;

        // --- MÉTODOS ---

        // Se o sólido for intersectado pelo Raio "raio", retorna o escalar que é a distância entre o ponto inicial do raio e o ponto de intersecção mais próximo.
        // Se não houver intersecção, retorna -1.
        virtual double escalar_interseccao(Raio& raio) = 0;

        // Retorna o vetor unitário normal a superfície do sólido num ponto.
        virtual Vetor3 vetor_normal_ponto(Ponto3 ponto) const = 0;

        // Retorna true se o material do sólido é uma textura.
        bool tem_textura() const;

        // Retorna cor em formato de IntensidadeLuz do pixel da textura correspondente a um ponto.
        virtual IntensidadeLuz cor_textura(Ponto3 ponto) = 0;

};

#endif