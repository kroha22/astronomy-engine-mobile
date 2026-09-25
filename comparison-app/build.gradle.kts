plugins {
    id("com.android.application")
    kotlin("android")
}

android {
    namespace = "com.nyra.astronomy.comparison"
    compileSdk = 36

    defaultConfig {
        applicationId = "com.nyra.astronomy.comparison"
        minSdk = 24
        targetSdk = 36
        versionCode = 1
        versionName = "0.1"
        testInstrumentationRunner = "androidx.test.runner.AndroidJUnitRunner"
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
}

kotlin {
    jvmToolchain(17)
}

dependencies {
    implementation(project(":android"))

    // Test application only: the official Kotlin/JVM implementation is never
    // exposed by the mobile wrapper or shipped to downstream applications.
    implementation("io.github.cosinekitty:astronomy:2.1.19")

    androidTestImplementation("androidx.test.ext:junit:1.2.1")
    androidTestImplementation("androidx.test:runner:1.6.2")
}
