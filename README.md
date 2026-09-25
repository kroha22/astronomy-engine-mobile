# Astronomy Engine Mobile

One small, deterministic astronomy core for Android and Apple clients.

This repository wraps the C edition of
[Astronomy Engine](https://github.com/cosinekitty/astronomy) behind a stable
mobile API. Both platforms use the same C implementation, so a given UTC
instant produces the same result on Android and Apple.

The project is MIT-licensed. It contains no paid runtime service, account,
network request, or proprietary ephemeris dependency.

## What it provides

- apparent geocentric longitude in the true ecliptic of date for the Sun, Moon,
  Mercury, Venus, Mars, Jupiter, Saturn, Uranus, Neptune, and Pluto;
- mean lunar ascending node;
- an explicitly named osculating lunar ascending-node convention;
- UTC epoch-millisecond input;
- engine, coordinate-frame, node-convention, and error-bound metadata;
- an Android Kotlin API backed by JNI;
- an Apple Swift API backed by a Swift Package C target.

The library returns astronomical measurements. It does not contain Human Design
gates, the 88-degree design calculation, zodiac interpretations, houses,
aspects, traits, predictions, or user-facing conclusions. Those remain in the
consuming application.

Natal-chart houses and house systems are intentionally not approximated in this
first version. Adding them requires a separately specified and tested contract.

## Repository layout

```text
Sources/CAstronomyEngine/       C wrapper and pinned upstream C engine
Sources/AstronomyEngineMobile/  Swift API
android/                        Kotlin/JNI Android library
native-tests/                   portable C contract tests
third_party/                    upstream license and provenance
```

## Use from Apple

Add this repository as a Swift Package and import `AstronomyEngineMobile`:

```swift
let result = try AstronomyEngine.longitude(
    of: .sun,
    at: Date(timeIntervalSince1970: 946_728_000)
)
```

The public metadata makes the coordinate and node conventions explicit:

```swift
let metadata = AstronomyEngine.metadata
```

## Use from Android

Include the `:android` Gradle module, then use:

```kotlin
val result = AstronomyEngine.longitude(
    body = AstronomyBody.Sun,
    epochMilliseconds = 946_728_000_000L,
)
```

## Build and test locally

```sh
cmake -S . -B build/native
cmake --build build/native
ctest --test-dir build/native --output-on-failure

swift test
./gradlew :android:assembleRelease
```

The Android module currently builds the four standard ABIs configured by the
Android Gradle Plugin: `arm64-v8a`, `armeabi-v7a`, `x86`, and `x86_64`.

## Accuracy and conventions

The upstream project documents planetary accuracy of approximately one
arcminute. This wrapper reports `maximumErrorDegrees = 1/60` for supported body
longitudes and `0.1` degrees for its osculating lunar-node calculation. These
are conservative downstream boundary-handling bounds, not a claim of exact
agreement with every ephemeris or node convention.

Consumers must persist the engine and convention metadata with any derived
chart. If a value is close to a gate, line, sign, or other boundary, the
consumer should keep the result unresolved rather than silently choosing a
side.

## Provenance and licensing

The vendored upstream source is pinned to Astronomy Engine `v2.1.19`, commit
`61dc07020aaa6885d2c7f688a4d82beaf6edb9ef`. The upstream source remains
unchanged and retains Don Cross's copyright and MIT notice. See
[`THIRD_PARTY_NOTICES.md`](THIRD_PARTY_NOTICES.md) and
[`third_party/astronomy-engine/LICENSE`](third_party/astronomy-engine/LICENSE).

Our C wrapper, Kotlin API, Swift API, tests, build files, and documentation are
covered by the repository's MIT license in [`LICENSE`](LICENSE).

The license permits commercial use, modification, redistribution, and private
use, subject to preserving the required copyright and license notices. This is
an engineering summary, not legal advice.

## Status

The repository is an initial open-source foundation. The API is usable, but
house systems, device-level integration examples, Maven/CocoaPods release
publishing, and a public issue/discussion policy are intentionally still open.
