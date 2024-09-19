#ifndef GEOMETRIA_BOUNDINGVOLUME_HPP
#define GEOMETRIA_BOUNDINGVOLUME_HPP

#include <vector>
#include <memory>
#include "geometria/Cilindro.hpp"
#include "geometria/malha/Malha.hpp"
#include "utils/tipos.hpp"

struct hit_info;

class BoundingVolume {

    private:
        Cilindro volume;
        std::vector<std::shared_ptr<Solido>> solidos;
        std::vector<std::shared_ptr<Malha>> malhas;
        std::vector<std::shared_ptr<BoundingVolume>> sub_grupos;
        Ponto3 ponto_minimo, ponto_maximo;
        double ajuste_volume;

    public:
        BoundingVolume (double _ajuste_volume = 0.0);

        Ponto3 get_ponto_minimo () const;

        Ponto3 get_ponto_maximo () const;

        double get_ajuste_volume () const;
        void set_ajuste_volume (double _ajuste_volume);

        void inserir (const Ponto3 & ponto);
        void inserir (const std::shared_ptr<Solido>& solido);
        void inserir (const std::shared_ptr<Malha>& malha);
        void inserir (const std::shared_ptr<BoundingVolume>& sub_grupo);

        hit_info calcula_interseccao (Raio& raio);

        // Retorna se um dado Ponto3 está contido dentro do volume.
        bool ponto_dentro (const Ponto3& ponto) const;

};

struct hit_info {

    rgb cor = {0, 0, 0};
    std::shared_ptr<Solido> solido = nullptr;
    std::shared_ptr<Malha> malha = nullptr;
    std::shared_ptr<BoundingVolume> bounding_volume = nullptr;
    double distancia = -1.0;
    bool intersectou = false;

};

#endif