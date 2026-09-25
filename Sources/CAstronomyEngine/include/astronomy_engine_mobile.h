#ifndef ASTRONOMY_ENGINE_MOBILE_H
#define ASTRONOMY_ENGINE_MOBILE_H

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    AME_SUCCESS = 0,
    AME_INVALID_BODY = 1,
    AME_INVALID_TIME = 2,
    AME_CALCULATION_ERROR = 3,
    AME_INVALID_ARGUMENT = 4
} ame_status_t;

typedef enum {
    AME_BODY_MERCURY = 0,
    AME_BODY_VENUS = 1,
    AME_BODY_MARS = 2,
    AME_BODY_JUPITER = 3,
    AME_BODY_SATURN = 4,
    AME_BODY_URANUS = 5,
    AME_BODY_NEPTUNE = 6,
    AME_BODY_PLUTO = 7,
    AME_BODY_SUN = 8,
    AME_BODY_MOON = 9
} ame_body_t;

typedef enum {
    AME_NODE_MEAN = 0,
    AME_NODE_TRUE_OSCULATING = 1
} ame_node_convention_t;

typedef struct {
    ame_status_t status;
    double degrees;
    double maximum_error_degrees;
} ame_longitude_result_t;

const char *ame_wrapper_version(void);
const char *ame_upstream_version(void);
const char *ame_coordinate_contract(void);
const char *ame_true_node_contract(void);

ame_longitude_result_t ame_ecliptic_longitude(
    ame_body_t body,
    long long epoch_milliseconds
);

ame_longitude_result_t ame_lunar_ascending_node(
    ame_node_convention_t convention,
    long long epoch_milliseconds
);

#ifdef __cplusplus
}
#endif

#endif
