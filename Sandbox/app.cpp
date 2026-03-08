
#include "Wireworld/wireworld.h"
#include "application.h"
#include "Sandsim/sandsim.h"
#include "Conway/conway.h"
#include "Lattice gas/latticeGas.h"
#include "Langtons ant/langtonsAnt.h"
#include "Mazectric/mazectric.h"
#include "BZReaction/BZReaction.h"
#include "Wireworld/wireworld.h"
#include "Mandelbrot/mandelbrot.h"

int main()
{

  Cellata app;

  BZReaction bzReaction;
  Sandsim sandsim;
  Conway conway;
  LatticeGas latticeGas;
  LangtonsAnt langtonsAnt;
  Mazectric mazectric;
  Wireworld wireworld;
  Mandelbrot mandelbrot;

  // Lattice Gas Automata LGA & FHP Models
  // Lattice gas with gravity?

  app.Add(&mandelbrot);
  app.Add(&sandsim);
  app.Add(&conway);
  app.Add(&latticeGas);
  app.Add(&langtonsAnt);
  app.Add(&mazectric);
  app.Add(&bzReaction);
  app.Add(&wireworld);

  app.Run();

  return 0;
}
