# qiven-math

Qiven's small, deterministic, external-semantics-free spatial math vocabulary.

## Current public surface

The public header set currently provides the scalar vocabulary and the
fixed-size vector core: `scalar.hpp`, `Vec2`/`Vec3`/`Vec4` (`vec2.hpp`,
`vec3.hpp`, `vec4.hpp`, plus the `vec.hpp` umbrella) and
`vec_algorithms.hpp`. Matrix types (`Mat3`, `Mat4`), `Quat`,
`RigidTransform3` and `AffineTransform3` are **planned, not provided**:
each is promoted only when a real first consumer's contract is stated
and admitted (ADR-0024 admission; ADR-0013 scope). Earlier README
wording listed them as owned types; that was aspirational, and this
correction records the actual tree.

It deliberately does not own geometric primitives (`AABB`, `Ray`, `Plane`, `Triangle`, `Polygon`, `Mesh`, or `BRep`), CAD
entities, physical units and dimensions, dynamic or decomposition-oriented linear algebra, physics, or rendering API
conventions. Those concerns belong to higher or different layers including `qiven-geometry`, `qiven-units`, `qiven-linalg`,
`qiven-cad`, `qiven-robotics`, and `qiven-physics`.

The library currently consumes `qiven-foundation` and requires C++20.

## Engineering and architecture entry

- Engineering conventions and standards are canonical in the Devkit
  (`JasonHuang3D/qiven-devkit`: `docs/conventions/README.md` and
  `docs/engineering/README.md` there, per ADR-0046; this repository
  carries no local `docs/engineering/` copy — earlier README wording
  pointed at one).
- Domain decisions and the tested scalar/vector contract live under
  `docs/architecture/` (`math.md`; ADR-0013/0014/0015).

## CI claim scope (honesty note)

CI is explicit-dispatch. The Windows unit checks out a
workspace-locked snapshot (see `.github/workflows/ci.yml`'s pinned
revision), not necessarily this repository's current head; the Unix
and contract legs are typed skips on that path. A green CI run
validates the pinned snapshot's configuration — it is not by itself a
validation claim for an arbitrary current candidate head. Local
validation evidence for a candidate is the repository gate at the
exact candidate head.
