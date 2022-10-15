// This code is under LGPL-2.1 license.
// https://www.gnu.org/licenses/old-licenses/lgpl-2.1-standalone.html
//
// Author: Francois Coulon
//

/* Tool Command is used here to be able to undo and redo the nurbs curve
creation.
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
    MStatus undoIt() override { return MGlobal::deleteNode(m_thisDagPath.transform()); }
    bool isUndoable() const override { return true; }
    MStatus finalize() override;

    void setEPs(MPointArray& eps) { m_eps = eps; }
    void setRebuildMode(short rebuildMode) { m_rebuildMode = rebuildMode; }
    void setRebuildValue(unsigned int rebuildValue) { m_rebuildValue = rebuildValue; }

  private:
    MPointArray m_eps;
    unsigned int m_rebuildValue{4};
    short m_rebuildMode{0};
    MDagPath m_thisDagPath;
  };

} // namespace LivingPuppet

#endif
