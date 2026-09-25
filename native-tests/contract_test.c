#include "astronomy_engine_mobile.h"

#include <assert.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

static double angular_difference(double left, double right)
{
    double difference = fmod(left - right + 180.0, 360.0);
    if (difference < 0.0) difference += 360.0;
    return fabs(difference - 180.0);
}

static void assert_longitude(ame_longitude_result_t result)
{
    assert(result.status == AME_SUCCESS);
    assert(isfinite(result.degrees));
    assert(result.degrees >= 0.0);
    assert(result.degrees < 360.0);
    assert(result.maximum_error_degrees > 0.0);
}

int main(void)
{
    assert(strcmp(ame_wrapper_version(), "0.1.0") == 0);
    assert(strstr(ame_upstream_version(), "v2.1.19") != 0);
    assert(strcmp(ame_coordinate_contract(), "apparent_geocentric_true_ecliptic_of_date") == 0);

    const long long j2000 = 946728000000LL;
    const ame_body_t bodies[] = {
        AME_BODY_SUN,
        AME_BODY_MOON,
        AME_BODY_MERCURY,
        AME_BODY_VENUS,
        AME_BODY_MARS,
        AME_BODY_JUPITER,
        AME_BODY_SATURN,
        AME_BODY_URANUS,
        AME_BODY_NEPTUNE,
        AME_BODY_PLUTO,
    };
    const double expected[] = {
        280.3689092,
        223.3237860,
        271.8892699,
        241.5657794,
        327.9632921,
        25.2530685,
        40.3956366,
        314.8091680,
        303.1930007,
        251.4547644,
    };
    ame_longitude_result_t mean_node = ame_lunar_ascending_node(AME_NODE_MEAN, j2000);
    ame_longitude_result_t true_node = ame_lunar_ascending_node(
        AME_NODE_TRUE_OSCULATING,
        j2000
    );

    unsigned long index;
    for (index = 0; index < sizeof(bodies) / sizeof(bodies[0]); ++index) {
        ame_longitude_result_t result = ame_ecliptic_longitude(bodies[index], j2000);
        assert_longitude(result);
        assert(angular_difference(result.degrees, expected[index]) <= result.maximum_error_degrees);
    }
    assert_longitude(mean_node);
    assert_longitude(true_node);

    /* Independently recorded NASA/JPL Horizons DE441 apparent ECT fixtures. */
    double node_difference = angular_difference(true_node.degrees, 123.9580554371928);
    if (node_difference > true_node.maximum_error_degrees) {
        fprintf(
            stderr,
            "true node %.9f differs from fixture by %.9f degrees\n",
            true_node.degrees,
            node_difference
        );
    }
    assert(node_difference <= true_node.maximum_error_degrees);
    return 0;
}
