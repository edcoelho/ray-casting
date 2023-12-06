#ifndef GEOMETRIA_MALHA_MALHA_HPP
#define GEOMETRIA_MALHA_MALHA_HPP

#include <vector>
#include "utils/tipos.hpp"
#include "algebra/Ponto3.hpp"
#include "algebra/Vetor3.hpp"
#include "algebra/Matriz4.hpp"
#include "utils/Material.hpp"
#include "geometria/Triangulo.hpp"

// Estrutura para representar uma aresta de uma face triangular.
struct Aresta {

    Aresta(std::size_t id_vertice1 = 0, std::size_t id_vertice2 = 0);

    std::size_t id_vertice1;
    std::size_t id_vertice2;

};

// Estrutura para representar uma face triangular.
struct Face {

    Face(std::size_t id_aresta1 = 0, std::size_t id_aresta2 = 0, std::size_t id_aresta3 = 0);

    std::size_t id_aresta1;
    std::size_t id_aresta2;
    std::size_t id_aresta3;

};

// Malha de faces triangulares.
class Malha {

    private:

        Material material;

        // ID da última face intersectada.
        std::size_t id_ultima_face;

    protected:

        std::vector<Ponto3> vertices;
        std::vector<Aresta> arestas;
        std::vector<Face> faces;

    public:

        // --- CONSTRUTORES ---

            Malha();

            Malha(Material material);

            Malha(std::vector<Ponto3> vertices, std::vector<Aresta> arestas, std::vector<Face> faces, Material material);

        // --- GETTERS E SETTERS ---

            Material get_material() const;
            void set_material(Material m);

            std::size_t get_id_ultima_face() const;
            void set_id_ultima_face(std::size_t id);

        // --- OUTROS MÉTODOS ---

            // Busca vértice pelo ID. ID começa a contar a partir do 1.
            Ponto3 buscar_vertice_por_id(std::size_t id) const;
            void inserir_vertice(Ponto3 v);

            // Busca aresta pelo ID. ID começa a contar a partir do 1.
            Aresta buscar_aresta_por_id(std::size_t id) const;
            void inserir_aresta(Aresta a);

            // Busca face pelo ID. ID começa a contar a partir do 1.
            Face buscar_face_por_id(std::size_t id) const;
            void inserir_face(Face f);

            // TODO: Lista de materiais

            // Se a malha for intersectada pelo raio, retorna o escalar que é a distância entre o ponto inicial do raio e o ponto de intersecção mais próximo. Além disso, salva a intersecção no histórico de intersecções.
            // Se não houver intersecção, retorna -1.
            double escalar_interseccao(Raio& raio);

            // Retorna um objeto do tipo Triangulo correspondente à face do ID passado.
            Triangulo triangulo_por_id_face(std::size_t id_face) const;

            // Aplica uma matriz de transformação qualquer.
            virtual void transformar(Matriz4 const& matriz);

            // Aplica uma translação.
            virtual void transladar(double x, double y, double z);

            // Aplica uma rotação em um dos eixos canônicos.
            virtual void rotacionar(double angulo, EixoCanonico eixo);
            // Aplica uma rotação em um eixo arbitrário.
            virtual void rotacionar(double angulo, Ponto3 ponto_eixo, Vetor3 direcao_eixo);

            // Aplica uma escala.
            virtual void escalar(double fator_x, double fator_y, double fator_z, Ponto3 ponto_amarra = Ponto3(0.0));

            // Aplica um cisalhamento num plano formado pelo eixo1 e eixo2 cisalhando no eixo2.
            virtual void cisalhar(double angulo, EixoCanonico eixo1, EixoCanonico eixo2, Ponto3 ponto_amarra = Ponto3(0.0));

            // Aplica uma reflexão em um plano formado por dois dos eixos canônicos.
            virtual void refletir(EixoCanonico eixo1, EixoCanonico eixo2);
            // Aplica uma reflexão em um plano arbitrário.
            virtual void refletir(Vetor3 vetor_normal_plano, Ponto3 ponto_plano = Ponto3(0.0));

};

#endif