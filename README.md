# qiven-math

Qiven's small, deterministic, external-semantics-free spatial math vocabulary.

This library owns small, fixed-size algebraic value types such as `Vec2`, `Vec3`, `Vec4`, `Mat3`, `Mat4`, `Quat`,
`RigidTransform3`, and `AffineTransform3`.

It deliberately does not own geometric primitives (`AABB`, `Ray`, `Plane`, `Triangle`, `Polygon`, `Mesh`, or `BRep`), CAD
entities, physical units and dimensions, dynamic or decomposition-oriented linear algebra, physics, or rendering API
conventions. Those concerns belong to higher or different layers including `qiven-geometry`, `qiven-units`, `qiven-linalg`,
`qiven-cad`, `qiven-robotics`, and `qiven-physics`.

The library currently consumes `qiven-foundation` and requires C++20. Shared engineering conventions are under
`docs/engineering/`; domain decisions are recorded under `docs/architecture/`.
