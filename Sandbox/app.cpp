
#include "application.h"
#include "Sandsim/sandsim.h"
#include "Conway/conway.h"
#include "Lattice gas/latticeGas.h"
#include "Langtons ant/langtonsAnt.h"
#include "Mazectric/mazectric.h"
#include "BZReaction/BZReaction.h"

int main()
{

  Cellata app;

  BZReaction bzReaction;
  Sandsim sandsim;
  Conway conway;
  LatticeGas latticeGas;
  LangtonsAnt langtonsAnt;
  Mazectric mazectric;

  app.Add(&bzReaction);

  app.Add(&sandsim);
  app.Add(&conway);
  app.Add(&latticeGas);
  app.Add(&langtonsAnt);
  app.Add(&mazectric);

  app.Run();

  return 0;
}
