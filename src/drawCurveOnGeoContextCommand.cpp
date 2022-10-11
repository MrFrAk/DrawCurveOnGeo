// This code is under LGPL-2.1 license.
// https://www.gnu.org/licenses/old-licenses/lgpl-2.1-standalone.html
//
// Author: Francois Coulon
//

// #include <maya/MArgParser.h>
// #include <maya/MSyntax.h>

#include "drawCurveOnGeoContextCommand.h"

using namespace LivingPuppet;


// ideas: allow rebuilding the created curve to the given cvs value.
//    amountMode let the user choose if the cvValue is a "fixed"" number of CVs
//    or if cvValue "divide" the number of cvs of the created curve. Maybe use
//    a percentage ? (tbd)
// static const char* kNumCVsFlag = "-cv";
// static const char* kNumCVsLongFlag = "-cvValue";
// static const char* kAmountModeFlag = "-am";
// static const char* kAmountModeLongFlag = "-amountMode";

MPxContext* DrawCurveOnGeoContextCommand::makeObj()
{
  // m_context = new DrawCurveOnGeoContext();
  return new DrawCurveOnGeoContext(); // m_context;
}


// MStatus DrawCurveOnGeoContextCommand::appendSyntax()
// {
//    MSyntax cmdSyntax = syntax();
//    cmdSyntax.addFlag(kNumCVsFlag, kNumCVsLongFlag, MSyntax::kUnsigned);
//    cmdSyntax.addFlag(kAmountModeFlag, kAmountModeLongFlag, MSyntax::kString);

//    return MStatus::kSuccess;
// }


// MStatus DrawCurveOnGeoContextCommand::doEditFlags()
// {
//    MArgParser argData = parser();
//    int iValue;
//    MString sValue;

//    if (argData.isFlagSet(kNumCVsFlag))
//    {
//        argData.getFlagArgument(kNumCVsFlag, 0, iValue);
//        m_context->setNumCVs(iValue);
//    }

//    if (argData.isFlagSet(kAmountModeFlag))
//    {
//        argData.getFlagArgument(kAmountModeFlag, 0, sValue);
//        m_context->setAmountMode(sValue);
//    }

//    return MStatus::kSuccess;
// }
