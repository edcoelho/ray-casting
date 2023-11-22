#ifndef INCLUDE_LIST_HPP
#define INCLUDE_LIST_HPP

// Bibliotecas externas
#include <SDL2/SDL.h>

// Biblioteca padrão do C++
#include <vector>
#include <array>
#include <memory>
#include <cmath>

// Headers implementados
#include "utils/tipos.hpp"
#include "utils/Material.hpp"

#include "algebra/Matriz3.hpp"
#include "algebra/Vetor3.hpp"
#include "algebra/Ponto3.hpp"

#include "geometria/Raio.hpp"
#include "geometria/Solido.hpp"
#include "geometria/Esfera.hpp"
#include "geometria/Plano.hpp"
#include "geometria/Cilindro.hpp"
#include "geometria/Cone.hpp"
#include "geometria/Triangulo.hpp"

#include "geometria/malha/Aresta.hpp"
#include "geometria/malha/Face.hpp"
#include "geometria/malha/Malha.hpp"
#include "geometria/malha/MalhaEspecifica.hpp"
#include "geometria/malha/RetanguloXY.hpp"
#include "geometria/malha/RetanguloXZ.hpp"
#include "geometria/malha/RetanguloYZ.hpp"

#include "luz/IntensidadeLuz.hpp"
#include "luz/FonteLuz.hpp"
#include "luz/LuzPontual.hpp"
#include "luz/LuzSpot.hpp"
#include "luz/LuzDirecional.hpp"

#include "cena/Cena.hpp"
#include "cena/Camera.hpp"
#include "cena/Canvas.hpp"

#endif