#ifndef STRATUM_MATH_CONSTANTS
#define STRATUM_MATH_CONSTANTS

#include "stratum/macro/namespaces.hpp"


// Knuth Appendix A Table I  TABLES OF  NUMERICAL QUANTITIES   Page 701
// Scanned in from printed book using OCR software to convert to a text file.

// (C) Copyright Paul A Bristow, hetp Chromatography, 2001, 2002,
// and D E Knuth, Addison-Welsey ISBN 0201896834
// Permission to copy, use, modify, sell and distribute this software
// is granted provided this copyright notice appears in all copies.
// This software is provided "as is" with express or implied warranty,
// and with no claim as to its suitability for any purpose.

NAMESPACE_BEGIN(stratum, math, constant)


typedef long double value_type;


constexpr value_type pi             = 3.1415926535897932384626433832795028841972L; 
constexpr value_type e              = 2.7182818284590452353602874713526624977572L;
constexpr value_type sqrtTwo        = 1.4142135623730950488016887242096980785697L;
constexpr value_type sqrtThree      = 1.7320508075688772935274463415058723669428L;
constexpr value_type sqrtFive       = 2.2360679774997896964091736687312762354406L;
constexpr value_type sqrtTen        = 3.1622776601683793319988935444327185337196L;
constexpr value_type cubeRootTwo    = 1.2599210498948731647672106072782283505703L;
constexpr value_type cubeRootThree  = 1.4422495703074083823216383107801095883919L;
constexpr value_type fourthRootTwo  = 1.1892071150027210667174999705604759152930L;
constexpr value_type lnTwo          = 0.6931471805599453094172321214581765680755L;
constexpr value_type lnThree        = 1.0986122886681096913952452369225257046475L;
constexpr value_type lnTen          = 2.3025850929940456840179914546843642076011L;
constexpr value_type oneDivLnTwo    = 1.4426950408889634073599246810018921374266L;
constexpr value_type oneDivLnTen    = 0.4342944819032518276511289189166050822944L;
constexpr value_type degree         = 0.0174532925199432957692369076848861271344L;
constexpr value_type oneDivPi       = 0.3183098861837906715377675267450287240689L;
constexpr value_type piSqr          = 9.8696044010893586188344909998761511353137L;
constexpr value_type sqrtPi         = 1.7724538509055160272981674833411451827975L;
constexpr value_type gammaThird     = 2.6789385347077476336556929409746776441287L;
constexpr value_type gammaTwoThirds = 1.3541179394264004169452880281545137855193L;
constexpr value_type oneDivE        = 0.3678794411714423215955237701614608674458L;
constexpr value_type eSqr           = 7.3890560989306502272304274605750078131803L;
constexpr value_type euler          = 0.5772156649015328606065120900824024310422L;
constexpr value_type lnPi           = 1.1447298858494001741434273513530587116473L;
constexpr value_type phi            = 1.6180339887498948482045868343656381177203L;
constexpr value_type ePowEuler      = 1.7810724179901979852365041031071795491696L;
constexpr value_type ePowQuarterPi  = 2.1932800507380154565597696592787382234616L;
constexpr value_type sinOne         = 0.8414709848078965066525023216302989996226L;
constexpr value_type cosOne         = 0.5403023058681397174009366074429766037323L;
constexpr value_type zetaThree      = 1.2020569031595942853997381615114499907650L;
constexpr value_type lnPhi          = 0.4812118250596034474977589134243684231352L;
constexpr value_type oneDivLnPhi    = 2.0780869212350275376013226061177957677422L;
constexpr value_type phiConjugate   = 0.6180339887498948482045868343656381177203L;


NAMESPACE_END(stratum, math, constant)

#endif //STRATUM_MATH_CONSTANTS
