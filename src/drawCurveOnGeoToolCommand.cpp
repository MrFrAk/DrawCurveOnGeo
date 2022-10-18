// This code is under LGPL-2.1 license.
// https://www.gnu.org/licenses/old-licenses/lgpl-2.1-standalone.html
//
// Author: Francois Coulon
//

#include "drawCurveOnGeoToolCommand.h"

#include <maya/MDoubleArray.h>
#include <maya/MString.h>

#include <maya/MFnDependencyNode.h>
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
  MStatus status = fnCurve.getPath(m_thisDagPath);
  switch(m_rebuildMode)
  {
    case 1: // to a fixed number of CVs
      status = rebuildCurveInPlace(fnCurve, curveDag, (m_rebuildValue - 3)); // 3: curve's degree
      break;
    case 2: // to a fraction of the number of CVs
      {
        unsigned int numberOfCVs = static_cast<unsigned int>(fnCurve.numCVs()) / m_rebuildValue;
        unsigned int numberOfSpans = (numberOfCVs > 4)? (numberOfCVs - 3): 1;
        status = rebuildCurveInPlace(fnCurve, curveDag, numberOfSpans);
      }
      break;
    default:
      break;
  }
  return status;
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
  command.addArg(m_thisDagPath.partialPathName());
  command.addArg(MString("cubic curve created with"));
  command.addArg(MFnNurbsCurve(m_thisDagPath).numCVs());
  command.addArg(MString("cvs."));
  return MPxToolCommand::doFinalize(command);
}


MStatus DrawCurveOnGeoToolCommand::rebuildCurveInPlace(MFnNurbsCurve& fnCurve,
                                                       MObject& curveDag,
                                                       unsigned int spans)
{
  // At this point we only have one shape. Let's grab it to delete it later as
  // we'll have a new rebuilt one.
  m_thisDagPath.extendToShape();
  MObject curve = m_thisDagPath.node();

  // 3: degree of curve, 0: parametrize between 0.0-1.0
  MObject curveRebuiltData = fnCurve.rebuild(spans, 3, 0);

  MFnNurbsCurve fnCurveRebuilt(curveRebuiltData);
  MPointArray cvsRebuilt;
  fnCurveRebuilt.getCVs(cvsRebuilt);
  MDoubleArray knotsRebuilt;
  fnCurveRebuilt.getKnots(knotsRebuilt);
  MObject curveRebuilt = fnCurve.create(cvsRebuilt, knotsRebuilt, 3,
                                        MFnNurbsCurve::kOpen, false, true,
                                        curveDag);

  // Cosmetics: deletion of original curve and renaming rebuilt one.
  MString curveName = MFnDependencyNode(curve).name();
  MGlobal::deleteNode(curve);

  MFnDependencyNode curveRebuiltNode(curveRebuilt);
  curveRebuiltNode.setName(curveName);
  return fnCurve.getPath(m_thisDagPath); // getting the rebuilt dagPath
}
