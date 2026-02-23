
#include "application.h"
#include "Sandsim/sandsim.h"
#include "Conway/conway.h"

int main()
{

  Cellata app;

  Sandsim sandsim;

  Conway conway;

  app.Add(&sandsim);
  app.Add(&conway);
  app.Run();

  return 0;
}
