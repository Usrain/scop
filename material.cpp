#include "includes/material.hpp"
material::material()
    : name(""),
      Ka(Vec3{0.2f, 0.2f, 0.2f}),
      Kd(Vec3{0.8f, 0.8f, 0.8f}),
      Ks(Vec3{0.0f, 0.0f, 0.0f}),
      Ke(Vec3{0.0f, 0.0f, 0.0f}),
      Tf(Vec3{1.0f, 1.0f, 1.0f}),
      Ns(0.0f),
      Ni(1.0f),
      d(1.0f),
      Tr(0.0f),
      illum(2),
      map_Ka(""),
      map_Kd(""),
      map_Ks(""),
      map_Ns(""),
      map_d(""),
      map_bump("")
{
}
material::~material()
{}