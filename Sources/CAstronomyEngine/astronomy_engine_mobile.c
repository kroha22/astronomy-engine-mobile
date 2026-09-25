#include "astronomy_engine_mobile.h"
#include "astronomy.h"

#include <math.h>

#define AME_J2000_UNIX_SECONDS 946728000.0
#define AME_SECONDS_PER_DAY 86400.0
#define AME_PLANET_ERROR_DEGREES (1.0 / 60.0)
#define AME_NODE_ERROR_DEGREES 0.1

static double normalize_degrees(double value)
{
    value = fmod(value, 360.0);
    return value < 0.0 ? value + 360.0 : value;
}

static ame_longitude_result_t error_result(ame_status_t status)
{
    ame_longitude_result_t result;
    result.status = status;
    result.degrees = NAN;
    result.maximum_error_degrees = NAN;
    return result;
}

static astro_time_t time_from_epoch_milliseconds(long long epoch_milliseconds)
{
    double unix_seconds = ((double) epoch_milliseconds) / 1000.0;
    return Astronomy_TimeFromDays(
        (unix_seconds - AME_J2000_UNIX_SECONDS) / AME_SECONDS_PER_DAY
    );
}

static astro_body_t upstream_body(ame_body_t body)
{
    switch (body) {
        case AME_BODY_MERCURY: return BODY_MERCURY;
        case AME_BODY_VENUS: return BODY_VENUS;
        case AME_BODY_MARS: return BODY_MARS;
        case AME_BODY_JUPITER: return BODY_JUPITER;
        case AME_BODY_SATURN: return BODY_SATURN;
        case AME_BODY_URANUS: return BODY_URANUS;
        case AME_BODY_NEPTUNE: return BODY_NEPTUNE;
        case AME_BODY_PLUTO: return BODY_PLUTO;
        case AME_BODY_SUN: return BODY_SUN;
        case AME_BODY_MOON: return BODY_MOON;
        default: return BODY_INVALID;
    }
}

const char *ame_wrapper_version(void)
{
    return "0.1.0";
}

const char *ame_upstream_version(void)
{
    return "astronomy-engine-v2.1.19+61dc070";
}

const char *ame_coordinate_contract(void)
{
    return "apparent_geocentric_true_ecliptic_of_date";
}

const char *ame_true_node_contract(void)
{
    return "osculating_ascending_node_from_geocentric_lunar_state_in_ect";
}

ame_longitude_result_t ame_ecliptic_longitude(
    ame_body_t body,
    long long epoch_milliseconds
)
{
    astro_body_t mapped = upstream_body(body);
    if (mapped == BODY_INVALID) {
        return error_result(AME_INVALID_BODY);
    }

    astro_time_t time = time_from_epoch_milliseconds(epoch_milliseconds);
    if (!isfinite(time.ut) || !isfinite(time.tt)) {
        return error_result(AME_INVALID_TIME);
    }

    astro_vector_t geocentric = Astronomy_GeoVector(mapped, time, ABERRATION);
    astro_ecliptic_t upstream = Astronomy_Ecliptic(geocentric);
    if (upstream.status != ASTRO_SUCCESS || !isfinite(upstream.elon)) {
        return error_result(AME_CALCULATION_ERROR);
    }

    ame_longitude_result_t result;
    result.status = AME_SUCCESS;
    result.degrees = normalize_degrees(upstream.elon);
    result.maximum_error_degrees = AME_PLANET_ERROR_DEGREES;
    return result;
}

static ame_longitude_result_t mean_lunar_node(astro_time_t time)
{
    double t = time.tt / 36525.0;
    double t2 = t * t;
    double t3 = t2 * t;
    double t4 = t2 * t2;
    double degrees = 125.0445479
        - 1934.1362891 * t
        + 0.0020754 * t2
        + t3 / 467441.0
        - t4 / 60616000.0;

    ame_longitude_result_t result;
    result.status = AME_SUCCESS;
    result.degrees = normalize_degrees(degrees);
    result.maximum_error_degrees = AME_NODE_ERROR_DEGREES;
    return result;
}

static ame_longitude_result_t osculating_lunar_node(astro_time_t time)
{
    astro_state_vector_t state = Astronomy_GeoMoonState(time);
    if (state.status != ASTRO_SUCCESS) {
        return error_result(AME_CALCULATION_ERROR);
    }

    astro_rotation_t rotation = Astronomy_Rotation_EQJ_ECT(&time);
    if (rotation.status != ASTRO_SUCCESS) {
        return error_result(AME_CALCULATION_ERROR);
    }

    double r[3] = {state.x, state.y, state.z};
    double v[3] = {state.vx, state.vy, state.vz};
    double er[3] = {0.0, 0.0, 0.0};
    double ev[3] = {0.0, 0.0, 0.0};
    int row;
    int column;
    for (row = 0; row < 3; ++row) {
        for (column = 0; column < 3; ++column) {
            er[row] += rotation.rot[column][row] * r[column];
            ev[row] += rotation.rot[column][row] * v[column];
        }
    }

    double hx = er[1] * ev[2] - er[2] * ev[1];
    double hy = er[2] * ev[0] - er[0] * ev[2];
    double node_x = -hy;
    double node_y = hx;
    if (!isfinite(node_x) || !isfinite(node_y) || hypot(node_x, node_y) == 0.0) {
        return error_result(AME_CALCULATION_ERROR);
    }

    ame_longitude_result_t result;
    result.status = AME_SUCCESS;
    result.degrees = normalize_degrees(atan2(node_y, node_x) * 180.0 / 3.14159265358979323846);
    result.maximum_error_degrees = AME_NODE_ERROR_DEGREES;
    return result;
}

ame_longitude_result_t ame_lunar_ascending_node(
    ame_node_convention_t convention,
    long long epoch_milliseconds
)
{
    astro_time_t time = time_from_epoch_milliseconds(epoch_milliseconds);
    if (!isfinite(time.ut) || !isfinite(time.tt)) {
        return error_result(AME_INVALID_TIME);
    }

    switch (convention) {
        case AME_NODE_MEAN:
            return mean_lunar_node(time);
        case AME_NODE_TRUE_OSCULATING:
            return osculating_lunar_node(time);
        default:
            return error_result(AME_INVALID_ARGUMENT);
    }
}
