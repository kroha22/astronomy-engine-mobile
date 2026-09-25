package com.nyra.astronomy.comparison

import androidx.test.ext.junit.runners.AndroidJUnit4
import androidx.test.platform.app.InstrumentationRegistry
import com.nyra.astronomy.mobile.AstronomyBody
import com.nyra.astronomy.mobile.AstronomyEngine
import com.nyra.astronomy.mobile.LunarNodeConvention
import io.github.cosinekitty.astronomy.Aberration
import io.github.cosinekitty.astronomy.Body
import io.github.cosinekitty.astronomy.Time
import io.github.cosinekitty.astronomy.equatorialToEcliptic
import io.github.cosinekitty.astronomy.geoVector
import org.json.JSONObject
import org.junit.Assert.assertEquals
import org.junit.Assert.assertTrue
import org.junit.Test
import org.junit.runner.RunWith
import java.time.Instant
import java.time.ZoneOffset

@RunWith(AndroidJUnit4::class)
class AstronomyEngineComparisonTest {
    private val fixture by lazy {
        val context = InstrumentationRegistry.getInstrumentation().context
        JSONObject(context.assets.open("jpl_horizons_j2000.json").bufferedReader().use { it.readText() })
    }

    @Test
    fun mobileWrapperMatchesOfficialKotlinImplementation() {
        val epoch = fixture.getLong("epochMilliseconds")
        val expectedBodies = fixture.getJSONObject("bodyLongitudesDegrees")
        bodies.forEach { (mobileBody, upstreamBody) ->
            val expected = officialApparentLongitude(upstreamBody, epoch)
            val actual = AstronomyEngine.longitude(mobileBody, epoch).degrees
            assertAngularClose("$mobileBody official Kotlin", expected, actual, 1.0e-7)

            val independent = expectedBodies.getDouble(mobileBody.name)
            assertAngularClose("$mobileBody JPL/Horizons", independent, actual, 1.0 / 60.0)
        }
    }

    @Test
    fun lunarNodeApiIsCheckedSeparatelyAgainstIndependentFixture() {
        val epoch = fixture.getLong("epochMilliseconds")
        val actual = AstronomyEngine.lunarAscendingNode(
            LunarNodeConvention.TrueOsculating,
            epoch,
        )
        val independent = fixture.getDouble("trueOsculatingLunarAscendingNodeDegrees")
        assertAngularClose("true osculating lunar node JPL/Horizons", independent, actual.degrees, actual.maximumErrorDegrees)

        val mean = AstronomyEngine.lunarAscendingNode(LunarNodeConvention.Mean, epoch)
        assertTrue("mean node must be normalized", mean.degrees >= 0.0 && mean.degrees < 360.0)
    }

    private fun officialApparentLongitude(body: Body, epochMilliseconds: Long): Double {
        val instant = Instant.ofEpochMilli(epochMilliseconds).atZone(ZoneOffset.UTC)
        val second = instant.second + instant.nano / 1_000_000_000.0
        val time = Time(instant.year, instant.monthValue, instant.dayOfMonth, instant.hour, instant.minute, second)
        return equatorialToEcliptic(geoVector(body, time, Aberration.Corrected)).elon
    }

    private fun assertAngularClose(label: String, expected: Double, actual: Double, tolerance: Double) {
        val difference = kotlin.math.abs((expected - actual + 540.0) % 360.0 - 180.0)
        assertEquals(label, 0.0, difference, tolerance)
    }

    private val bodies = listOf(
        AstronomyBody.Sun to Body.Sun,
        AstronomyBody.Moon to Body.Moon,
        AstronomyBody.Mercury to Body.Mercury,
        AstronomyBody.Venus to Body.Venus,
        AstronomyBody.Mars to Body.Mars,
        AstronomyBody.Jupiter to Body.Jupiter,
        AstronomyBody.Saturn to Body.Saturn,
        AstronomyBody.Uranus to Body.Uranus,
        AstronomyBody.Neptune to Body.Neptune,
        AstronomyBody.Pluto to Body.Pluto,
    )
}
