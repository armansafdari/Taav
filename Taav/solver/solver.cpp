#include "solver_src/processorcontroller.h"
#include "solver_src/fluidmodels.h"

int main(int argc, char *argv[])
{
    plb::plbInit(&argc, &argv);

    std::string xmlFileName(argv[1]);
    Parameters  Param(xmlFileName);
    ProcessStart(Param.WorkFolder);
    FluidModels(Param);

}
