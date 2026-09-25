# Astronomy Engine Mobile

An MIT-licensed mobile wrapper around the C edition of
[Astronomy Engine](https://github.com/cosinekitty/astronomy). One C core is
shared by Android and Apple clients so both platforms calculate the same
astronomical values.

This library returns astronomy data only. Human Design gates, the 88-degree
design instant, zodiac signs, houses, aspects, traits, and interpretations
belong to consuming applications.

## Version 0 scope

- apparent geocentric true-ecliptic-of-date longitude for the Sun, Moon,
  Mercury, Venus, Mars, Jupiter, Saturn, Uranus, Neptune, and Pluto;
- mean lunar ascending node;
- osculating lunar ascending node, explicitly labelled as this library's
  `TRUE` convention;
- UTC epoch-millisecond input;
- engine/version/convention metadata and a conservative error bound;
- Android Kotlin API backed by JNI;
- Apple Swift API backed by a Swift Package C target.

Natal-chart houses and house systems are intentionally not approximated here.
They need a separately specified, tested contract. Geographic observer APIs can
be added without introducing astrology-specific interpretation.

## Repository layout

```text
Sources/CAstronomyEngine/       C wrapper and pinned upstream C engine
Sources/AstronomyEngineMobile/  Swift API
android/                        Kotlin/JNI Android library
native-tests/                   portable C contract tests
third_party/                    upstream license and provenance
```

## Apple

Add this repository as a Swift Package and import `AstronomyEngineMobile`.

```swift
let result = try AstronomyEngine.longitude(
    of: .sun,
    at: Date(timeIntervalSince1970: 946_728_000)
)
```

## Android

Publish or include the `:android` Gradle module, then use:

```kotlin
val result = AstronomyEngine.longitude(
    body = AstronomyBody.Sun,
    epochMilliseconds = 946_728_000_000L,
)
```

## Accuracy contract

Astronomy Engine documents planetary accuracy within approximately one
arcminute. This wrapper reports `maximumErrorDegrees = 1/60` for supported
body longitudes and `0.1` degrees for its osculating lunar-node calculation.
These values are bounds for downstream boundary handling, not claims of exact
agreement with another ephemeris or node convention.

## License

The wrapper is MIT licensed. Astronomy Engine is also MIT licensed and retains
its original copyright notice. See `LICENSE`, `THIRD_PARTY_NOTICES.md`, and
`third_party/astronomy-engine/LICENSE`.
