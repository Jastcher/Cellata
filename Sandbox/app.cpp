
#include "application.h"
#include "Sandsim/sandsim.h"
#include "Conway/conway.h"
#include "Lattice gas/latticeGas.h"

int main()
{

  Cellata app;

  Sandsim sandsim;
  Conway conway;
  LatticeGas latticeGas;

  // Langton ant
  // better fluid sim
  // maze generator

  app.Add(&sandsim);
  app.Add(&conway);
  app.Add(&latticeGas);
  app.Run();

  return 0;
}
