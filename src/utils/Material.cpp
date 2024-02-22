#include "utils/Material.hpp"

Material::Material(std::string nome, IntensidadeLuz ka, IntensidadeLuz kd, IntensidadeLuz ke, double espelhamento) {

    this->nome = nome;
    this->k_A = ka;
    this->k_D = kd;
    this->k_E = ke;
    this->espelhamento = espelhamento;

}

Material::Material(std::string nome, std::shared_ptr<Textura> textura) {

    this->nome = nome;
    this->textura = std::move(textura);
    this->k_A = IntensidadeLuz(1.0);
    this->k_D = IntensidadeLuz(0.7);
    this->k_E = IntensidadeLuz(0.0);
    this->espelhamento = 0.0;

}

std::string Material::get_nome() const {

    return this->nome;

}

IntensidadeLuz Material::get_k_A() {

    return this->k_A;
    
}
void Material::set_k_A(IntensidadeLuz k) {

    this->k_A = k;
   
}

IntensidadeLuz Material::get_k_D() {

    return this->k_D;
    
}
void Material::set_k_D(IntensidadeLuz k) {

    this->k_D = k;
   
}

IntensidadeLuz Material::get_k_E() {

    return this->k_E;
    
}
void Material::set_k_E(IntensidadeLuz k) {

    this->k_E = k;
   
}

double Material::get_espelhamento() {

    return this->espelhamento;

}
void Material::set_espelhamento(double e) {

    this->espelhamento = e;

}

std::shared_ptr<Textura> Material::get_textura() const {

    return this->textura;

}
void Material::set_textura(std::shared_ptr<Textura> t) {

    this->textura = std::move(t);

}