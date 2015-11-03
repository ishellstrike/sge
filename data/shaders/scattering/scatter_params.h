const float SUN_INTENSITY = 100;

const vec3 earthPos = vec3(0.0, 0.00, 0.0);

// ----------------------------------------------------------------------------
// PHYSICAL MODEL PARAMETERS
// ----------------------------------------------------------------------------

const float SCALE = 0.0000950f;

const float Rg = 6360.0f * SCALE;
const float Rt = 6420.0f * SCALE;
const float RL = 6421.0f * SCALE;

const float AVERAGE_GROUND_REFLECTANCE = 0.1f;

// Rayleigh
const float HR = 18.0f * SCALE;
const vec3 betaR = vec3(5.8e-3f, 1.35e-2f, 3.31e-2f) / SCALE;

// Mie
// DEFAULT
const float HM = 1.2f * SCALE;

const float mieG = 0.8f;
// CLEAR SKY
/*const float HM = 1.2 * SCALE;
const vec3 betaMSca = vec3(20e-3) / SCALE;
const vec3 betaMEx = betaMSca / 0.9;
const float mieG = 0.76;*/
// PARTLY CLOUDY
/*const float HM = 3.0 * SCALE;
const vec3 betaMSca = vec3(3e-3) / SCALE;
const vec3 betaMEx = betaMSca / 0.9;
const float mieG = 0.65;*/


// ----------------------------------------------------------------------------
// NUMERICAL INTEGRATION PARAMETERS
// ----------------------------------------------------------------------------

const int TRANSMITTANCE_INTEGRAL_SAMPLES = 500;
const int INSCATTER_INTEGRAL_SAMPLES = 50;
const int IRRADIANCE_INTEGRAL_SAMPLES = 32;
const int INSCATTER_SPHERICAL_INTEGRAL_SAMPLES = 16;

// ----------------------------------------------------------------------------
// PARAMETERIZATION OPTIONS
// ----------------------------------------------------------------------------

const int TRANSMITTANCE_W = 256;
const int TRANSMITTANCE_H = 64;

const int SKY_W = 64;
const int SKY_H = 16;

const int RES_R = 32;
const int RES_MU = 128;
const int RES_MU_S = 32;
const int RES_NU = 8;
