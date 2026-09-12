#include "swsbbh/State.hpp"

#include <AMReX.H>
#include <AMReX_BoxArray.H>
#include <AMReX_DistributionMapping.H>
#include <AMReX_Geometry.H>
#include <AMReX_MultiFab.H>
#include <AMReX_ParmParse.H>
#include <AMReX_Print.H>

namespace {

void initialise_scaffold()
{
    static_assert(AMREX_SPACEDIM == 1, "This project evolves a radial mode in 1D.");

    amrex::ParmParse mesh("amr");
    int cells = 0;
    int max_grid_size = 128;
    int max_level = 0;
    mesh.get("n_cell", cells);
    mesh.query("max_grid_size", max_grid_size);
    mesh.query("max_level", max_level);

    if (cells < 8 || max_grid_size < 8) {
        amrex::Abort("Use at least eight cells and amr.max_grid_size >= 8.");
    }
    if (max_level != 0) {
        amrex::Abort("Adaptive evolution is a later milestone; use amr.max_level=0 for the scaffold.");
    }

    // Geometry reads geometry.prob_lo, prob_hi, coord_sys and is_periodic.
    const amrex::Box domain(amrex::IntVect(0), amrex::IntVect(cells - 1));
    const amrex::Geometry geometry(domain);
    amrex::BoxArray boxes(domain);
    boxes.maxSize(max_grid_size);
    const amrex::DistributionMapping distribution(boxes);

    // AMReX owns storage and, when enabled, distributes boxes across MPI ranks.
    amrex::MultiFab state(boxes, distribution, swsbbh::NumComponents,
                         swsbbh::GhostCells);
    state.setVal(0.0);

    amrex::Print()
        << "swsbbh: AMReX bootstrap only; no wave evolution is implemented.\n"
        << "AMReX revision: " << SWSBBH_AMREX_REVISION << '\n'
        << "Domain: [" << geometry.ProbLo(0) << ", " << geometry.ProbHi(0) << "]\n"
        << "Cells: " << cells << ", boxes: " << boxes.size()
        << ", dx: " << geometry.CellSize(0) << '\n'
        << "Allocated zeroed psi/Pi with " << swsbbh::GhostCells << " ghost cells.\n"
        << "Next: work through docs/roadmap.md, milestone 1, then implement initial data.\n"
        << "Physics, time-stepping and output parameters in the inputs are reserved for those exercises.\n";

    // Learning tasks: analytic initial data -> ghost fill -> RHS -> RK4.
    // Add each component with its own numerical check; see docs/roadmap.md.
}

} // namespace

int main(int argc, char* argv[])
{
    amrex::Initialize(argc, argv);
    {
        // Destroy all AMReX data before Finalize, especially for later MPI/GPU work.
        initialise_scaffold();
    }
    amrex::Finalize();
    return 0;
}

