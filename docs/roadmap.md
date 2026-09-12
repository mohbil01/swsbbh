# Learning roadmap

The target is a reproducible scalar-wave scattering experiment, not a completed full-GR code. Plan for roughly 80 hours, including theory and review. Only the build/bootstrap and study materials are supplied initially.

## 1. Derive the model — about 10 hours

Read the theory guide through initial data and boundary conditions. Work the exercises before looking up extra machinery.

- [ ] Derive the reduced wave equation from the covariant scalar equation.
- [ ] Explain why the tortoise coordinate sends the horizon to minus infinity.
- [ ] Derive the signs for incoming and outgoing pulses.
- [ ] Derive the energy density/flux and the quasinormal boundary conditions.
- [ ] Calculate the time window unaffected by significant boundary reflections.

**Review:** explain the meaning of each evolved quantity, the test-field approximation, the origin of the potential and what ringdown measures.

## 2. Ordinary waves and AMReX — about 12 hours

The bootstrap already owns Geometry, BoxArray, DistributionMapping and MultiFab. Trace each object's role in `src/main.cpp`, then implement the following components yourself.

- [ ] Initialise the periodic analytic solution `psi=sin(x)`, `Pi=-cos(x)` at cell centres.
- [ ] Fill periodic ghost cells and implement the fourth-order second derivative.
- [ ] Implement the two-component RHS and RK4; fill ghosts at every stage.
- [ ] Add L2/L-infinity error diagnostics against `sin(x-t)` and `-cos(x-t)`.
- [ ] Run a three-resolution convergence experiment at one period.

**Review:** explain point samples versus cell averages, ghost values versus valid cells, numerical dispersion, and the assumptions behind the CFL bound. Expect approximately fourth-order convergence in the resolved regime.

## 3. Schwarzschild scattering — about 16 hours

- [ ] Implement the dimensionless tortoise inversion and scalar potential in a separate background component.
- [ ] Precompute the potential when a grid is created; use the logarithmic variable described in the guide near the horizon.
- [ ] Initialise the Gaussian in `inputs/schwarzschild`, with `Pi=partial_x psi`.
- [ ] Add nonperiodic, homogeneous Dirichlet ghost filling by odd reflection at each endpoint face.
- [ ] Extract the waveform with at least fourth-order spatial accuracy, and add reduced-wave-energy diagnostics.
- [ ] Inspect propagation and scattering; repeat with a larger domain at the same spacing.

**Review:** a visible oscillation alone does not establish physical ringdown. Establish resolution and domain dependence first. The unit input amplitude is a normalisation for a linear test field.

## 4. Adaptive refinement — about 20 hours

Study the pinned [Wave_AmrLevel tutorial](https://github.com/AMReX-Codes/amrex-tutorials/tree/1a73f32e10514d917333ea7a3fd54e661e232144/ExampleCodes/Amr/Wave_AmrLevel). Reuse the framework's lifecycle and RK-aware interpolation rather than inventing time interpolation for fine-grid ghost cells.

- [ ] Introduce an `AmrLevel` subclass; first reproduce the single-level solution.
- [ ] Validate a fixed ratio-two refinement interface with a flat travelling pulse.
- [ ] Use the tutorial's quartic coarse-to-fine interpolation and fourth-order point-value restriction. Do not substitute cell-average restriction.
- [ ] Add RK4-compatible subcycling and then dynamic regridding at synchronised coarse times.
- [ ] Enable the forced interval and curvature indicator specified in `inputs/schwarzschild_amr`.
- [ ] Mask coarse cells covered by finer levels in composite diagnostics; refresh masks after regridding.
- [ ] Quantify interface reflections and regridding error under refinement.

The final hierarchy has a maximum of two finer levels, each at ratio two. Always refine `[-20,80]` on eligible levels; outside it, start with the dimensionless indicator `dx^2*abs(Dxx psi)/max(max_level_abs_psi,1e-12) > 1e-3`. Here `max_level_abs_psi` means the maximum absolute value over valid cells of the current AMR level. Buffer eight cells and regrid every four coarse steps. Sweep the threshold during validation.

## 5. Numerical study — about 14 hours

- [ ] Uniform grids: 1,024, 2,048 and 4,096 cells; reference: 8,192 cells on the default domain.
- [ ] Adaptive grids: 256, 512 and 1,024 base cells, with two finer levels, matching the uniform finest spacings.
- [ ] Compare waveforms at common physical times using interpolation that preserves the intended accuracy.
- [ ] Fit `exp(-gamma*(t-t0))*(A*cos(omega*(t-t0))+B*sin(omega*(t-t0)))` across several post-prompt time windows.
- [ ] Report fit residuals and dependence on the window, spacing and boundary placement.
- [ ] Compare waveform error with wall time and RHS cell updates; include warm-up/repeated timings for short runs.

Target the fundamental l=2 scalar value `M*omega = 0.483644 - 0.096759i` (Dolan, Table II): frequency error below 1%, damping-rate error below 3%, supported by the convergence and window checks. These are proposed acceptance targets, not achieved results. Avoid baking the reference into the fitted result.

An AMR speedup is not guaranteed for this small 1D problem. A correct explanation of measured overhead and accuracy is a useful result.

## 6. Report and review — about 8 hours

- [ ] Write a 4–6-page report: question, equations, method, validation, measurements and limitations.
- [ ] Include waveform, convergence, fit-window and accuracy/cost figures.
- [ ] Record the exact application/dependency commits, compiler, build type and input files for each run.
- [ ] Make every reported figure reproducible from saved data and analysis commands.
- [ ] Explain what carries over to full NR and what has not yet been learned.

## Interfaces to introduce as the milestones are reached

Keep a small executable `bh_scatter inputs [key=value ...]`. Use AMReX's geometry/mesh parameter names and the `problem`, `scalar`, `pulse`, `wave`, `diagnostics`, and `tagging` namespaces already illustrated in the input files.

At synchronised output times write:

- `waveform.csv`: `time,observer_x,psi,Pi`.
- `diagnostics.csv`: `time,energy,max_abs_psi,active_cells,rhs_cell_updates,wall_seconds`.
- AMReX plotfiles at the configured stride.
- `metadata.json`: actual parameters, dependency and application revisions, compiler/build configuration and whether the working tree was modified.

Place future outputs beneath ignored `runs/`. Add short analysis scripts only once their corresponding data exist. Keep background calculations, RHS kernels, refinement and diagnostics independently understandable; no general plugin system is needed.

## Later projects

After the core result: test MPI process-count independence, investigate outgoing boundaries or late-time tails, then consider full 3D fixed-background evolution and HPC. GPU backend changes require actual GPU validation. Self-gravitating fields require solving/evolving the spacetime and add constraints, gauge choices and initial-data issues absent here.

