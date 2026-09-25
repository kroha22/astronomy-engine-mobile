# Android comparison app

This is a test-only Android application. It puts the official
`io.github.cosinekitty:astronomy:2.1.19` Kotlin/JVM implementation beside the
`AstronomyEngineMobile` JNI wrapper and compares both on the same emulator.
The official dependency is not part of the reusable Android library or of any
production application.

The fixtures in `src/androidTest/assets/jpl_horizons_j2000.json` are recorded
independently from NASA/JPL Horizons. The tests therefore check two separate
properties:

1. our mobile wrapper agrees with the official Kotlin implementation;
2. the wrapper agrees with an independent apparent-ECT fixture.

The lunar ascending-node API has its own test because it is an extension of the
upstream API. The true osculating node is compared against its independent
fixture; the mean-node test additionally checks normalization.

Run on a connected emulator or device:

```sh
./gradlew :comparison-app:connectedDebugAndroidTest
```

The comparison app is deliberately excluded from the published library API.
