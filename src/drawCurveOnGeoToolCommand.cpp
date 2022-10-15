// This code is under LGPL-2.1 license.
// https://www.gnu.org/licenses/old-licenses/lgpl-2.1-standalone.html
//
// Author: Francois Coulon
//

#include "drawCurveOnGeoToolCommand.h"

#include <maya/MFnNurbsCurve.h>

using namespace LivingPuppet;


DrawCurveOnGeoToolCommand::DrawCurveOnGeoToolCommand()
{
  setCommandString("drawCurveOnGeoToolCmd");
}


MStatus DrawCurveOnGeoToolCommand::redoIt()
//  This method creates the curve from the edit points
{
  MFnNurbsCurve fnCurve;
  fnCurve.createWithEditPoints(m_eps, 3, MFnNurbsCurve::kOpen, false, false, true);

  std::cout << "// DEBUG: DrawCurveOnGeoToolCommand - m_rebuildMode: " << m_rebuildMode;
  std::cout << "\n// DEBUG: DrawCurveOnGeoToolCommand - m_rebuildValue: " << m_rebuildValue << std::endl;
  switch(m_rebuildMode)
  {
    case 1: // to a fixed number of CVs
      std::cout << "// DEBUG: DrawCurveOnGeoToolCommand - spans: " << (m_rebuildValue - 3) << std::endl;
      fnCurve.rebuild((m_rebuildValue - 3)); // 3: degree of curve
      break;
    case 2: // to a fraction of the number of CVs
      {
        unsigned int numberOfCVs = static_cast<unsigned int>(fnCurve.numCVs()) / m_rebuildValue;
        unsigned int numberOfSpans = (numberOfCVs > 4)? (numberOfCVs - 3): 1;
        std::cout << "// DEBUG: DrawCurveOnGeoToolCommand - numberOfSpans: " << numberOfSpans << std::endl;
        fnCurve.rebuild(numberOfSpans);
      }
      break;
    default:
      break;
  }
  return fnCurve.getPath(m_thisDagPath);
}


MStatus DrawCurveOnGeoToolCommand::undoIt()
{
    MObject transform = m_thisDagPath.transform();
    return MGlobal::deleteNode(transform);
}


MStatus DrawCurveOnGeoToolCommand::finalize()
// Command is finished, construct a string for the command for journaling.
{
  MArgList command;
  command.addArg(MString("Create degree 3 nurbsCurve with"));
  command.addArg(MFnNurbsCurve(m_thisDagPath).numCVs());
  command.addArg(MString("cvs."));
  return MPxToolCommand::doFinalize(command);
}
