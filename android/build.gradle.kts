plugins {
    id("com.android.library")
    kotlin("android")
    `maven-publish`
}

group = "com.nyra.astronomy"
version = "0.1.1"

android {
    namespace = "com.nyra.astronomy.mobile"
    compileSdk = 36

    defaultConfig {
        minSdk = 24
        consumerProguardFiles("consumer-rules.pro")
    }

    externalNativeBuild {
        cmake {
            path = file("src/main/cpp/CMakeLists.txt")
            version = "3.22.1"
        }
    }

    publishing {
        singleVariant("release") {
            withSourcesJar()
        }
    }

    compileOptions {
        sourceCompatibility = JavaVersion.VERSION_17
        targetCompatibility = JavaVersion.VERSION_17
    }
}

kotlin {
    jvmToolchain(17)
}

publishing {
    publications {
        register<MavenPublication>("release") {
            groupId = "com.github.kroha22"
            artifactId = "astronomy-engine-mobile"
            version = project.version.toString()
            afterEvaluate {
                from(components["release"])
            }
        }
    }
}
