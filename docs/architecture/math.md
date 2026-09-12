# Math Architecture

`qiven-math` is Qiven's small, deterministic, external-semantics-free spatial math vocabulary. It owns compact algebraic
values, not geometry, units, general numerical linear algebra, CAD entities, robotics policy, physics, or rendering
conventions.

## Scalar policy

The initial public scalar vocabulary is exactly `float` and `double`. It excludes cv-qualified scalars, `long double`, all
integral types, and `bool`. This boundary may broaden only when a concrete units or numeric integration requires it; there is
no generic numeric customization framework in this phase.

Downstream code should normally use `double` for CAD, geometry, robotics, and initially physics. Rendering and GPU boundaries
should convert explicitly to `float`.

## Representation

Canonical spatial values are compact aggregates and do not encode a SIMD ABI. `VecN<T>` occupies exactly `N * sizeof(T)` and
has `alignof(T)`. There is no hidden padding, over-alignment, union overlay, packed representation, or platform SIMD type.
Future performance work belongs in batch, structure-of-arrays, SIMD, or GPU kernels rather than canonical value types.

## Coordinate and algebra conventions

This library does not choose a product or world coordinate system. Up-axis, handedness, and AutoCAD, Unreal Engine, glTF,
USD, or MuJoCo conversions belong at domain, import/export, and scene boundaries.

Future matrices use `M(row, column)` indexing, column vectors, `v' = M * v`, and
`Mworld = Mparent * Mlocal` composition. Quaternions describe active rotations, and cross products use the standard
mathematical right-hand rule. Matrix storage order is separate from these mathematical conventions and remains unspecified in
this batch.

## Equality, tolerance, and contracts

Vector `operator==` is exact component equality. There is no global epsilon or approximate equality operator. Future
approximate APIs must take an explicit tolerance; CAD topology tolerance belongs to the geometry or CAD algorithm context.

Programmer preconditions use Foundation's `QIVEN_ASSERT`. Inputs that violate such preconditions are outside the valid API
contract. Future external-data APIs may add checked, recoverable operations.

## Vec4 meaning

`Vec4` is a four-component algebraic vector. It is not intrinsically a homogeneous point, color, quaternion, plane, or GPU
register; higher layers assign those meanings explicitly.
