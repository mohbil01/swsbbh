# swsbbh — scalar-wave scattering by a black hole

Evolve radial modes of a massless scalar field on a fixed Schwarzschild spacetime, recover their ringdown, and investigate numerical accuracy and adaptive refinement.

**Current state:** buildable scaffolding. The executable initialises AMReX and allocates a zeroed, one-dimensional grid. Initial data, the physical potential, evolution, diagnostics and AMR are still implementation exercises. No numerical physics have been generated yet.

Start with the [theory guide](docs/theory.pdf) ([LaTeX source](docs/theory.tex)) and [learning roadmap](docs/roadmap.md). The guide assumes graduate GR but introduces the black-hole and numerical-relativity material needed here.

For a short introduction to what the wave represents, how to specify it and how to see its propagation, read [What is the wave?](docs/notes/wave-explanation.pdf) ([LaTeX source](docs/notes/wave-explanation.tex)). Additional explanatory notes live in [docs/notes](docs/notes/README.md).

## Build and check the scaffold

Requirements: a C++20 compiler, CMake 3.25 or newer, Ninja, and internet access for the first configure. AMReX 26.09 is fetched at a fixed commit into the build directory. The default is serial CPU execution with double precision and no Fortran.

From the repository root:

```sh
cmake --preset debug
cmake --build --preset debug
ctest --preset debug
./build/debug/bh_scatter inputs/flat_wave
./build/debug/bh_scatter inputs/schwarzschild
```

Both runs currently report grid allocation only. The AMR input is a future specification and is deliberately rejected until the AMR milestone is implemented.

Use `cmake --preset release` and `cmake --build --preset release` for performance measurements once evolution exists. Keep Debug for development. Four concurrent compilation jobs are set in the presets to suit a laptop.

For an existing checkout of the pinned dependency, configure with `-DFETCHCONTENT_SOURCE_DIR_AMREX=/absolute/path/to/amrex` to avoid the download. Keep its revision consistent with CMakeLists.txt.

## Project layout

| Location | Purpose |
| --- | --- |
| `src/main.cpp`, `include/swsbbh/State.hpp` | Buildable entry point and field conventions |
| `inputs/` | Flat-wave exercise and planned scattering experiments |
| `docs/theory.tex`, `docs/theory.pdf` | Derivations, numerical methods, worked examples and exercises |
| `docs/notes/` | Short explanations, with LaTeX sources and rendered PDFs |
| `docs/roadmap.md` | Milestones, acceptance checks and interface decisions |
| `tests/README.md` | Physics tests to implement as the solver develops |
| `analysis/` | Analysis environment specification and planned workflow |
| `runs/` | Ignored location for future simulation output |

The solver will use cell-centred **point values**, `psi` and `Pi = partial_t psi`, for one angular mode. The full field is `Phi = psi(t,r) Y_lm / r`. Coordinates and times in the supplied inputs are measured in black-hole mass units, with `M=1`.

## How to work through it

The intended workflow is: derive a result, implement a small part, compare against a known answer, and review before continuing. You write the central solver. Assistant help should normally be explanations, hints, debugging and code review; complete implementations can be requested explicitly.

`SWSBBH_ENABLE_MPI` exposes the dependency build switch for future use; numerical MPI correctness and scaling have not been tested.

## Theory and analysis tools

The PDF is kept beside its standalone LaTeX source. To rebuild with an installed Tectonic compiler:

```sh
tectonic -o docs docs/theory.tex
```

With a standard TeX installation instead:

```sh
pdflatex -interaction=nonstopmode -halt-on-error -output-directory=docs docs/theory.tex
pdflatex -interaction=nonstopmode -halt-on-error -output-directory=docs docs/theory.tex
```

The bibliography is embedded; BibTeX and shell escape are not needed. See [analysis/README.md](analysis/README.md) when numerical output is available. No Python package is needed to build the C++ scaffold.

## Sources

- [AMReX 26.09](https://github.com/AMReX-Codes/amrex/tree/a52ca73324ac2c7b65ec04f131e6df99eec9c576), pinned in CMake. Its source retains its own licence.
- [AMReX wave tutorial](https://github.com/AMReX-Codes/amrex-tutorials/tree/1a73f32e10514d917333ea7a3fd54e661e232144/ExampleCodes/Amr/Wave_AmrLevel), the reference for later `AmrLevel` integration. The tutorial solver has not been copied into this scaffold.
- [Berti, Cardoso and Starinets, *Quasinormal modes of black holes and black branes*](https://arxiv.org/abs/0905.2975).
- [Dolan, *Instability of the massive Klein–Gordon field on the Kerr spacetime*](https://arxiv.org/abs/0705.2880), including the massless, nonrotating reference frequency in Table II.
