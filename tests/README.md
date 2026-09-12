# Validation to add while implementing

CTest currently runs one bootstrap integration check: AMReX initialisation and grid allocation. It does **not** test a numerical solver.

Add each check alongside the corresponding milestone:

1. **Analytic periodic wave:** compare both fields against the exact travelling solution after one period; demonstrate fourth-order convergence on three grids.
2. **Background:** test coordinate inversion residuals over horizon-side and far-field coordinates; test positivity and asymptotics of the potential, including the l=0 case.
3. **Pulse direction:** flat-space Gaussian with each momentum sign travels in the expected direction.
4. **Boundary isolation:** compare the default scattering domain with an enlarged domain at identical spacing; differences in the observation window should lie below discretisation error.
5. **Fixed AMR interface:** transmit a smooth pulse through a coarse/fine interface and measure spurious reflected energy or waveform amplitude.
6. **Regridding:** repeat with changing grids; check refinement decreases error and that composite integrals do not double-count covered coarse cells.
7. **Extraction:** test point interpolation against smooth functions and verify that waveform extraction does not reduce the solver's convergence order.
8. **Ringdown study:** compare frequency and damping with the cited scalar reference and quantify fitting-window sensitivity. Keep this longer experiment separate from the fast test suite.

Use independent mathematical answers and convergence studies. Matching two copies of the same stencil is not a correctness test. Define norms, comparison times and tolerances when each numerical check is implemented.

