// This code is under LGPL-2.1 license.
// https://www.gnu.org/licenses/old-licenses/lgpl-2.1-standalone.html
//
// Author: Francois Coulon
//

#include "drawCurveOnGeoContextCommand.h"

#include <maya/MArgParser.h>
#include <maya/MSyntax.h>

using namespace LivingPuppet;


static const char* kRebuildModeFlag = "-rm";
static const char* kRebuildModeLongFlag = "-rebuildMode"; // 0: off, 1: fixed, 2: fraction
static const char* kRebuildValueFlag = "-rv";
static const char* kRebuildValueLongFlag = "-rebuildValue";


MPxContext* DrawCurveOnGeoContextCommand::makeObj()
{
  m_context = new DrawCurveOnGeoContext();
  return m_context;
}


MStatus DrawCurveOnGeoContextCommand::appendSyntax()
{
  MSyntax cmdSyntax = syntax();
  cmdSyntax.addFlag(kRebuildModeFlag, kRebuildModeLongFlag, MSyntax::kUnsigned);
  cmdSyntax.addFlag(kRebuildValueFlag, kRebuildValueLongFlag, MSyntax::kUnsigned);
  return MStatus::kSuccess;
}


MStatus DrawCurveOnGeoContextCommand::doQueryFlags()
{
  MArgParser argData = parser();

  if (argData.isFlagSet(kRebuildModeFlag))
    setResult(static_cast<int>(m_context->getRebuildMode()));

  if (argData.isFlagSet(kRebuildValueFlag))
    setResult(static_cast<int>(m_context->getRebuildValue()));

  return MStatus::kSuccess;
}


MStatus DrawCurveOnGeoContextCommand::doEditFlags()
{
  MArgParser argData = parser();

  if (argData.isFlagSet(kRebuildModeFlag))
  {
    unsigned int iValue;
    argData.getFlagArgument(kRebuildModeFlag, 0, iValue);
    m_context->setRebuildMode(iValue);
  }

  if (argData.isFlagSet(kRebuildValueFlag))
  {
    unsigned int iValue;
    argData.getFlagArgument(kRebuildValueFlag, 0, iValue);
    m_context->setRebuildValue(iValue);
  }

  return MStatus::kSuccess;
}
