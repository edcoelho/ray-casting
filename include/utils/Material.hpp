#ifndef UTILS_MATERIAL_HPP
#define UTILS_MATERIAL_HPP

#include "luz/IntensidadeLuz.hpp"
#include "utils/Textura.hpp"

#include <memory>
#include <string>

class Material {

    private:

        std::string nome;

        // K ambiente
        IntensidadeLuz k_A;
        // K difusão
        IntensidadeLuz k_D;
        // K especulamento (cor da luz "refletida" pelo sólido)
        IntensidadeLuz k_E;
        // Expoente de espelhamento.
        double espelhamento;

        std::shared_ptr<Textura> textura;

    public:

        // --- CONSTRUTORES ---

        Material(std::string nome = std::string("padrao"), IntensidadeLuz ka = IntensidadeLuz(1.0, 1.0, 1.0), IntensidadeLuz kd = IntensidadeLuz(0.7, 0.7, 0.7), IntensidadeLuz ke = IntensidadeLuz(1.0, 1.0, 1.0), double espelhamento = 10.0);

        Material(std::string nome, std::shared_ptr<Textura> textura);

        // --- GETTERS E SETTERS ---

        std::string get_nome() const;

        IntensidadeLuz get_k_A();
        void set_k_A(IntensidadeLuz k);

        IntensidadeLuz get_k_D();
        void set_k_D(IntensidadeLuz k);

        IntensidadeLuz get_k_E();
        void set_k_E(IntensidadeLuz k);

        double get_espelhamento();
        void set_espelhamento(double e);

        std::shared_ptr<Textura> get_textura() const;
        void set_textura(std::shared_ptr<Textura> t);

};

#endif