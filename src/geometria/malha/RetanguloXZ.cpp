#include "geometria/malha/RetanguloXZ.hpp"

// --- CONSTRUTORES ---

RetanguloXZ::RetanguloXZ() {

    this->centro_base = Ponto3();
    this->largura = 30;
    this->altura = 60.0;
    this->set_material(Material());
    this->sentido_vetor_normal = true;

    this->calcular_estrutura();

}

RetanguloXZ::RetanguloXZ(Ponto3 centro_base, double largura, double altura, Material material, bool sentido_vetor_normal) {

    this->centro_base = centro_base;
    this->largura = largura;
    this->altura = altura;
    this->set_material(material);
    this->sentido_vetor_normal = sentido_vetor_normal;

    this->calcular_estrutura();

}

// --- GETTERS E SETTERS ---

Ponto3 RetanguloXZ::get_centro_base() {

    return this->centro_base;

}

double RetanguloXZ::get_largura() {

    return this->largura;

}

double RetanguloXZ::get_altura() {

    return this->altura;

}

bool RetanguloXZ::get_sentido_vetor_normal() {

    return this->sentido_vetor_normal;

}

void RetanguloXZ::calcular_estrutura() {

    if (this->get_sentido_vetor_normal()) {

        // Inserindo vértices de forma que a normal da face aponte para o Y positivo.
        this->inserir_vertice(this->get_centro_base() + Vetor3(this->get_largura()/2.0, 0.0, 0.0));
        this->inserir_vertice(this->get_centro_base() + Vetor3(-1.0*this->get_largura()/2.0, 0.0, 0.0));
        this->inserir_vertice(this->get_centro_base() + Vetor3(this->get_largura()/2.0, 0.0, this->get_altura()));
        this->inserir_vertice(this->get_centro_base() + Vetor3(-1.0*this->get_largura()/2.0, 0.0, this->get_altura()));

    } else {

        // Inserindo vértices de forma que a normal da face aponte para o Y negativo.
        this->inserir_vertice(this->get_centro_base() + Vetor3(-1.0*this->get_largura()/2.0, 0.0, 0.0));
        this->inserir_vertice(this->get_centro_base() + Vetor3(this->get_largura()/2.0, 0.0, 0.0));
        this->inserir_vertice(this->get_centro_base() + Vetor3(-1.0*this->get_largura()/2.0, 0.0, this->get_altura()));
        this->inserir_vertice(this->get_centro_base() + Vetor3(this->get_largura()/2.0, 0.0, this->get_altura()));

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