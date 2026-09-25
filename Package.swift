// swift-tools-version: 5.9
import PackageDescription

let package = Package(
    name: "AstronomyEngineMobile",
    platforms: [
        .iOS(.v13),
        .macOS(.v11),
    ],
    products: [
        .library(name: "AstronomyEngineMobile", targets: ["AstronomyEngineMobile"]),
    ],
    targets: [
        .target(
            name: "CAstronomyEngine",
            path: "Sources/CAstronomyEngine",
            publicHeadersPath: "include",
            cSettings: [.define("ASTRONOMY_ENGINE_NO_CURRENT_TIME")],
            linkerSettings: [.linkedLibrary("m")]
        ),
        .target(
            name: "AstronomyEngineMobile",
            dependencies: ["CAstronomyEngine"]
        ),
        .testTarget(
            name: "AstronomyEngineMobileTests",
            dependencies: ["AstronomyEngineMobile"]
        ),
    ]
)
