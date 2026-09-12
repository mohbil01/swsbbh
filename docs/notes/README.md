# Project notes

Short explanations that complement the [theory guide](../theory.pdf) and [learning roadmap](../roadmap.md).

- [What is the wave in the black-hole scattering project?](wave-explanation.pdf) ([LaTeX source](wave-explanation.tex)): scalar fields versus metric perturbations, initial data, adjustable wave properties and ways to visualise propagation.

Keep each PDF beside its LaTeX source and rebuild it after editing. From the repository root, with Tectonic installed:

```sh
tectonic -o docs/notes docs/notes/wave-explanation.tex
```

With a standard TeX installation, run twice:

```sh
pdflatex -interaction=nonstopmode -halt-on-error -output-directory=docs/notes docs/notes/wave-explanation.tex
```
