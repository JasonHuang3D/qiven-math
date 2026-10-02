from __future__ import annotations

"""B7b carrier fixture suite (ADR-0060 D3; P0 repair batch B7b, 2026-10-02).

Pins the four-element carriers of this repository's CI producer surface
(register row math-ci-full; the plan admission and the ci-gate
conclusion enforcement):

  B7b-M1  the plan admission errors carry the four-element law -
          labeled WHAT/WHY/EVIDENCE/NEXT plus the mechanical FIX route
          (typed unknown-unit rejection is never a bare error)
  B7b-M2  the ci-gate conclusion enforcement carries a typed FAIL
          message with the four-element law and the DIAGNOSE route
          (bare [[ ]] assertions taught nothing on failure)
  B7b-M3  the honest typed-skip units stay byte-stable (the [SKIP]
          selector law: a skip validates nothing and says so)
  B7b-M4  the ci.full registration resolves in-tree (workflow ci.yml
          present; no dangling registration for this repository)

Source pins follow the B7a devkit precedent; gh is the transport, the
workflow log text is the carrier. Each case id rides in the failure
message.
"""

import json
import sys
from pathlib import Path

ROOT = Path(__file__).resolve().parents[1]
CHECKS = 0


def check(condition: bool, label: str, detail: str = "") -> None:
    global CHECKS
    CHECKS += 1
    if not condition:
        raise AssertionError(f"[{label}] {detail}" if detail else f"[{label}] assertion failed")


def case_m1() -> None:
    text = (ROOT / ".github" / "workflows" / "ci.yml").read_text(encoding="utf-8")
    check("error: WHAT: the jobs input is malformed" in text, "B7b-M1",
          "empty-request rejection kept (labeled WHAT)")
    check("error: WHAT: unknown validation unit(s):" in text, "B7b-M1",
          "unknown-unit rejection kept (labeled WHAT)")
    check("error: WHY: the plan is the typed admission surface" in text, "B7b-M1",
          "WHY present on admission failures")
    check("error: EVIDENCE: received jobs input string:" in text, "B7b-M1",
          "EVIDENCE present on admission failures")
    check("rule: math/ci-plan" in text, "B7b-M1", "rule token")
    check("error: NEXT action: FIX - re-dispatch with jobs=full" in text, "B7b-M1",
          "empty-request FIX route")
    check("error: NEXT action: FIX - correct the unit name from:" in text, "B7b-M1",
          "unknown-unit FIX route")


def case_m2() -> None:
    text = (ROOT / ".github" / "workflows" / "ci.yml").read_text(encoding="utf-8")
    check("[FAIL] CI Gate: WHAT: a requested validation unit did not succeed"
          in text, "B7b-M2", "typed ci-gate FAIL present (labeled WHAT)")
    check("rule: math/ci-gate" in text, "B7b-M2", "rule token")
    check("EVIDENCE: resolve=${RESOLVE_RESULT}, plan=${PLAN_RESULT}," in text,
          "B7b-M2", "ci-gate FAIL carries labeled EVIDENCE")
    check("NEXT action: DIAGNOSE - open the failed unit's step logs" in text,
          "B7b-M2", "DIAGNOSE route")
    check("without a fix is not a retry" in text, "B7b-M2",
          "anti-reroll law")
    check("Requested validation passed: $SELECTED" in text, "B7b-M2",
          "PASS selector kept")


def case_m3() -> None:
    text = (ROOT / ".github" / "workflows" / "ci.yml").read_text(encoding="utf-8")
    check('echo "[SKIP] unix:' in text, "B7b-M3", "unix typed skip kept")
    check('echo "[SKIP] contracts:' in text, "B7b-M3", "contracts typed skip kept")
    check("this run validates nothing" in text, "B7b-M3", "honest skip law")


def case_m4() -> None:
    config = json.loads((ROOT / ".qiven" / "operator.json").read_text(encoding="utf-8"))
    declared = config.get("ci", {}).get("full", {}).get("workflow")
    check(declared == "ci.yml", "B7b-M4", f"ci.full declares {declared!r}")
    check((ROOT / ".github" / "workflows" / str(declared)).is_file(), "B7b-M4",
          "declared workflow resolves in-tree")


def main() -> int:
    case_m1()
    case_m2()
    case_m3()
    case_m4()
    print(f"[ OK ] math B7b carrier fixtures ({CHECKS} checks)")
    return 0


if __name__ == "__main__":
    try:
        raise SystemExit(main())
    except AssertionError as failure:
        print(f"[FAIL] b7b-carrier-tests: {failure}", file=sys.stderr)
        print("[FAIL] b7b-carrier-tests: WHY: a pinned four-element carrier "
              "selector broke (rule: math/b7b-carriers)", file=sys.stderr)
        print("       NEXT action: FIX - the B7b-Mn id above names the carrier; "
              "restore the four-element law, never the check", file=sys.stderr)
        raise SystemExit(1)
