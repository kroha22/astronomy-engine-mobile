import Foundation
import Testing
@testable import AstronomyEngineMobile

@Test func exposesPinnedVersions() {
    #expect(AstronomyEngine.wrapperVersion == "0.1.0")
    #expect(AstronomyEngine.upstreamVersion.contains("v2.1.19"))
    #expect(AstronomyEngine.metadata.coordinateContract == "apparent_geocentric_true_ecliptic_of_date")
}

@Test func calculatesNormalizedSunLongitude() throws {
    let date = Date(timeIntervalSince1970: 946_728_000)
    let result = try AstronomyEngine.longitude(of: .sun, at: date)
    #expect((0.0..<360.0).contains(result.degrees))
    #expect(result.maximumErrorDegrees > 0.0)
}
