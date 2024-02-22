#include "geometria/malha/RetanguloYZ.hpp"
#include <cmath>

// --- CONSTRUTORES ---

RetanguloYZ::RetanguloYZ(Ponto3 centro_base, double largura, double altura, Material material, bool aponta_para_positivo) : MalhaEspecifica("Retângulo", material) {

    this->centro_base = centro_base;
    this->largura = largura;
    this->altura = altura;
    this->aponta_para_positivo = aponta_para_positivo;

    this->calcular_estrutura();

}

// --- GETTERS E SETTERS ---

Ponto3 RetanguloYZ::get_centro_base() {

    return this->centro_base;

}

double RetanguloYZ::get_largura() {

    return this->largura;

}

double RetanguloYZ::get_altura() {

    return this->altura;

}

void RetanguloYZ::calcular_estrutura() {

    if (this->aponta_para_positivo) {

        // Inserindo vértices de forma que a normal da face aponte para o X positivo.
        this->inserir_vertice(this->get_centro_base() + Vetor3(0.0, 0.0, this->get_largura()/2.0));
        this->inserir_vertice(this->get_centro_base() + Vetor3(0.0, 0.0, -1.0*this->get_largura()/2.0));
        this->inserir_vertice(this->get_centro_base() + Vetor3(0.0, this->get_altura(), this->get_largura()/2.0));
        this->inserir_vertice(this->get_centro_base() + Vetor3(0.0, this->get_altura(), -this->get_largura()/2.0));

    } else {

        // Inserindo vértices de forma que a normal da face aponte para o X negativo.
        this->inserir_vertice(this->get_centro_base() + Vetor3(0.0, 0.0, -1.0*this->get_largura()/2.0));
        this->inserir_vertice(this->get_centro_base() + Vetor3(0.0, 0.0, this->get_largura()/2.0));
        this->inserir_vertice(this->get_centro_base() + Vetor3(0.0, this->get_altura(), -this->get_largura()/2.0));
        this->inserir_vertice(this->get_centro_base() + Vetor3(0.0, this->get_altura(), this->get_largura()/2.0));

    }

    // Inserindo arestas.
    this->inserir_aresta(Aresta(1, 2));
    this->inserir_aresta(Aresta(2, 3));
    this->inserir_aresta(Aresta(3, 1));
    this->inserir_aresta(Aresta(2, 4));
    this->inserir_aresta(Aresta(4, 3));

    // Inserindo faces.
    this->inserir_face(Face(1, 2, 3));
    this->inserir_face(Face(2, 4, 5));

}

void RetanguloYZ::recalcular_atributos() {

    this->largura = (this->vertices[0] - this->vertices[1]).norma();
    this->altura = (this->vertices[0] - this->vertices[2]).norma();

}

IntensidadeLuz RetanguloYZ::cor_textura(Ponto3 ponto) {
    
    std::size_t id_face = this->id_ultima_face_intersectada;
    Ponto3 coordenadas_baricentricas = this->ultima_face_intersectada.get_coordenadas_baricentricas();
    // Coordenadas de textura.
    int x, y;
    std::size_t u, v;
    // Cor RGB do pixel da textura correspondende à intersecção.
    rgb cor_pixel;

    if (id_face == 1) {

        y = std::floor(coordenadas_baricentricas[2] * this->get_material().get_textura()->get_largura_pixels() - 0.5);

        x = std::floor(this->get_material().get_textura()->get_altura_pixels() - coordenadas_baricentricas[0] * this->get_material().get_textura()->get_altura_pixels() - 0.5);

    } else {

        y = std::floor((coordenadas_baricentricas[0] + coordenadas_baricentricas[2]) * this->get_material().get_textura()->get_largura_pixels() - 0.5);

        x = std::floor(this->get_material().get_textura()->get_altura_pixels() - (coordenadas_baricentricas[0] + coordenadas_baricentricas[1]) * this->get_material().get_textura()->get_altura_pixels() - 0.5);

    }

    v = x < 0 ? 0 : x;
    u = y < 0 ? 0 : y;

    // Obtendo a cor do pixel da textura.
    cor_pixel = this->get_material().get_textura()->get_cor_pixel(v, u);

    return IntensidadeLuz(cor_pixel);

}

void RetanguloYZ::transformar(Matriz4 const& matriz) {

    this->vertices.push_back(this->centro_base);

    Malha::transformar(matriz);

    this->centro_base = this->vertices.back();
    this->vertices.pop_back();

    this->recalcular_atributos();

}

void RetanguloYZ::transladar(double x, double y, double z) {

    this->vertices.push_back(this->centro_base);

    Malha::transladar(x, y, z);

    this->centro_base = this->vertices.back();
    this->vertices.pop_back();

    this->recalcular_atributos();

}

void RetanguloYZ::rotacionar(double angulo, EixoCanonico eixo) {

    this->vertices.push_back(this->centro_base);

    Malha::rotacionar(angulo, eixo);

    this->centro_base = this->vertices.back();
    this->vertices.pop_back();

    this->recalcular_atributos();

}

void RetanguloYZ::rotacionar(double angulo, Ponto3 ponto_eixo, Vetor3 direcao_eixo) {

    this->vertices.push_back(this->centro_base);

    Malha::rotacionar(angulo, ponto_eixo, direcao_eixo);

    this->centro_base = this->vertices.back();
    this->vertices.pop_back();

    this->recalcular_atributos();

}

void RetanguloYZ::escalar(double fator_x, double fator_y, double fator_z, Ponto3 ponto_amarra) {

    this->vertices.push_back(this->centro_base);

    Malha::escalar(fator_x, fator_y, fator_z, ponto_amarra);

    this->centro_base = this->vertices.back();
    this->vertices.pop_back();

    this->recalcular_atributos();

}

void RetanguloYZ::cisalhar(double angulo, EixoCanonico eixo1, EixoCanonico eixo2, Ponto3 ponto_amarra) {

    this->vertices.push_back(this->centro_base);

    Malha::cisalhar(angulo, eixo1, eixo2, ponto_amarra);

    this->centro_base = this->vertices.back();
    this->vertices.pop_back();

    this->recalcular_atributos();

}

void RetanguloYZ::refletir(EixoCanonico eixo1, EixoCanonico eixo2) {

    this->vertices.push_back(this->centro_base);

    Malha::refletir(eixo1, eixo2);

    this->centro_base = this->vertices.back();
    this->vertices.pop_back();

    this->recalcular_atributos();

}

void RetanguloYZ::refletir(Vetor3 vetor_normal_plano, Ponto3 ponto_plano) {

    this->vertices.push_back(this->centro_base);

    Malha::refletir(vetor_normal_plano, ponto_plano);

    this->centro_base = this->vertices.back();
    this->vertices.pop_back();

    this->recalcular_atributos();

}
