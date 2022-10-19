// This code is under LGPL-2.1 license.
// https://www.gnu.org/licenses/old-licenses/lgpl-2.1-standalone.html
//
// Author: Francois Coulon
//

/* Tool Command is used to undo and redo the nurbs curve creation.
To rebuild the curve, we get the rebuildMode and rebuildValue user data from
the contextCommand via context.onRelease() function where this tool command is
created and used.
*/

#ifndef _LP_DRAWCURVEONGEO_TOOLCOMMAND_H_
#define _LP_DRAWCURVEONGEO_TOOLCOMMAND_H_

#include <maya/MPxToolCommand.h>

#include <maya/MArgList.h>
#include <maya/MDagPath.h>
#include <maya/MDoubleArray.h>

#include <maya/MGlobal.h>
#include <maya/MPointArray.h>
#include <maya/MStatus.h>


namespace LivingPuppet
{

  class DrawCurveOnGeoToolCommand: public MPxToolCommand
  {
  public:
    DrawCurveOnGeoToolCommand();
    virtual ~DrawCurveOnGeoToolCommand() override {}
    static void* creator() { return new DrawCurveOnGeoToolCommand; }

    MStatus doIt(const MArgList& args) override { return redoIt(); };
    MStatus redoIt() override;
    MStatus undoIt() override;
    bool isUndoable() const override { return true; }
    MStatus finalize() override;

    void setEPs(MPointArray& eps) { m_eps = eps; }
    void setSpans(int spans) { m_spans = spans; }
    void setKeepControlPoints(bool keepControlPoints) { m_keepControlPoints = keepControlPoints; }

  private:
    MStatus rebuildCurveInPlace(MFnNurbsCurve& fnCurve,
                                MObject& curveDag);

    MPointArray m_eps;
    int m_spans {1};
    bool m_keepControlPoints{false};
    MDagPath m_thisDagPath;
  };

} // namespace LivingPuppet

#endif
