package com.nyra.astronomy.mobile

enum class AstronomyBody(internal val nativeCode: Int) {
    Mercury(0),
    Venus(1),
    Mars(2),
    Jupiter(3),
    Saturn(4),
    Uranus(5),
    Neptune(6),
    Pluto(7),
    Sun(8),
    Moon(9),
}

enum class LunarNodeConvention(internal val nativeCode: Int) {
    Mean(0),
    TrueOsculating(1),
}

data class EclipticLongitude(
    val degrees: Double,
    val maximumErrorDegrees: Double,
)

data class AstronomyEngineMetadata(
    val wrapperVersion: String,
    val upstreamVersion: String,
    val coordinateContract: String,
    val trueNodeContract: String,
)

class AstronomyCalculationException(
    val statusCode: Int,
) : IllegalArgumentException("Astronomy calculation failed with status $statusCode")

object AstronomyEngine {
    val wrapperVersion: String get() = NativeBridge.wrapperVersion()
    val upstreamVersion: String get() = NativeBridge.upstreamVersion()
    val metadata: AstronomyEngineMetadata
        get() = AstronomyEngineMetadata(
            wrapperVersion = wrapperVersion,
            upstreamVersion = upstreamVersion,
            coordinateContract = NativeBridge.coordinateContract(),
            trueNodeContract = NativeBridge.trueNodeContract(),
        )

    fun longitude(
        body: AstronomyBody,
        epochMilliseconds: Long,
    ): EclipticLongitude = NativeBridge.longitude(body.nativeCode, epochMilliseconds).toResult()

    fun lunarAscendingNode(
        convention: LunarNodeConvention,
        epochMilliseconds: Long,
    ): EclipticLongitude = NativeBridge.lunarNode(
        convention.nativeCode,
        epochMilliseconds,
    ).toResult()

    private fun DoubleArray.toResult(): EclipticLongitude {
        require(size == 3) { "Native result must contain status, degrees, and error" }
        val status = this[0].toInt()
        if (status != 0) throw AstronomyCalculationException(status)
        return EclipticLongitude(degrees = this[1], maximumErrorDegrees = this[2])
    }
}

internal object NativeBridge {
    init {
        System.loadLibrary("astronomy_engine_mobile")
    }

    external fun wrapperVersion(): String
    external fun upstreamVersion(): String
    external fun coordinateContract(): String
    external fun trueNodeContract(): String
    external fun longitude(bodyCode: Int, epochMilliseconds: Long): DoubleArray
    external fun lunarNode(conventionCode: Int, epochMilliseconds: Long): DoubleArray
}
