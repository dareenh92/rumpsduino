//------------------------------------------------------------------------------

//

// Main Program
// Application    : turboimpl_dspc
// Core           : DSP Core
// Purpose
//  - Turbo code implementation on RUMPS401
//  - > calc d, LLR

// ### Interfacing with LMS6002D, RX part ###

#include "main.h"
#include "turbo_rumps_c3.h"
//#include "libdivide_rumps.h"

// packet structure
#define TIMINGPILOT_LEN 16
#define CHUNK_LEN 48
#define CHUNK_NUM 16
#define N_FREQPILOTPAIR 16 // pairs
#define CODEWORD_LEN 768
#define RF_PAYLOADLEN (N_FREQPILOTPAIR<<1) + CODEWORD_LEN

#define IO_CHNLCTRL_HDR 0x1
#define IO_BITS_HDR 0x2
#define IO_LLRACK_HDR 0x3
#define IO_NOVAR_HDR 0x4

#define DSP_LLR_HDR 0x31
#define DSP_BITSACK_HDR 0x32

#define RX_STOP 0xa0
#define RX_DETECT 0xa1
#define RX_TIMING_SYNC 0xa2
#define RX_FREQ_SYNC 0xa3
#define RX_PAYLOAD 0xa4
#define RX_PHASECORR 0xa5
#define RX_DECODE 0xa6
#define RX_SENDUP 0xa7
#define STATE_EMPTY 0xaf

// LED pin number
const unsigned char ledpin = 0;

// Frequency sync known-preamble
const accum freqsync_preamble[2] = { // since I=Q, this saves space
  0.707,   // 1st preamble - 1
  0.707  // 2nd preamble - 1
};

// Mapping constant for sine-cos lookup
const unsigned long accum sineMappingConst = 0.0012437;
// const unsigned long accum sineMappingConst = 0.0024875;
// const unsigned long accum sineMappingConst = 0.0049751;

// Sine look up table

const accum sineTable[256] = {
  0.0000, 0.0245, 0.0491, 0.0736, 0.0980, 0.1224, 0.1467, 0.1710,
  0.1951, 0.2191, 0.2430, 0.2667, 0.2903, 0.3137, 0.3369, 0.3599,
  0.3827, 0.4052, 0.4276, 0.4496, 0.4714, 0.4929, 0.5141, 0.5350,
  0.5556, 0.5758, 0.5957, 0.6152, 0.6344, 0.6532, 0.6716, 0.6895,
  0.7071, 0.7242, 0.7410, 0.7572, 0.7730, 0.7883, 0.8032, 0.8176,
  0.8315, 0.8449, 0.8577, 0.8701, 0.8819, 0.8932, 0.9040, 0.9142,
  0.9239, 0.9330, 0.9415, 0.9495, 0.9569, 0.9638, 0.9700, 0.9757,
  0.9808, 0.9853, 0.9892, 0.9925, 0.9952, 0.9973, 0.9988, 0.9997,
  1.0000, 0.9997, 0.9988, 0.9973, 0.9952, 0.9925, 0.9892, 0.9853,
  0.9808, 0.9757, 0.9700, 0.9638, 0.9569, 0.9495, 0.9415, 0.9330,
  0.9239, 0.9142, 0.9040, 0.8932, 0.8819, 0.8701, 0.8577, 0.8449,
  0.8315, 0.8176, 0.8032, 0.7883, 0.7730, 0.7572, 0.7410, 0.7242,
  0.7071, 0.6895, 0.6716, 0.6532, 0.6344, 0.6152, 0.5957, 0.5758,
  0.5556, 0.5350, 0.5141, 0.4929, 0.4714, 0.4496, 0.4276, 0.4052,
  0.3827, 0.3599, 0.3369, 0.3137, 0.2903, 0.2667, 0.2430, 0.2191,
  0.1951, 0.1710, 0.1467, 0.1224, 0.0980, 0.0736, 0.0491, 0.0245,
  0.0000, -0.0245, -0.0491, -0.0736, -0.0980, -0.1224, -0.1467, -0.1710,
  -0.1951, -0.2191, -0.2430, -0.2667, -0.2903, -0.3137, -0.3369, -0.3599,
  -0.3827, -0.4052, -0.4276, -0.4496, -0.4714, -0.4929, -0.5141, -0.5350,
  -0.5556, -0.5758, -0.5957, -0.6152, -0.6344, -0.6532, -0.6716, -0.6895,
  -0.7071, -0.7242, -0.7410, -0.7572, -0.7730, -0.7883, -0.8032, -0.8176,
  -0.8315, -0.8449, -0.8577, -0.8701, -0.8819, -0.8932, -0.9040, -0.9142,
  -0.9239, -0.9330, -0.9415, -0.9495, -0.9569, -0.9638, -0.9700, -0.9757,
  -0.9808, -0.9853, -0.9892, -0.9925, -0.9952, -0.9973, -0.9988, -0.9997,
  -1.0000, -0.9997, -0.9988, -0.9973, -0.9952, -0.9925, -0.9892, -0.9853,
  -0.9808, -0.9757, -0.9700, -0.9638, -0.9569, -0.9495, -0.9415, -0.9330,
  -0.9239, -0.9142, -0.9040, -0.8932, -0.8819, -0.8701, -0.8577, -0.8449,
  -0.8315, -0.8176, -0.8032, -0.7883, -0.7730, -0.7572, -0.7410, -0.7242,
  -0.7071, -0.6895, -0.6716, -0.6532, -0.6344, -0.6152, -0.5957, -0.5758,
  -0.5556, -0.5350, -0.5141, -0.4929, -0.4714, -0.4496, -0.4276, -0.4052,
  -0.3827, -0.3599, -0.3369, -0.3137, -0.2903, -0.2667, -0.2430, -0.2191,
  -0.1951, -0.1710, -0.1467, -0.1224, -0.0980, -0.0736, -0.0491, -0.0245
};


/*
static const accum sineTable[512] = {
  0.0000,0.0123,0.0245,0.0368,0.0491,0.0613,0.0736,0.0858,
  0.0980,0.1102,0.1224,0.1346,0.1467,0.1589,0.1710,0.1830,
  0.1951,0.2071,0.2191,0.2311,0.2430,0.2549,0.2667,0.2785,
  0.2903,0.3020,0.3137,0.3253,0.3369,0.3484,0.3599,0.3713,
  0.3827,0.3940,0.4052,0.4164,0.4276,0.4386,0.4496,0.4605,
  0.4714,0.4822,0.4929,0.5035,0.5141,0.5246,0.5350,0.5453,
  0.5556,0.5657,0.5758,0.5858,0.5957,0.6055,0.6152,0.6249,
  0.6344,0.6438,0.6532,0.6624,0.6716,0.6806,0.6895,0.6984,
  0.7071,0.7157,0.7242,0.7327,0.7410,0.7491,0.7572,0.7652,
  0.7730,0.7807,0.7883,0.7958,0.8032,0.8105,0.8176,0.8246,
  0.8315,0.8382,0.8449,0.8514,0.8577,0.8640,0.8701,0.8761,
  0.8819,0.8876,0.8932,0.8987,0.9040,0.9092,0.9142,0.9191,
  0.9239,0.9285,0.9330,0.9373,0.9415,0.9456,0.9495,0.9533,
  0.9569,0.9604,0.9638,0.9670,0.9700,0.9729,0.9757,0.9783,
  0.9808,0.9831,0.9853,0.9873,0.9892,0.9909,0.9925,0.9939,
  0.9952,0.9963,0.9973,0.9981,0.9988,0.9993,0.9997,0.9999,
  1.0000,0.9999,0.9997,0.9993,0.9988,0.9981,0.9973,0.9963,
  0.9952,0.9939,0.9925,0.9909,0.9892,0.9873,0.9853,0.9831,
  0.9808,0.9783,0.9757,0.9729,0.9700,0.9670,0.9638,0.9604,
  0.9569,0.9533,0.9495,0.9456,0.9415,0.9373,0.9330,0.9285,
  0.9239,0.9191,0.9142,0.9092,0.9040,0.8987,0.8932,0.8876,
  0.8819,0.8761,0.8701,0.8640,0.8577,0.8514,0.8449,0.8382,
  0.8315,0.8246,0.8176,0.8105,0.8032,0.7958,0.7883,0.7807,
  0.7730,0.7652,0.7572,0.7491,0.7410,0.7327,0.7242,0.7157,
  0.7071,0.6984,0.6895,0.6806,0.6716,0.6624,0.6532,0.6438,
  0.6344,0.6249,0.6152,0.6055,0.5957,0.5858,0.5758,0.5657,
  0.5556,0.5453,0.5350,0.5246,0.5141,0.5035,0.4929,0.4822,
  0.4714,0.4605,0.4496,0.4386,0.4276,0.4164,0.4052,0.3940,
  0.3827,0.3713,0.3599,0.3484,0.3369,0.3253,0.3137,0.3020,
  0.2903,0.2785,0.2667,0.2549,0.2430,0.2311,0.2191,0.2071,
  0.1951,0.1830,0.1710,0.1589,0.1467,0.1346,0.1224,0.1102,
  0.0980,0.0858,0.0736,0.0613,0.0491,0.0368,0.0245,0.0123,
  0.0000,-0.0123,-0.0245,-0.0368,-0.0491,-0.0613,-0.0736,-0.0858,
  -0.0980,-0.1102,-0.1224,-0.1346,-0.1467,-0.1589,-0.1710,-0.1830,
  -0.1951,-0.2071,-0.2191,-0.2311,-0.2430,-0.2549,-0.2667,-0.2785,
  -0.2903,-0.3020,-0.3137,-0.3253,-0.3369,-0.3484,-0.3599,-0.3713,
  -0.3827,-0.3940,-0.4052,-0.4164,-0.4276,-0.4386,-0.4496,-0.4605,
  -0.4714,-0.4822,-0.4929,-0.5035,-0.5141,-0.5246,-0.5350,-0.5453,
  -0.5556,-0.5657,-0.5758,-0.5858,-0.5957,-0.6055,-0.6152,-0.6249,
  -0.6344,-0.6438,-0.6532,-0.6624,-0.6716,-0.6806,-0.6895,-0.6984,
  -0.7071,-0.7157,-0.7242,-0.7327,-0.7410,-0.7491,-0.7572,-0.7652,
  -0.7730,-0.7807,-0.7883,-0.7958,-0.8032,-0.8105,-0.8176,-0.8246,
  -0.8315,-0.8382,-0.8449,-0.8514,-0.8577,-0.8640,-0.8701,-0.8761,
  -0.8819,-0.8876,-0.8932,-0.8987,-0.9040,-0.9092,-0.9142,-0.9191,
  -0.9239,-0.9285,-0.9330,-0.9373,-0.9415,-0.9456,-0.9495,-0.9533,
  -0.9569,-0.9604,-0.9638,-0.9670,-0.9700,-0.9729,-0.9757,-0.9783,
  -0.9808,-0.9831,-0.9853,-0.9873,-0.9892,-0.9909,-0.9925,-0.9939,
  -0.9952,-0.9963,-0.9973,-0.9981,-0.9988,-0.9993,-0.9997,-0.9999,
  -1.0000,-0.9999,-0.9997,-0.9993,-0.9988,-0.9981,-0.9973,-0.9963,
  -0.9952,-0.9939,-0.9925,-0.9909,-0.9892,-0.9873,-0.9853,-0.9831,
  -0.9808,-0.9783,-0.9757,-0.9729,-0.9700,-0.9670,-0.9638,-0.9604,
  -0.9569,-0.9533,-0.9495,-0.9456,-0.9415,-0.9373,-0.9330,-0.9285,
  -0.9239,-0.9191,-0.9142,-0.9092,-0.9040,-0.8987,-0.8932,-0.8876,
  -0.8819,-0.8761,-0.8701,-0.8640,-0.8577,-0.8514,-0.8449,-0.8382,
  -0.8315,-0.8246,-0.8176,-0.8105,-0.8032,-0.7958,-0.7883,-0.7807,
  -0.7730,-0.7652,-0.7572,-0.7491,-0.7410,-0.7327,-0.7242,-0.7157,
  -0.7071,-0.6984,-0.6895,-0.6806,-0.6716,-0.6624,-0.6532,-0.6438,
  -0.6344,-0.6249,-0.6152,-0.6055,-0.5957,-0.5858,-0.5758,-0.5657,
  -0.5556,-0.5453,-0.5350,-0.5246,-0.5141,-0.5035,-0.4929,-0.4822,
  -0.4714,-0.4605,-0.4496,-0.4386,-0.4276,-0.4164,-0.4052,-0.3940,
  -0.3827,-0.3713,-0.3599,-0.3484,-0.3369,-0.3253,-0.3137,-0.3020,
  -0.2903,-0.2785,-0.2667,-0.2549,-0.2430,-0.2311,-0.2191,-0.2071,
  -0.1951,-0.1830,-0.1710,-0.1589,-0.1467,-0.1346,-0.1224,-0.1102,
  -0.0980,-0.0858,-0.0736,-0.0613,-0.0491,-0.0368,-0.0245,-0.0123,
};
*/

/*
static const accum sineTable[1024] = {
  0.0000,0.0061,0.0123,0.0184,0.0245,0.0307,0.0368,0.0429,
  0.0491,0.0552,0.0613,0.0674,0.0736,0.0797,0.0858,0.0919,
  0.0980,0.1041,0.1102,0.1163,0.1224,0.1285,0.1346,0.1407,
  0.1467,0.1528,0.1589,0.1649,0.1710,0.1770,0.1830,0.1891,
  0.1951,0.2011,0.2071,0.2131,0.2191,0.2251,0.2311,0.2370,
  0.2430,0.2489,0.2549,0.2608,0.2667,0.2726,0.2785,0.2844,
  0.2903,0.2962,0.3020,0.3078,0.3137,0.3195,0.3253,0.3311,
  0.3369,0.3427,0.3484,0.3542,0.3599,0.3656,0.3713,0.3770,
  0.3827,0.3883,0.3940,0.3996,0.4052,0.4108,0.4164,0.4220,
  0.4276,0.4331,0.4386,0.4441,0.4496,0.4551,0.4605,0.4660,
  0.4714,0.4768,0.4822,0.4876,0.4929,0.4982,0.5035,0.5088,
  0.5141,0.5194,0.5246,0.5298,0.5350,0.5402,0.5453,0.5505,
  0.5556,0.5607,0.5657,0.5708,0.5758,0.5808,0.5858,0.5908,
  0.5957,0.6006,0.6055,0.6104,0.6152,0.6201,0.6249,0.6296,
  0.6344,0.6391,0.6438,0.6485,0.6532,0.6578,0.6624,0.6670,
  0.6716,0.6761,0.6806,0.6851,0.6895,0.6940,0.6984,0.7028,
  0.7071,0.7114,0.7157,0.7200,0.7242,0.7285,0.7327,0.7368,
  0.7410,0.7451,0.7491,0.7532,0.7572,0.7612,0.7652,0.7691,
  0.7730,0.7769,0.7807,0.7846,0.7883,0.7921,0.7958,0.7995,
  0.8032,0.8068,0.8105,0.8140,0.8176,0.8211,0.8246,0.8280,
  0.8315,0.8349,0.8382,0.8416,0.8449,0.8481,0.8514,0.8546,
  0.8577,0.8609,0.8640,0.8670,0.8701,0.8731,0.8761,0.8790,
  0.8819,0.8848,0.8876,0.8904,0.8932,0.8960,0.8987,0.9013,
  0.9040,0.9066,0.9092,0.9117,0.9142,0.9167,0.9191,0.9215,
  0.9239,0.9262,0.9285,0.9308,0.9330,0.9352,0.9373,0.9395,
  0.9415,0.9436,0.9456,0.9476,0.9495,0.9514,0.9533,0.9551,
  0.9569,0.9587,0.9604,0.9621,0.9638,0.9654,0.9670,0.9685,
  0.9700,0.9715,0.9729,0.9743,0.9757,0.9770,0.9783,0.9796,
  0.9808,0.9820,0.9831,0.9842,0.9853,0.9863,0.9873,0.9883,
  0.9892,0.9901,0.9909,0.9917,0.9925,0.9932,0.9939,0.9946,
  0.9952,0.9958,0.9963,0.9968,0.9973,0.9977,0.9981,0.9985,
  0.9988,0.9991,0.9993,0.9995,0.9997,0.9998,0.9999,1.0000,
  1.0000,1.0000,0.9999,0.9998,0.9997,0.9995,0.9993,0.9991,
  0.9988,0.9985,0.9981,0.9977,0.9973,0.9968,0.9963,0.9958,
  0.9952,0.9946,0.9939,0.9932,0.9925,0.9917,0.9909,0.9901,
  0.9892,0.9883,0.9873,0.9863,0.9853,0.9842,0.9831,0.9820,
  0.9808,0.9796,0.9783,0.9770,0.9757,0.9743,0.9729,0.9715,
  0.9700,0.9685,0.9670,0.9654,0.9638,0.9621,0.9604,0.9587,
  0.9569,0.9551,0.9533,0.9514,0.9495,0.9476,0.9456,0.9436,
  0.9415,0.9395,0.9373,0.9352,0.9330,0.9308,0.9285,0.9262,
  0.9239,0.9215,0.9191,0.9167,0.9142,0.9117,0.9092,0.9066,
  0.9040,0.9013,0.8987,0.8960,0.8932,0.8904,0.8876,0.8848,
  0.8819,0.8790,0.8761,0.8731,0.8701,0.8670,0.8640,0.8609,
  0.8577,0.8546,0.8514,0.8481,0.8449,0.8416,0.8382,0.8349,
  0.8315,0.8280,0.8246,0.8211,0.8176,0.8140,0.8105,0.8068,
  0.8032,0.7995,0.7958,0.7921,0.7883,0.7846,0.7807,0.7769,
  0.7730,0.7691,0.7652,0.7612,0.7572,0.7532,0.7491,0.7451,
  0.7410,0.7368,0.7327,0.7285,0.7242,0.7200,0.7157,0.7114,
  0.7071,0.7028,0.6984,0.6940,0.6895,0.6851,0.6806,0.6761,
  0.6716,0.6670,0.6624,0.6578,0.6532,0.6485,0.6438,0.6391,
  0.6344,0.6296,0.6249,0.6201,0.6152,0.6104,0.6055,0.6006,
  0.5957,0.5908,0.5858,0.5808,0.5758,0.5708,0.5657,0.5607,
  0.5556,0.5505,0.5453,0.5402,0.5350,0.5298,0.5246,0.5194,
  0.5141,0.5088,0.5035,0.4982,0.4929,0.4876,0.4822,0.4768,
  0.4714,0.4660,0.4605,0.4551,0.4496,0.4441,0.4386,0.4331,
  0.4276,0.4220,0.4164,0.4108,0.4052,0.3996,0.3940,0.3883,
  0.3827,0.3770,0.3713,0.3656,0.3599,0.3542,0.3484,0.3427,
  0.3369,0.3311,0.3253,0.3195,0.3137,0.3078,0.3020,0.2962,
  0.2903,0.2844,0.2785,0.2726,0.2667,0.2608,0.2549,0.2489,
  0.2430,0.2370,0.2311,0.2251,0.2191,0.2131,0.2071,0.2011,
  0.1951,0.1891,0.1830,0.1770,0.1710,0.1649,0.1589,0.1528,
  0.1467,0.1407,0.1346,0.1285,0.1224,0.1163,0.1102,0.1041,
  0.0980,0.0919,0.0858,0.0797,0.0736,0.0674,0.0613,0.0552,
  0.0491,0.0429,0.0368,0.0307,0.0245,0.0184,0.0123,0.0061,
  0.0000,-0.0061,-0.0123,-0.0184,-0.0245,-0.0307,-0.0368,-0.0429,
  -0.0491,-0.0552,-0.0613,-0.0674,-0.0736,-0.0797,-0.0858,-0.0919,
  -0.0980,-0.1041,-0.1102,-0.1163,-0.1224,-0.1285,-0.1346,-0.1407,
  -0.1467,-0.1528,-0.1589,-0.1649,-0.1710,-0.1770,-0.1830,-0.1891,
  -0.1951,-0.2011,-0.2071,-0.2131,-0.2191,-0.2251,-0.2311,-0.2370,
  -0.2430,-0.2489,-0.2549,-0.2608,-0.2667,-0.2726,-0.2785,-0.2844,
  -0.2903,-0.2962,-0.3020,-0.3078,-0.3137,-0.3195,-0.3253,-0.3311,
  -0.3369,-0.3427,-0.3484,-0.3542,-0.3599,-0.3656,-0.3713,-0.3770,
  -0.3827,-0.3883,-0.3940,-0.3996,-0.4052,-0.4108,-0.4164,-0.4220,
  -0.4276,-0.4331,-0.4386,-0.4441,-0.4496,-0.4551,-0.4605,-0.4660,
  -0.4714,-0.4768,-0.4822,-0.4876,-0.4929,-0.4982,-0.5035,-0.5088,
  -0.5141,-0.5194,-0.5246,-0.5298,-0.5350,-0.5402,-0.5453,-0.5505,
  -0.5556,-0.5607,-0.5657,-0.5708,-0.5758,-0.5808,-0.5858,-0.5908,
  -0.5957,-0.6006,-0.6055,-0.6104,-0.6152,-0.6201,-0.6249,-0.6296,
  -0.6344,-0.6391,-0.6438,-0.6485,-0.6532,-0.6578,-0.6624,-0.6670,
  -0.6716,-0.6761,-0.6806,-0.6851,-0.6895,-0.6940,-0.6984,-0.7028,
  -0.7071,-0.7114,-0.7157,-0.7200,-0.7242,-0.7285,-0.7327,-0.7368,
  -0.7410,-0.7451,-0.7491,-0.7532,-0.7572,-0.7612,-0.7652,-0.7691,
  -0.7730,-0.7769,-0.7807,-0.7846,-0.7883,-0.7921,-0.7958,-0.7995,
  -0.8032,-0.8068,-0.8105,-0.8140,-0.8176,-0.8211,-0.8246,-0.8280,
  -0.8315,-0.8349,-0.8382,-0.8416,-0.8449,-0.8481,-0.8514,-0.8546,
  -0.8577,-0.8609,-0.8640,-0.8670,-0.8701,-0.8731,-0.8761,-0.8790,
  -0.8819,-0.8848,-0.8876,-0.8904,-0.8932,-0.8960,-0.8987,-0.9013,
  -0.9040,-0.9066,-0.9092,-0.9117,-0.9142,-0.9167,-0.9191,-0.9215,
  -0.9239,-0.9262,-0.9285,-0.9308,-0.9330,-0.9352,-0.9373,-0.9395,
  -0.9415,-0.9436,-0.9456,-0.9476,-0.9495,-0.9514,-0.9533,-0.9551,
  -0.9569,-0.9587,-0.9604,-0.9621,-0.9638,-0.9654,-0.9670,-0.9685,
  -0.9700,-0.9715,-0.9729,-0.9743,-0.9757,-0.9770,-0.9783,-0.9796,
  -0.9808,-0.9820,-0.9831,-0.9842,-0.9853,-0.9863,-0.9873,-0.9883,
  -0.9892,-0.9901,-0.9909,-0.9917,-0.9925,-0.9932,-0.9939,-0.9946,
  -0.9952,-0.9958,-0.9963,-0.9968,-0.9973,-0.9977,-0.9981,-0.9985,
  -0.9988,-0.9991,-0.9993,-0.9995,-0.9997,-0.9998,-0.9999,-1.0000,
  -1.0000,-1.0000,-0.9999,-0.9998,-0.9997,-0.9995,-0.9993,-0.9991,
  -0.9988,-0.9985,-0.9981,-0.9977,-0.9973,-0.9968,-0.9963,-0.9958,
  -0.9952,-0.9946,-0.9939,-0.9932,-0.9925,-0.9917,-0.9909,-0.9901,
  -0.9892,-0.9883,-0.9873,-0.9863,-0.9853,-0.9842,-0.9831,-0.9820,
  -0.9808,-0.9796,-0.9783,-0.9770,-0.9757,-0.9743,-0.9729,-0.9715,
  -0.9700,-0.9685,-0.9670,-0.9654,-0.9638,-0.9621,-0.9604,-0.9587,
  -0.9569,-0.9551,-0.9533,-0.9514,-0.9495,-0.9476,-0.9456,-0.9436,
  -0.9415,-0.9395,-0.9373,-0.9352,-0.9330,-0.9308,-0.9285,-0.9262,
  -0.9239,-0.9215,-0.9191,-0.9167,-0.9142,-0.9117,-0.9092,-0.9066,
  -0.9040,-0.9013,-0.8987,-0.8960,-0.8932,-0.8904,-0.8876,-0.8848,
  -0.8819,-0.8790,-0.8761,-0.8731,-0.8701,-0.8670,-0.8640,-0.8609,
  -0.8577,-0.8546,-0.8514,-0.8481,-0.8449,-0.8416,-0.8382,-0.8349,
  -0.8315,-0.8280,-0.8246,-0.8211,-0.8176,-0.8140,-0.8105,-0.8068,
  -0.8032,-0.7995,-0.7958,-0.7921,-0.7883,-0.7846,-0.7807,-0.7769,
  -0.7730,-0.7691,-0.7652,-0.7612,-0.7572,-0.7532,-0.7491,-0.7451,
  -0.7410,-0.7368,-0.7327,-0.7285,-0.7242,-0.7200,-0.7157,-0.7114,
  -0.7071,-0.7028,-0.6984,-0.6940,-0.6895,-0.6851,-0.6806,-0.6761,
  -0.6716,-0.6670,-0.6624,-0.6578,-0.6532,-0.6485,-0.6438,-0.6391,
  -0.6344,-0.6296,-0.6249,-0.6201,-0.6152,-0.6104,-0.6055,-0.6006,
  -0.5957,-0.5908,-0.5858,-0.5808,-0.5758,-0.5708,-0.5657,-0.5607,
  -0.5556,-0.5505,-0.5453,-0.5402,-0.5350,-0.5298,-0.5246,-0.5194,
  -0.5141,-0.5088,-0.5035,-0.4982,-0.4929,-0.4876,-0.4822,-0.4768,
  -0.4714,-0.4660,-0.4605,-0.4551,-0.4496,-0.4441,-0.4386,-0.4331,
  -0.4276,-0.4220,-0.4164,-0.4108,-0.4052,-0.3996,-0.3940,-0.3883,
  -0.3827,-0.3770,-0.3713,-0.3656,-0.3599,-0.3542,-0.3484,-0.3427,
  -0.3369,-0.3311,-0.3253,-0.3195,-0.3137,-0.3078,-0.3020,-0.2962,
  -0.2903,-0.2844,-0.2785,-0.2726,-0.2667,-0.2608,-0.2549,-0.2489,
  -0.2430,-0.2370,-0.2311,-0.2251,-0.2191,-0.2131,-0.2071,-0.2011,
  -0.1951,-0.1891,-0.1830,-0.1770,-0.1710,-0.1649,-0.1589,-0.1528,
  -0.1467,-0.1407,-0.1346,-0.1285,-0.1224,-0.1163,-0.1102,-0.1041,
  -0.0980,-0.0919,-0.0858,-0.0797,-0.0736,-0.0674,-0.0613,-0.0552,
  -0.0491,-0.0429,-0.0368,-0.0307,-0.0245,-0.0184,-0.0123,-0.0061,
};
*/

// sine lookup table, for details look at note
// RUMPS and n1169 extension specific
accum sin_lookup(int inAngle){
  unsigned int idx = 0;
  int angleSign = sign_f(inAngle);
  
  // Filter input angle
  if(angleSign<0) // sin(-x) = -sin(x)
    inAngle = (~inAngle)+1;

  // COMPILER_ERROR - assuming input won't exceed 2pi
  // let's discard this operation for now
  while(inAngle>205887) // sin(x) = sin(x+2pi)
    inAngle-=205887;
  
  // Index mapping - inAngle to LUT index
  // simply want integer part of inAngle * mappingConst
  // implementation is a bit complicated due to 64bit mul
  ulaccum_int_t mappingInput,
                mappingConst;
  mappingInput.ulaccum_cont = inAngle;          // A
  mappingConst.ulaccum_cont = sineMappingConst; // B

  //  split a and b into 32 bits halves
  uint32_t a_lo = (uint32_t) mappingInput.uint_cont;
  uint32_t a_hi = mappingInput.uint_cont >> 32;
  uint32_t b_lo = (uint32_t) mappingConst.uint_cont;
  uint32_t b_hi = mappingConst.uint_cont >> 32;

  // * splitting calculations
  uint64_t p0 = mac_umul_32(a_lo, b_lo);
  uint64_t p1 = mac_umul_32(a_lo, b_hi);
  uint64_t p2 = mac_umul_32(a_hi, b_lo);
  uint64_t p3 = mac_umul_32(a_hi, b_hi);

  // * carry from lower half MUL
  uint32_t cy = (uint32_t)(((p0 >> 32) + (uint32_t)p1 + (uint32_t)p2) >> 32);

  // final MUL result - we only take hi (S31.32)
  // uint32_t lo = p0 + (p1 << 32) + (p2 << 32);
  uint32_t hi = p3 + (p1 >> 32) + (p2 >> 32) + cy;
  // if(lo>0x80000000) // >0.5
  //   hi++;
  idx = hi & 255;

  // Return lookup value
  // COMPILER_ERROR - if both modification and return happen
  // accum_int_t returnVal;
  // returnVal.accum_cont = sineTable[idx];  
  // if(angleSign<0) // sin(-x) = -sin(x)
  //   returnVal.int_cont = (~returnVal.int_cont) + 1;
  // return returnVal.accum_cont; 

  return (angleSign * sineTable[idx]);
}

// cosine lookup table, for details look at note
// RUMPS and n1169 extension specific
accum cos_lookup(int inAngle){
  // Filter input angle
  if(sign_f(inAngle)<0) // cos(-x) = cos(x)
    inAngle = (~inAngle)+1;

  // Return lookup value
  // cos(x) = sin(x+pi/2)
  inAngle += 51472; // 51472 is pi/2 in accum format
  return sin_lookup(inAngle);
}

//------------------------------------------------------------------------------

int main(void)

{
  set_trellis();
  

  // *** Part1 - Setup ***
  // MUX - select TM_COM0 & TM_COM1
  MUXC_SELECT = 0x4;

  pinMode_output(ledpin);
  digitalWrite_high(ledpin);
  
  // Define variables
  int Idata, Qdata;
  volatile uint8_t rx_state = STATE_EMPTY;
  
  // Buffer for TED-sign version
  int Isamp[3]; // 0-prev, 1-mid, 2-curr
  int Qsamp[3];
  
  // Counters and flags
  unsigned int nSamples = 0; // general counter
  unsigned int nRecv = 0; // tracks phase-corrected bits
  accum ted_total;
  int tempcalc = 0;
  int ted = 0;
  int stepCorrection = 0;

  // General calculation buffer
  accum_int_t temp_accumint;
  int32_t mulOpA_s32, mulOpB_s32;
  int64_t mulResult_s64;
  int tempdump[20];
  
  // Buffer for Freq sync
  int freqsyncBuff[2][2]; // freq pilot pair
  accum_int_t iBuff, qBuff;
  accum_int_t sigNorm;
  volatile accum_int_t sinBuff, cosBuff;
  accum_int_t iCorrected, qCorrected;
  accum_int_t phaseOffset;
  accum_int_t planeCorr; // plane and random starting phase offset
  accum_int_t relativeCorr; // relative phase offset
  accum_int_t tempCorr;

  // Turbo decoding variables
  short iorecv_i = 0; //count bits received from IO core
  short retllr_i = 0; //count returned LLR calculation
  short delta_i = 0; //count sent delta
  char alpha_i = 0; //count received alpha (increment)
  char beta_i = 31; //count received beta (decrement)
  
  accum iorecv_pbit[(r_win<<1)]; //store parity bit, for Le (2*windows)
  accum local_deltabuff[(r_win<<1)][16]; //local delta buffer for 2*windows
  accum local_alphabuff[r_win][8];
  accum local_betabuff[r_win][8];
  
  accum temp_sysbit;
  accum temp_ln_ap1;
  accum temp_ln_ap0;
  accum noise_var;
  
  unsigned char calcLLR;
  unsigned char iorecv_hold = 0; //flag for receiving data from IO core
  unsigned char deltasend_hold = 0; //flag for sending delta over to NCs
  unsigned char llrsend_hold = 0; //flag for sending LLR to IO core
  
  unsigned char flag_pkt_type = 0;

  volatile accum noisevar_sum_Ex2 = 0; // for noise var estimation - E{x^2}
  unsigned int sbit_tracker = 2;
  unsigned int tempCtr = 0;

  // Interrupts
  NVIC_SetPriority(2, 0);   

  // *** END - part1 ***

  // *** Part2 - Lime's initialization ***
  while(1){
    // wait for Lime's initialization to be finished
    while(noc_NC_rxbuff0_av!=1)__NOP();
    int tempack = NC_NOC_RX_BUFF0;
    
    while(noc_NC_txbuff_isfull==1)__NOP();
    NC_NOC_TX_BUFF0 = tempack;

    break;
  }
  
  // ** MAIN LOOP
  while(1){
    // *** Part3 - Lime RX ***
    
    //determine sync state - header from IO Core
    if(rx_state==STATE_EMPTY){
      while(noc_NC_rxbuff0_av!=1)__NOP();  
      rx_state = NC_NOC_RX_BUFF0;
    }

    // DETECT - calc power level I^2 + Q2
    if(rx_state==RX_DETECT){
      //receive I and Q
      while(noc_NC_rxbuff0_av!=1)__NOP(); Idata = NC_NOC_RX_BUFF0;
      while(noc_NC_rxbuff0_av!=1)__NOP(); Qdata = NC_NOC_RX_BUFF0;

      unsigned int sigPower = 0;
      
      mulResult_s64 = mac_smul_32((int32_t)Idata, (int32_t)Idata);
      sigPower += (unsigned int)mulResult_s64;

      mulResult_s64 = mac_smul_32((int32_t)Qdata, (int32_t)Qdata);
      sigPower += (unsigned int)mulResult_s64;

      //send back power calc result
      while(noc_NC_txbuff_isfull==1)__NOP();
      NC_NOC_TX_BUFF0 = sigPower;

      // reset rx_state
      rx_state = STATE_EMPTY;
    }

    // TIMING_SYNC - TED algorithm
    if(rx_state==RX_TIMING_SYNC){
      
      //receive I and Q
      while(noc_NC_rxbuff0_av!=1)__NOP(); Idata = NC_NOC_RX_BUFF0;
      while(noc_NC_rxbuff0_av!=1)__NOP(); Qdata = NC_NOC_RX_BUFF0;

      Isamp[nSamples] = sign_f(Idata);
      Qsamp[nSamples] = sign_f(Qdata);
    
      if(++nSamples==3){
        nSamples = 0; // reset counter

        // TED calc - Sign version 
        ted = 0;

        // I-part
        tempcalc = Isamp[2]- Isamp[0];
        tempcalc = mac_smul_32((int32_t)(Isamp[1]),
                               (int32_t)(tempcalc));
        ted += tempcalc;

        // Q-part
        tempcalc = Qsamp[2]- Qsamp[0];
        tempcalc = mac_smul_32((int32_t)(Qsamp[1]),
                               (int32_t)(tempcalc));
        ted += tempcalc;

        // determine step correction
        stepCorrection = sign_f(ted);

        //send back synchronization result
        while(noc_NC_txbuff_isfull==1)__NOP();
        NC_NOC_TX_BUFF0 = stepCorrection;
      }

      // reset rx_state
      rx_state = STATE_EMPTY;
    }
    
    // PHASECORR - freq offset calculation, and correct payload
    // estimate noise variance of payload at the same time
    if(rx_state==RX_PHASECORR){
      digitalWrite_low(ledpin);
      // init variables for noise var estimate
      noisevar_sum_Ex2 = 0;
      sbit_tracker = 2;

      for(int i=0; i<CHUNK_NUM; i++){
        // * receive freq pilot pair (I)
        while(noc_NC_rxbuff0_av!=1)__NOP();
        freqsyncBuff[0][0] = NC_NOC_RX_BUFF0;
        while(noc_NC_rxbuff0_av!=1)__NOP();
        freqsyncBuff[1][0] = NC_NOC_RX_BUFF0;

        // * request freq pilot pair (Q)
        while(noc_NC_rxbuff2_av!=1)__NOP();
        freqsyncBuff[0][1] = NC_NOC_RX_BUFF2;
        while(noc_NC_rxbuff2_av!=1)__NOP();
        freqsyncBuff[1][1] = NC_NOC_RX_BUFF2;

        // * freq offset calculation
        for(int j=0; j<2; j++){
          // scale back to 1 and -1 value (1 = +1024)
          iBuff.accum_cont = freqsyncBuff[j][0]; iBuff.int_cont >>= 10;
          qBuff.accum_cont = freqsyncBuff[j][1]; qBuff.int_cont >>= 10;
          
          // 1st preamble sync - pre sync preparation
          if(j==0){
            // determine which cartesius plane the received symbol is at
            // and move symbol to plane 1 (preamble '1')
            // so that cross product phase detector works properly
            if(iBuff.int_cont>=0){
              if(qBuff.int_cont>=0) // plane 1, corr 0
                planeCorr.accum_cont = 0;
              else                  // plane 4, corr 270 degree
                planeCorr.accum_cont = 4.7124; //154415
            }
            else{
              if(qBuff.int_cont>=0) // plane 2, corr 90 degree
                planeCorr.accum_cont = 1.5708; // 51471 
              else                  // plane 3, corr 180 degree
                planeCorr.accum_cont = 3.1416; // 102943 
            }

            // normalization - N = sqrt(i^2 + q^2)
            sigNorm.accum_cont = 0;
            mulResult_s64 = mac_smul_32((int32_t)(iBuff.int_cont),
                                 (int32_t)(iBuff.int_cont));
            temp_accumint.int_cont = (mulResult_s64>>15);
            sigNorm.accum_cont += temp_accumint.accum_cont; //i^2
            
            mulResult_s64 = mac_smul_32((int32_t)(qBuff.int_cont),
                                 (int32_t)(qBuff.int_cont));
            temp_accumint.int_cont = (mulResult_s64>>15);
            sigNorm.accum_cont += temp_accumint.accum_cont; //q^2
            
            unsigned int t = (sigNorm.int_cont << 1);
            t = S15_16_sqrt(t); // sqrt
            t >>= 1;
            sigNorm.int_cont = t;
          }

          iBuff.accum_cont /= sigNorm.accum_cont;
          qBuff.accum_cont /= sigNorm.accum_cont;

          // ~~DEBUG, dump normalized I & Q
          // Set up 1st plane / random offset correction for the
          // corresponding freqsync bit 1 / 2
          if(j==0){
            tempdump[0] = iBuff.int_cont; // freqbit_1 I
            tempdump[1] = qBuff.int_cont; // freqbit_1 Q
            sinBuff.accum_cont = sin_lookup(planeCorr.int_cont);
            cosBuff.accum_cont = cos_lookup(planeCorr.int_cont);
          }
          else if(j==1){
            tempdump[5] = iBuff.int_cont; // freqbit_2 I
            tempdump[6] = qBuff.int_cont; // freqbit_2 Q
            sinBuff.accum_cont = 0;
            cosBuff.accum_cont = 1;
          }

          ////
          // Improved flow, loop of phase correction and detection
          // (contains loop of detect and correct by fixed angle pi/32)
          // (to approach the accuracy of arcsin in detection)
          tempCorr.int_cont = 0;
          unsigned int loopctr = 0;
          while(1){
            // Phase offset correction part -> v * exp(-j.theta)
            // ** I part
            iCorrected.accum_cont = 0;

            mulResult_s64 = mac_smul_32((int32_t)(iBuff.int_cont),
                                 (int32_t)(cosBuff.int_cont));
            temp_accumint.int_cont = (mulResult_s64>>15);
            iCorrected.accum_cont += temp_accumint.accum_cont;

            mulResult_s64 = mac_smul_32((int32_t)(qBuff.int_cont),
                                 (int32_t)(sinBuff.int_cont));
            temp_accumint.int_cont = (mulResult_s64>>15);
            iCorrected.accum_cont += temp_accumint.accum_cont;
            
            // ** Q part
            qCorrected.accum_cont = 0;

            mulResult_s64 = mac_smul_32((int32_t)(qBuff.int_cont),
                                 (int32_t)(cosBuff.int_cont));
            temp_accumint.int_cont = (mulResult_s64>>15);
            qCorrected.accum_cont += temp_accumint.accum_cont;

            mulResult_s64 = mac_smul_32((int32_t)(iBuff.int_cont),
                                 (int32_t)(sinBuff.int_cont));
            temp_accumint.int_cont = (mulResult_s64>>15);
            qCorrected.accum_cont -= temp_accumint.accum_cont;

            // Phase offset detection part
            temp_accumint.accum_cont = freqsync_preamble[nSamples-1];

            if(j==1)
              temp_accumint.int_cont = tempdump[0]; // normalized freqbit1-I
            mulResult_s64 = mac_smul_32((int32_t)(temp_accumint.int_cont),
                                 (int32_t)(qCorrected.int_cont));
            phaseOffset.int_cont = (mulResult_s64>>15);

            if(j==1)
              temp_accumint.int_cont = tempdump[1]; // normalized freqbit1-Q
            mulResult_s64 = mac_smul_32((int32_t)(temp_accumint.int_cont),
                                 (int32_t)(iCorrected.int_cont));
            temp_accumint.int_cont = (mulResult_s64>>15);
            phaseOffset.accum_cont -= temp_accumint.accum_cont;

            // Accuracy loop part
            loopctr++;
            if((abs_f(phaseOffset.int_cont)<=3217) ||
               (loopctr==16)){
              tempCorr.accum_cont += phaseOffset.accum_cont;
              break;
            }

            // ** correction angle = pi/32
            // LUT[256], sin(pi/32) = [4], cos(pi/32) = [68]
            // LUT[512], sin(pi/32) = [8], cos(pi/32) = [136]
            if(sign_f(phaseOffset.int_cont)>0){
              sinBuff.accum_cont = sineTable[4];
              cosBuff.accum_cont = sineTable[68];
              tempCorr.int_cont += 3217; // + pi/32
            }
            else{
              sinBuff.accum_cont = -sineTable[4];
              cosBuff.accum_cont = sineTable[68];
              tempCorr.int_cont -= 3217; // + pi/32
            }

            // ** recursive for next loop
            iBuff.accum_cont = iCorrected.accum_cont;
            qBuff.accum_cont = qCorrected.accum_cont;

          }
          phaseOffset.accum_cont = tempCorr.accum_cont;
          
          // 1st preamble sync - random starting phase offset
          if(j==0){

            // phase diff adds to plane correction value
            // as random starting offset correction
            tempdump[2] = iCorrected.int_cont; // freqbit_1 Icorr
            tempdump[3] = qCorrected.int_cont; // freqbit_1 Qcorr
            tempdump[10] = sigNorm.int_cont; // normalization factor

            planeCorr.accum_cont += phaseOffset.accum_cont;

            tempdump[4] = planeCorr.int_cont; // total RandOffset
          }

          // 2nd preamble sync - symbols' relative phase offset
          else if(j==1){

            // relative phase difference
            relativeCorr.accum_cont = phaseOffset.accum_cont;

            tempdump[7] = iCorrected.int_cont; // freqbit_2 Icorr
            tempdump[8] = qCorrected.int_cont; // freqbit_2 Qcorr
            tempdump[9] = relativeCorr.int_cont; // relative offset
            tempdump[11] = sigNorm.int_cont; // normalization factor

            nSamples = 0;

            // reset rx_state
            // rx_state = STATE_EMPTY;

            // prepare initial angle value for RX_PHASECORR
            phaseOffset.accum_cont = planeCorr.accum_cont +
                                     relativeCorr.accum_cont;
            // phaseOffset.int_cont += 20017; // 30 degree 
          }
        }
        
        // * retrive payload, phase correction
        for(int j=0; j<CHUNK_LEN; j++){
          // receive I
          while(noc_NC_rxbuff0_av!=1)__NOP();
          int temp = NC_NOC_RX_BUFF0;
          iBuff.accum_cont = temp;
          // receive Q
          while(noc_NC_rxbuff2_av!=1)__NOP();
          temp = NC_NOC_RX_BUFF2;
          qBuff.accum_cont = temp;

          // scaling
          iBuff.int_cont>>=10; 
          qBuff.int_cont>>=10;

          // relative angle correction
          // ** cumulative angle offset
          phaseOffset.int_cont += relativeCorr.int_cont;
          if(phaseOffset.int_cont>205887)
            phaseOffset.int_cont -= 205887;

          
          // ** cosine sine LUT
          sinBuff.accum_cont = sin_lookup(phaseOffset.int_cont);
          cosBuff.accum_cont = cos_lookup(phaseOffset.int_cont);

          // ** I part
          iCorrected.accum_cont = 0;

          mulResult_s64 = mac_smul_32((int32_t)(iBuff.int_cont),
                               (int32_t)(cosBuff.int_cont));
          temp_accumint.int_cont = (mulResult_s64>>15);
          iCorrected.accum_cont += temp_accumint.accum_cont;

          mulResult_s64 = mac_smul_32((int32_t)(qBuff.int_cont),
                               (int32_t)(sinBuff.int_cont));
          temp_accumint.int_cont = (mulResult_s64>>15);
          iCorrected.accum_cont += temp_accumint.accum_cont;

          // send back phase-corrected I
          while(noc_NC_txbuff_isfull==1)__NOP();
          NC_NOC_TX_BUFF0 = NC_NOC_TX_BUFF2 = iCorrected.int_cont;

          // noise variance estimate - E{x^2} part
          if(sbit_tracker++==2){
            sbit_tracker = 0;
            
            // x^2
            mulResult_s64 = mac_smul_32((int32_t)(iCorrected.int_cont),
                                 (int32_t)(iCorrected.int_cont));
            temp_accumint.int_cont = (mulResult_s64>>15);

            // sum
            noisevar_sum_Ex2 += temp_accumint.accum_cont;
          }
        }
        
      }

      // dump debug variables
      for(int i=0; i<12; i++){
        while(noc_NC_txbuff_isfull==1)__NOP();
        NC_NOC_TX_BUFF0 = tempdump[i];
        while(noc_NC_rxbuff0_av!=1)__NOP();
        int temp = NC_NOC_RX_BUFF0;
      }

      // reset rx_state;
      rx_state = STATE_EMPTY;
    }
    
    // *** END - part3 ***

    // *** Part4 - Turbo Decoding ***
    if(rx_state==RX_DECODE){
      digitalWrite_high(ledpin);

      // ## Estimate noise variance ##
      // send E{x^2} part
      noisevar_sum_Ex2 >>= 8; // divide by 256
      while(noc_NC_txbuff_isfull==1)__NOP();
      temp_accumint.accum_cont = noisevar_sum_Ex2;
      NC_NOC_TX_BUFF0 = temp_accumint.int_cont;

      // aid in squaring E{x}
      while(noc_NC_rxbuff0_av!=1)__NOP();
      temp_accumint.int_cont = NC_NOC_RX_BUFF0;

      mulResult_s64 = mac_smul_32((int32_t)(temp_accumint.int_cont),
                           (int32_t)(temp_accumint.int_cont));
      temp_accumint.int_cont = (mulResult_s64>>15);
      
      while(noc_NC_txbuff_isfull==1)__NOP();
      NC_NOC_TX_BUFF0 = temp_accumint.int_cont;

      // loop till decoding is finished
      while(1){ 
        // ## Examine packet header ##
        if( (noc_NC_rxbuff0_av==1) && (flag_pkt_type==0)  )
          flag_pkt_type = NC_NOC_RX_BUFF0;

        // ## Receive calcLLR flag and noise_var from IO core ##
        if( (flag_pkt_type==IO_CHNLCTRL_HDR) &&
            (noc_NC_rxbuff0_av==1) && (noc_getNC_rxbuff0_count>=0x2)  ){  

          flag_pkt_type = 0; //clear packet type's flag
          
          calcLLR = NC_NOC_RX_BUFF0;
          
          temp_accumint.int_cont = NC_NOC_RX_BUFF0;
          noise_var = temp_accumint.accum_cont;
          
          //send back ACK - just send back calcLLR
          while(noc_NC_txbuff_isfull==1)__NOP();
          NC_NOC_TX_BUFF0 = calcLLR;
        }

        // ## Receive bits info from IO core, calc delta ##
        if( (flag_pkt_type==IO_BITS_HDR) &&
            (noc_NC_rxbuff0_av==1) && (noc_getNC_rxbuff0_count>=0x4) ){      
          
          //receive sysbit, parbit, apriori from NoC
          temp_accumint.int_cont = NC_NOC_RX_BUFF0; 
          temp_sysbit = temp_accumint.accum_cont;
          
          temp_accumint.int_cont = NC_NOC_RX_BUFF0; 
          iorecv_pbit[iorecv_i & ((r_win<<1)-1)] = temp_accumint.accum_cont;
          
          temp_accumint.int_cont = NC_NOC_RX_BUFF0; 
          temp_ln_ap1 = temp_accumint.accum_cont;
          
          temp_accumint.int_cont = NC_NOC_RX_BUFF0;
          temp_ln_ap0 = temp_accumint.accum_cont;
                
          //calculate, store delta
          r_turbodec_dcalc(temp_sysbit, iorecv_pbit[iorecv_i & ((r_win<<1)-1)],
                           local_deltabuff[iorecv_i & ((r_win<<1)-1)], temp_ln_ap1,
                           temp_ln_ap0, noise_var);
          
          iorecv_i++;
          
          //send back ACK
          while(noc_NC_txbuff_isfull==1)__NOP();
          NC_NOC_TX_BUFF0 = DSP_BITSACK_HDR; // Header - bits ack
          while(noc_NC_txbuff_isfull==1)__NOP();
          NC_NOC_TX_BUFF0 = iorecv_i;
          
          flag_pkt_type = 0; //clear packet type's flag
          
          //stop receiving if it received two windows and none decoded completely
          if( (iorecv_i==256) || ((iorecv_i&(r_win-1))== 0 &&
              ((iorecv_i>>r_win_mul)-(retllr_i>>r_win_mul))>=2) )
            iorecv_hold = 1;
        }

        // ## Send delta to NC0 ##
        if( (iorecv_i>delta_i) && (deltasend_hold==0) &&
            ((iorecv_i & (r_win-1))==0) ){

          for(char i=0; i<r_win; i++){
            for(char j=0; j<16; j++){
              temp_accumint.accum_cont = local_deltabuff[delta_i & ((r_win<<1)-1)][j]; 
              
              while(noc_NC_txbuff_isfull==1)__NOP();
              NC_NOC_TX_BUFF1 = temp_accumint.int_cont;
            }
            delta_i++;
            while(noc_NC_rxbuff1_av!=1)__NOP();
            int tempack = NC_NOC_RX_BUFF1;
          }
          //if( ((delta_i & (r_win-1))==0) && (((delta_i>>r_win_mul)-(retllr_i>>r_win_mul))>=1))
          deltasend_hold = 1;
        }

        // ## Wait for alpha and beta from NC0 ##
        if( (noc_NC_rxbuff1_av==1) && (alpha_i<32) && (beta_i>0) &&
            (noc_getNC_rxbuff1_count>=0x8)){

          // receive alpha
          for(char j=0; j<r_win; j++){
            for(char i=0; i<8; i++){
              while(noc_NC_rxbuff1_av!=1)__NOP();
              temp_accumint.int_cont = NC_NOC_RX_BUFF1;
              local_alphabuff[alpha_i][i] = temp_accumint.accum_cont;
            }
            alpha_i++;
            while(noc_NC_txbuff_isfull==1)__NOP();
            NC_NOC_TX_BUFF1 = alpha_i;
          }
          // receive beta
          for(char j=0; j<r_win-1; j++){
            for(char i=0; i<8; i++){
              while(noc_NC_rxbuff1_av!=1)__NOP();
              temp_accumint.int_cont = NC_NOC_RX_BUFF1;
              local_betabuff[beta_i-1][i] = temp_accumint.accum_cont;
            }
            beta_i--;
            while(noc_NC_txbuff_isfull==1)__NOP();
            NC_NOC_TX_BUFF1 = beta_i;
          }
        }

        // ## Calculate LLR and send to NC1 for conversion to Le ##
        // * check for ACK per x flits sent
        if( (llrsend_hold==1) && (noc_NC_rxbuff2_av==1) ){
          int temp_ack = NC_NOC_RX_BUFF2;
          llrsend_hold = 0;
        }

        // * sending part
        if( (alpha_i==32) && (beta_i==0) && (llrsend_hold==0) ){
          // send and wait ACK per x flits
          temp_accumint.accum_cont = r_turbodec_llrcalc(local_deltabuff[retllr_i & ((r_win<<1)-1)],
                                        local_alphabuff[retllr_i & (r_win-1)],
                                        local_betabuff[retllr_i & (r_win-1)], 
                                        iorecv_pbit[retllr_i & ((r_win<<1)-1)],
                                        calcLLR, noise_var);
            
          if(retllr_i==0){ //send calcLLR on beginning of a half-iteration
            while(noc_NC_txbuff_isfull==1)__NOP();
            NC_NOC_TX_BUFF2 = calcLLR;
            while(noc_NC_rxbuff2_av!=1)__NOP(); //wait for ACK
            int tempack = NC_NOC_RX_BUFF2;
          }
          
          while(noc_NC_txbuff_isfull==1)__NOP();
          NC_NOC_TX_BUFF2 = temp_accumint.int_cont;
          
          retllr_i++;
          
          if( (retllr_i & 7)==0 )
            llrsend_hold = 1;
          
          // allow sending next window's delta
          if( ((delta_i & (r_win-1))==0) &&
              (((delta_i>>r_win_mul)-(retllr_i>>r_win_mul))<2))
            deltasend_hold = 0;
          
          // after it decoded a full window, clear flags and reset counters
          if((retllr_i & (r_win-1))==0){
            alpha_i = 0;
            beta_i = 31;
          }

          if( (iorecv_i<256) && ((retllr_i&(r_win-1))== 0) &&
              (((iorecv_i>>r_win_mul)-(retllr_i>>r_win_mul))<2) )
            iorecv_hold = 0;
          
          // end of half iteration reset flags and counters
          if(retllr_i==256){ 
            iorecv_i = iorecv_hold = delta_i = retllr_i = 0;

            if(calcLLR==1){ // decode finished
              // clear up last ACK for LLR
              while(noc_NC_rxbuff2_av!=1)__NOP();
              int tempack = NC_NOC_RX_BUFF2;
              llrsend_hold = 0;

              // exit decoding loop
              break;
            }
          }
        }  
      } // end of decoding loop

      rx_state = STATE_EMPTY;
    }
    // *** END - part4 ***

    // I dont know why I must have this 'break' case
    // so that COMPILER ERROR wont happen
    if(rx_state==RX_STOP)
      break;

  } // ** END - MAIN LOOP

  return 0;

}