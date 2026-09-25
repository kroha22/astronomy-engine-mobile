import CAstronomyEngine
import Foundation

public enum AstronomyBody: Int32, CaseIterable, Sendable {
    case mercury = 0
    case venus
    case mars
    case jupiter
    case saturn
    case uranus
    case neptune
    case pluto
    case sun
    case moon
}

public enum LunarNodeConvention: Int32, Sendable {
    case mean = 0
    case trueOsculating = 1
}

public struct EclipticLongitude: Equatable, Sendable {
    public let degrees: Double
    public let maximumErrorDegrees: Double

    public init(degrees: Double, maximumErrorDegrees: Double) {
        self.degrees = degrees
        self.maximumErrorDegrees = maximumErrorDegrees
    }
}

public struct AstronomyEngineMetadata: Equatable, Sendable {
    public let wrapperVersion: String
    public let upstreamVersion: String
    public let coordinateContract: String
    public let trueNodeContract: String
}

public enum AstronomyEngineError: Int32, Error, Equatable, Sendable {
    case invalidBody = 1
    case invalidTime = 2
    case calculationError = 3
    case invalidArgument = 4
    case unknown = -1
}

public enum AstronomyEngine {
    public static var wrapperVersion: String { String(cString: ame_wrapper_version()) }
    public static var upstreamVersion: String { String(cString: ame_upstream_version()) }
    public static var metadata: AstronomyEngineMetadata {
        AstronomyEngineMetadata(
            wrapperVersion: wrapperVersion,
            upstreamVersion: upstreamVersion,
            coordinateContract: String(cString: ame_coordinate_contract()),
            trueNodeContract: String(cString: ame_true_node_contract())
        )
    }

    public static func longitude(
        of body: AstronomyBody,
        at date: Date
    ) throws -> EclipticLongitude {
        try map(ame_ecliptic_longitude(
            ame_body_t(rawValue: UInt32(body.rawValue)),
            epochMilliseconds(date)
        ))
    }

    public static func lunarAscendingNode(
        convention: LunarNodeConvention,
        at date: Date
    ) throws -> EclipticLongitude {
        try map(ame_lunar_ascending_node(
            ame_node_convention_t(rawValue: UInt32(convention.rawValue)),
            epochMilliseconds(date)
        ))
    }

    private static func epochMilliseconds(_ date: Date) -> Int64 {
        Int64((date.timeIntervalSince1970 * 1_000.0).rounded())
    }

    private static func map(_ result: ame_longitude_result_t) throws -> EclipticLongitude {
        guard result.status == AME_SUCCESS else {
            throw AstronomyEngineError(rawValue: Int32(result.status.rawValue)) ?? .unknown
        }
        return EclipticLongitude(
            degrees: result.degrees,
            maximumErrorDegrees: result.maximum_error_degrees
        )
    }
}
