#pragma once

namespace swsbbh {

// Cell-centred POINT values of one angular mode: Phi = psi(t,r) Y_lm / r.
// Pi is partial_t psi, not the canonical momentum used by every NR formulation.
enum Component : int { Psi = 0, Pi = 1, NumComponents = 2 };

// The planned fourth-order centred second derivative needs two neighbours.
inline constexpr int GhostCells = 2;

} // namespace swsbbh

