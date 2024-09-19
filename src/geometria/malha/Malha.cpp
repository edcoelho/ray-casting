#include "geometria/malha/Malha.hpp"
#include <cmath>
#include <stdexcept>

Aresta::Aresta(std::size_t id_vertice1, std::size_t id_vertice2) {

    this->id_vertice1 = id_vertice1;
    this->id_vertice2 = id_vertice2;

}

Face::Face(std::size_t id_aresta1, std::size_t id_aresta2, std::size_t id_aresta3) {

    this->id_aresta1 = id_aresta1;
    this->id_aresta2 = id_aresta2;
    this->id_aresta3 = id_aresta3;

}

Malha::Malha(std::string nome, Material material) {

    this->nome = nome;
    this->material = material;

}

std::string Malha::get_nome() const {

    return this->nome;

}
void Malha::set_nome(std::string nome) {

    this->nome = nome;

}

Material Malha::get_material() const {

    return this->material;

}
void Malha::set_material(Material m) {

    this->material = m;

}

std::size_t Malha::get_id_ultima_face_intersectada() const {

    return this->id_ultima_face_intersectada;

}

Triangulo Malha::get_ultima_face_intersectada() {

    return this->ultima_face_intersectada;

}

Ponto3 Malha::buscar_vertice_por_id(std::size_t id) const {

    return this->vertices[id - 1];

}
void Malha::inserir_vertice(Ponto3 v) {

    this->vertices.push_back(v);

}

Aresta Malha::buscar_aresta_por_id(std::size_t id) const {

    return this->arestas[id - 1];

}
void Malha::inserir_aresta(Aresta a) {

    this->arestas.push_back(a);

}

Face Malha::buscar_face_por_id(std::size_t id) const {

    return this->faces[id - 1];

}
void Malha::inserir_face(Face f) {

    this->faces.push_back(f);

}

double Malha::escalar_interseccao(Raio& raio) {

    double t_int = -1.0;
    double min_t_int = INFINITY;
    // Triângulo intersectado primeiro.
    Triangulo triangulo;

    std::size_t id;

    // Itera sobre as faces. Os IDs começam a contar a partir de 1.
    for (id = 1; id <= this->faces.size(); id++) {

        triangulo = this->triangulo_por_id_face(id);
        t_int = triangulo.escalar_interseccao(raio);

        if (t_int >= 0.0 && t_int < min_t_int) {

            min_t_int = t_int;
            this->id_ultima_face_intersectada = id;
            this->ultima_face_intersectada = triangulo;

        }

    }

    if (min_t_int < INFINITY) {

        return min_t_int;

    }

    return -1.0;

}

bool Malha::tem_textura() const {

    if (this->material.get_textura() != nullptr) {

        return true;

    } else {

        return false;

    }

}

IntensidadeLuz Malha::cor_textura(Ponto3 ponto) { return this->get_material().get_k_D(); }

Triangulo Malha::triangulo_por_id_face(std::size_t id_face) const {

    Face face;
    Aresta aresta1, aresta2;

    // IDs dos vertices da face.

    std::size_t id_vertice_1, id_vertice_2, id_vertice_3;

    // Variáveis auxiliares.

    std::size_t aux1;
    float aux2;

    face = this->buscar_face_por_id(id_face);

    aresta1 = this->buscar_aresta_por_id(face.id_aresta1);
    aresta2 = this->buscar_aresta_por_id(face.id_aresta2);

    // Determinando vértices em sentido anti-horário.

    aux1 = aresta1.id_vertice1 * aresta1.id_vertice2;
    aux2 = (float) aux1 / (float) aresta2.id_vertice1;

    if (aux2 == ((float) aresta1.id_vertice1) || aux2 == ((float) aresta1.id_vertice2)) {

        id_vertice_1 = aresta2.id_vertice1;
        id_vertice_2 = aresta2.id_vertice2;
        id_vertice_3 = aux2;

    } else {

        id_vertice_1 = aresta2.id_vertice2;
        id_vertice_2 = aresta2.id_vertice1;
        id_vertice_3 = aux1 / aresta2.id_vertice2;
        
    }

    return Triangulo(this->buscar_vertice_por_id(id_vertice_1), this->buscar_vertice_por_id(id_vertice_2), this->buscar_vertice_por_id(id_vertice_3), this->get_material());

}

void Malha::transformar(Matriz4 const& matriz) {

    for (auto& vertice : this->vertices)

        vertice = matriz * vertice;

}

void Malha::transladar(double x, double y, double z) {

    Matriz4 matriz_t;

    matriz_t(0, 3) = x;
    matriz_t(1, 3) = y;
    matriz_t(2, 3) = z;

    for (auto& vertice : this->vertices) {

        vertice = matriz_t * vertice;

    }

}

void Malha::rotacionar(double angulo, EixoCanonico eixo) {

    Matriz4 matriz_r;
    double cos_angulo, sen_angulo;

    cos_angulo = std::cos(angulo);
    sen_angulo = std::sin(angulo);

    if (eixo == EIXO_X) {

        matriz_r(1, 1) = cos_angulo;
        matriz_r(1, 2) = -sen_angulo;
        matriz_r(2, 1) = sen_angulo;
        matriz_r(2, 2) = cos_angulo;

    } else if (eixo == EIXO_Y) {

        matriz_r(0, 0) = cos_angulo;
        matriz_r(0, 2) = sen_angulo;
        matriz_r(2, 0) = -sen_angulo;
        matriz_r(2, 2) = cos_angulo;

    } else {

        matriz_r(0, 0) = cos_angulo;
        matriz_r(0, 1) = -sen_angulo;
        matriz_r(1, 0) = sen_angulo;
        matriz_r(1, 1) = cos_angulo;

    }

    for (auto& vertice : this->vertices) {

        vertice = matriz_r * vertice;

    }

}

/*
void Malha::rotacionar(double angulo, Ponto3 ponto_eixo, Vetor3 direcao_eixo) {

    Matriz4 matriz_r, matriz_x_linha_para_x, matriz_x_para_x_linha;
    Vetor3 i_linha, j_linha, k_linha;
    double cos_angulo, sen_angulo;

    k_linha = direcao_eixo.unitario();
    j_linha = (k_linha * Vetor3(-k_linha[1], k_linha[0], 0.0)).unitario();
    i_linha = (j_linha * k_linha).unitario();

    cos_angulo = std::cos(angulo);
    sen_angulo = std::sin(angulo);

    matriz_r_z(0, 0) = cos_angulo;
    matriz_r_z(0, 1) = -sen_angulo;
    matriz_r_z(1, 0) = sen_angulo;
    matriz_r_z(1, 1) = cos_angulo;

    for (int c = 0; c < 3; c++) {

        matriz_x_linha_para_x(c, 0) = i_linha[c];
        matriz_x_linha_para_x(c, 1) = j_linha[c];
        matriz_x_linha_para_x(c, 2) = k_linha[c];
        matriz_x_linha_para_x(c, 3) = ponto_eixo[c];

        matriz_x_para_x_linha(0, c) = i_linha[c];
        matriz_x_para_x_linha(1, c) = j_linha[c];
        matriz_x_para_x_linha(2, c) = k_linha[c];

    }

    matriz_x_para_x_linha(0, 3) = -i_linha.escalar(ponto_eixo.vetor());
    matriz_x_para_x_linha(1, 3) = -j_linha.escalar(ponto_eixo.vetor());
    matriz_x_para_x_linha(2, 3) = -k_linha.escalar(ponto_eixo.vetor());

    matriz_r = matriz_x_linha_para_x * matriz_r * matriz_x_para_x_linha;

    for (auto& vertice : this->vertices) {

        vertice = matriz_r * vertice;

    }

}
*/

void Malha::rotacionar(double angulo, Ponto3 ponto_eixo, Vetor3 direcao_eixo) {

    Matriz4 matriz_r;
    Vetor3 i, j, k, aux;
    double cos_angulo, sen_angulo, i_escalar_p, j_escalar_p, k_escalar_p, a, b, c, d, e, f;
    // Índice com menor valor no vetor direcao.
    std::size_t menor_indice = 0;

    direcao_eixo.normalizar();

    k = direcao_eixo;

    // Escolhendo índice com menor valor do vetor direção.
    for (int ind = 0; ind < 3; ind++) {

        if (std::abs(direcao_eixo[menor_indice]) > std::abs(direcao_eixo[ind])) {

            menor_indice = ind;

        }
        
    }

    // Trocando os dois maiores valores do vetor direcao de lugar e invertendo o sinal de um dos valores para obter um vetor ortogonal.
    if (menor_indice == 0) {

        aux[1] = direcao_eixo[2];
        aux[2] = -direcao_eixo[1];

    } else if (menor_indice == 1) {

        aux[0] = direcao_eixo[2];
        aux[2] = -direcao_eixo[0];

    } else {

        aux[0] = direcao_eixo[1];
        aux[1] = -direcao_eixo[0];

    }

    j = (k * aux).unitario();
    i = (j * k).unitario();

    // Valores repetidos com frequência no cálculo da matriz.

    cos_angulo = std::cos(angulo);
    sen_angulo = std::sin(angulo);

    i_escalar_p = i.escalar(ponto_eixo.vetor());
    j_escalar_p = j.escalar(ponto_eixo.vetor());
    k_escalar_p = k.escalar(ponto_eixo.vetor());

    a = j[0] * sen_angulo + i[0] * cos_angulo;
    b = j[0] * cos_angulo - i[0] * sen_angulo;

    c = j[1] * sen_angulo + i[1] * cos_angulo;
    d = j[1] * cos_angulo - i[1] * sen_angulo;

    e = i[2] * cos_angulo + j[2] * sen_angulo;
    f = j[2] * cos_angulo - i[2] * sen_angulo;

    // 1ª linha

    matriz_r(0, 0) = (i[0] * i[0] + j[0] * j[0]) * cos_angulo + k[0] * k[0];
    matriz_r(0, 1) = i[1] * a + k[0] * k[1] + j[1] * b;
    matriz_r(0, 2) = i[2] * a + k[0] * k[2] + j[2] * b;
    matriz_r(0, 3) = ponto_eixo[0] - i_escalar_p * a - j_escalar_p * b - k[0] * k_escalar_p;

    // 2ª linha

    matriz_r(1, 0) = i[0] * c + j[0] * d + k[0] * k[1];
    matriz_r(1, 1) = (i[1] * i[1] + j[1] * j[1]) * cos_angulo + k[1] * k[1];
    matriz_r(1, 2) = i[2] * c + k[1] * k[2] + j[2] * d;
    matriz_r(1, 3) = ponto_eixo[1] - i_escalar_p * c - j_escalar_p * d - k[1] * k_escalar_p;
    
    // 3ª linha

    matriz_r(2, 0) = i[0] * e + k[0] * k[2] + j[0] * f;
    matriz_r(2, 1) = i[1] * e + k[1] * k[2] + j[1] * f;
    matriz_r(2, 2) = (i[2] * i[2] + j[2] * j[2]) * cos_angulo + k[2] * k[2];
    matriz_r(2, 3) = ponto_eixo[2] - i_escalar_p * e - j_escalar_p * f - k[2] * k_escalar_p;


    for (auto& vertice : this->vertices) {

        vertice = matriz_r * vertice;

    }

}

void Malha::escalar(double fator_x, double fator_y, double fator_z, Ponto3 ponto_amarra) {

    Matriz4 matriz_t_para_origem, matriz_esc, matriz_t_para_original;

    matriz_esc(0, 0) = fator_x;
    matriz_esc(1, 1) = fator_y;
    matriz_esc(2, 2) = fator_z;

    if (ponto_amarra != Ponto3(0.0)) {

        for (int i = 0; i < 3; i++) {

            matriz_t_para_origem(i, 3) = -ponto_amarra[i];
            matriz_t_para_original(i, 3) = ponto_amarra[i];

        }

        matriz_esc = matriz_t_para_original * matriz_esc * matriz_t_para_origem;

    }

    for (auto& vertice : this->vertices) {

        vertice = matriz_esc * vertice;

    }

}

void Malha::cisalhar(double angulo, EixoCanonico eixo1, EixoCanonico eixo2, Ponto3 ponto_amarra) {

    Matriz4 matriz_t_para_origem, matriz_c, matriz_t_para_original;
    double tan_angulo;

    if (eixo1 != eixo2) {

        tan_angulo = std::tan(angulo);

        if (eixo1 == EIXO_X) {

            if (eixo2 == EIXO_Y) {

                // Cisalhamento XY
                matriz_c(1, 2) = tan_angulo;

            } else {

                // Cisalhamento XZ
                matriz_c(2, 1) = tan_angulo;

            }

        } else if (eixo1 == EIXO_Y) {

            if (eixo2 == EIXO_X) {

                // Cisalhamento YX
                matriz_c(0, 2) = tan_angulo;

            } else {

                // Cisalhamento YZ
                matriz_c(2, 0) = tan_angulo;

            }

        } else {

            if (eixo2 == EIXO_X) {

                // Cisalhamento ZX
                matriz_c(0, 1) = tan_angulo;

            } else {

                // Cisalhamento ZY
                matriz_c(1, 0) = tan_angulo;

            }

        }

        if (ponto_amarra != Ponto3(0.0)) {

            for (int i = 0; i < 3; i++) {

                matriz_t_para_origem(i, 3) = -ponto_amarra[i];
                matriz_t_para_original(i, 3) = ponto_amarra[i];

            }

            matriz_c = matriz_t_para_original * matriz_c * matriz_t_para_origem;

        }

        for (auto& vertice : this->vertices) {

            vertice = matriz_c * vertice;

        }

    } else {

        throw std::invalid_argument("Erro: Tentativa de aplicar um cisalhamento em um plano formado por dois eixos canônicos iguais.");

    }

}

void Malha::refletir(EixoCanonico eixo1, EixoCanonico eixo2) {

    Matriz4 matriz_esp;
    std::size_t aux; // Variável para auxiliar na troca da ordem das arestas.

    if (eixo1 != eixo2) {

        if (eixo1 == EIXO_X) {

            if (eixo2 == EIXO_Y) {

                // Reflexão XY
                matriz_esp(2, 2) = -1.0;

            } else {

                // Reflexão XZ
                matriz_esp(1, 1) = -1.0;

            }

        } else if (eixo1 == EIXO_Y) {

            if (eixo2 == EIXO_X) {

                // Reflexão XY
                matriz_esp(2, 2) = -1.0;

            } else {

                // Reflexão YZ
                matriz_esp(0, 0) = -1.0;

            }

        } else {

            if (eixo2 == EIXO_X) {

                // Reflexão XZ
                matriz_esp(1, 1) = -1.0;

            } else {

                // Reflexão YZ
                matriz_esp(0, 0) = -1.0;

            }

        }

        for (auto& vertice : this->vertices) {

            vertice = matriz_esp * vertice;

        }

        // Trocando a posição das arestas na faces para que elas fiquem armazenadas no sentido anti-horário e, assim, não invertam o vetor normal das faces.
        for (auto& face : this->faces) {

            aux = face.id_aresta2;
            face.id_aresta2 = face.id_aresta3;
            face.id_aresta3 = aux;

        }

    } else {

        throw std::invalid_argument("Erro: Tentativa de aplicar uma reflexão em um plano formado por dois eixos canônicos iguais.");

    }

}

void Malha::refletir(Vetor3 vetor_normal_plano, Ponto3 ponto_plano) {

    Matriz4 matriz_esp, matriz_t_origem_para_ponto, matriz_householder, matriz_t_ponto_para_origem;
    std::size_t aux; // Variável para auxiliar na troca da ordem das arestas.

    for (int i = 0; i < 3; i++) {

        matriz_t_ponto_para_origem(i, 3) = -ponto_plano[i];
        matriz_t_origem_para_ponto(i, 3) = ponto_plano[i];

    }

    vetor_normal_plano.normalizar();

    for (int i = 0; i < 3; i++) {

        for (int j = 0; j < 3; j++) {

            matriz_householder(i, j) = matriz_householder(i, j) - (2 * vetor_normal_plano[i] * vetor_normal_plano[j]);

        }

    }

    if (ponto_plano != Ponto3(0.0)) {

        matriz_esp = matriz_t_origem_para_ponto * matriz_householder * matriz_t_ponto_para_origem;

    } else {

        matriz_esp = matriz_householder;

    }

    for (auto& vertice : this->vertices) {

        vertice = matriz_esp * vertice;

    }

    // Trocando a posição das arestas na faces para que elas fiquem armazenadas no sentido anti-horário e, assim, não invertam o vetor normal das faces.
    for (auto& face : this->faces) {

        aux = face.id_aresta2;
        face.id_aresta2 = face.id_aresta3;
        face.id_aresta3 = aux;

    }

}

std::pair<Ponto3, Ponto3> Malha::pontos_min_max () const {

    std::pair<Ponto3, Ponto3> resultado(Ponto3(INFINITY), Ponto3(-INFINITY));

    for (std::size_t i = 0; i < this->vertices.size(); i++) {

        for (std::size_t j = 0; j < 3; j++) {

            if (this->vertices[i][j] < resultado.first[j]) resultado.first[j] = this->vertices[i][j];
            if (this->vertices[i][j] > resultado.second[j]) resultado.second[j] = this->vertices[i][j];

        }

    }

    return resultado;

}