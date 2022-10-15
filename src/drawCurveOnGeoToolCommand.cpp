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
  const unsigned int curveDegree = 3;
  unsigned int numberOfCVs = m_eps.length();
  unsigned int numberOfSpans = numberOfCVs - deg;
  const unsigned int numberOfKnots = spans + 2 * deg - 1;

  MDoubleArray knots;
  for (double knot = 0.0; knot < numberOfKnots; knot += 1.0)
      knots.append(knot);

  MFnNurbsCurve fnCurve;
  fnCurve.createWithEditPoints(m_eps, 3, MFnNurbsCurve::kOpen, false, false, true);

  switch(m_rebuildMode)
  {
    case 1: // to a fixed number of CVs
      fnCurve.rebuild((m_rebuildValue - 3)); // 3: degree of curve
      break;
    case 2: // to a fraction of the number of CVs
      {
        unsigned int numberOfCVs = static_cast<unsigned int>(fnCurve.numCVs()) / m_rebuildValue;
        unsigned int numberOfSpans = (numberOfCVs > 4)? (numberOfCVs - 3): 1;
        fnCurve.rebuild(numberOfSpans);
      }
      break;
    default:
      break;
  }
  return fnCurve.getPath(m_thisDagPath);
}


MStatus DrawCurveOnGeoToolCommand::finalize()
// Command is finished, construct a string for the command for journaling.
{
  MArgList command;
  command.addArg(MString("Created a degree 3 nurbs curve with "));
  command.addArg(MFnNurbsCurve(m_thisDagPath).numCVs());
  command.addArg(MString(" cvs.");
  return MPxToolCommand::doFinalize(command);
}
