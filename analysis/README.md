# Analysis workspace

No simulation data or fitted results exist yet. Implement the analysis as the corresponding solver milestones are completed.

From the repository root, when needed:

```sh
python3 -m venv .venv
.venv/bin/python -m pip install -r analysis/requirements.txt
```

Planned scripts will read CSV files from `runs/`, produce waveform and convergence plots, fit a damped sinusoid over several time windows, and compare accuracy with computational cost. Record the actual installed versions with each final experiment; keep the environment itself out of Git.

For fits, use `psi` measured at a fixed tortoise coordinate. At that observer the full scalar differs by a constant radial/angular factor, so the oscillation frequency is unchanged. Do not treat samples at successive times as independent noisy observations and interpret least-squares covariance as the total physical error. Resolution, boundary placement, contamination by other modes and fitting-window choice need separate checks.

