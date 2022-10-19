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
  setCommandString("drawCurveOnGeoTool");
}


MStatus DrawCurveOnGeoToolCommand::redoIt()
//  This method creates the curve from the edit points
{
  MFnNurbsCurve fnCurve;
  MObject curveDag = fnCurve.createWithEditPoints(m_eps, 3, MFnNurbsCurve::kOpen, false, false, true);
  return rebuildCurveInPlace(fnCurve, curveDag);
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
  command.addArg(commandString());
  return MPxToolCommand::doFinalize(command);
}


MStatus DrawCurveOnGeoToolCommand::rebuildCurveInPlace(MFnNurbsCurve& fnCurve,
                                                       MObject& curveDag)
{
  // At this point we only have one shape. Let's grab it to delete it later as
  // we'll have a new rebuilt one.
  fnCurve.getPath(m_thisDagPath);
  m_thisDagPath.extendToShape();
  MObject curve = m_thisDagPath.node();

  MObject curveRebuiltData = fnCurve.rebuild(m_spans, 3, 0, 0, true, true, m_keepControlPoints);

  MFnNurbsCurve fnCurveRebuilt(curveRebuiltData);
  MPointArray cvsRebuilt;
  fnCurveRebuilt.getCVs(cvsRebuilt);
  MDoubleArray knotsRebuilt;
  fnCurveRebuilt.getKnots(knotsRebuilt);
  MObject curveRebuilt = fnCurve.create(cvsRebuilt,
                                        knotsRebuilt,
                                        3,
                                        MFnNurbsCurve::kOpen,
                                        false,
                                        true,
                                        curveDag);

  MString curveName = MFnDependencyNode(curve).name();
  MGlobal::deleteNode(curve);

  MFnDependencyNode curveRebuiltNode(curveRebuilt);
  curveRebuiltNode.setName(curveName);
  return fnCurve.getPath(m_thisDagPath); // getting the rebuilt dagPath
}
