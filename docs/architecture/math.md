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

## Vector algorithms — Batch 008

The shared vector algorithm surface is deliberately small and dimension-independent where the mathematics permits it:

- `dot` is available for `Vec2`, `Vec3`, and `Vec4` of the same scalar type;
- `cross` is available only for `Vec3` and follows the right-hand-rule convention above;
- `length_squared` is the direct algebraic dot product and therefore may overflow or underflow in ordinary floating-point
  arithmetic;
- `length` uses max-component scaling for finite vectors to avoid avoidable overflow and underflow, returns positive infinity
  if any component is infinite, and returns NaN for NaN input when no infinity is present;
- `normalize` has a programmer precondition that every component is finite and the vector is non-zero;
- `try_normalize` is the checked variant: it returns `false` for zero or non-finite input and leaves the output argument
  unchanged on failure;
- normalization uses max-component scaling before the square root so very large and very small finite vectors remain usable;
- `distance(a, b)` is `length(b - a)` and therefore inherits ordinary floating-point subtraction semantics before robust
  length evaluation;
- `is_near` is componentwise absolute comparison with an explicit finite non-negative tolerance. It has no global epsilon and
  no implicit relative-tolerance policy.

Pure algebraic operations remain `constexpr` where C++20 permits. Square-root-dependent operations are runtime operations in
this phase. Fast-math behavior is outside the contract.
