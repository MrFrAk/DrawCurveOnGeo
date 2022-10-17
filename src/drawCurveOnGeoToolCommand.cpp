// This code is under LGPL-2.1 license.
// https://www.gnu.org/licenses/old-licenses/lgpl-2.1-standalone.html
//
// Author: Francois Coulon
//

#include "drawCurveOnGeoToolCommand.h"

#include <maya/MDoubleArray.h>
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
  MObject curveDag = fnCurve.createWithEditPoints(m_eps, 3, MFnNurbsCurve::kOpen, false, false, true);

  std::cout << "// DEBUG: DrawCurveOnGeoToolCommand - m_rebuildMode: " << m_rebuildMode;
  std::cout << "\n// DEBUG: DrawCurveOnGeoToolCommand - m_rebuildValue: " << m_rebuildValue << std::endl;
  switch(m_rebuildMode)
  {
    case 1: // to a fixed number of CVs
      rebuildCurveInPlace(fnCurve, curveDag, (m_rebuildValue - 3)); // 3: curve's degree
      break;
    case 2: // to a fraction of the number of CVs
      {
        unsigned int numberOfCVs = static_cast<unsigned int>(fnCurve.numCVs()) / m_rebuildValue;
        unsigned int numberOfSpans = (numberOfCVs > 4)? (numberOfCVs - 3): 1;
        rebuildCurveInPlace(fnCurve, curveDag, numberOfSpans);
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
// Command is finished, construct a string for journaling.
{
  MArgList command;
  command.addArg(MString("Create degree 3 nurbsCurve with"));
  command.addArg(MFnNurbsCurve(m_thisDagPath).numCVs());
  command.addArg(MString("cvs."));
  return MPxToolCommand::doFinalize(command);
}


void DrawCurveOnGeoToolCommand::rebuildCurveInPlace(MFnNurbsCurve& fnCurve,
                                                    MObject& curveDag,
                                                    unsigned int spans)
{
  MObject curveRebuiltData = fnCurve.rebuild(spans); // 3: degree of curve

  MFnNurbsCurve fnCurveRebuilt(curveRebuiltData);
  MPointArray cvsRebuilt;
  fnCurveRebuilt.getCVs(cvsRebuilt);
  MDoubleArray knotsRebuilt;
  fnCurveRebuilt.getKnots(knotsRebuilt);
  MObject curveRebuiltShape = fnCurve.create(cvsRebuilt, knotsRebuilt, 3,
                                             MFnNurbsCurve::kOpen, false, true,
                                             curveDag);
}
