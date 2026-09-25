# Contributor guide

This repository is a platform wrapper around a pinned upstream astronomy
engine. Keep it reusable and deterministic.

- Keep the public API astronomical. Do not add Human Design gates, astrology
  signs, houses, aspects, traits, predictions, or interpretation copy.
- Implement calculations once in `Sources/CAstronomyEngine`; Android and Apple
  bindings must be thin mappings over that C contract.
- Treat UTC input, coordinate frame, node convention, upstream revision, error
  bounds, and serialized enum values as public compatibility contracts.
- Preserve `third_party/astronomy-engine/LICENSE` and update
  `THIRD_PARTY_NOTICES.md` whenever the vendored engine changes.
- Every calculation change needs an independent fixture and parity coverage.
- Never silently replace an unsupported calculation with an approximation.
- Keep release artifacts reproducible; do not commit local build output.
