// This code is under LGPL-2.1 license.
// https://www.gnu.org/licenses/old-licenses/lgpl-2.1-standalone.html
//
// Author: Francois Coulon
//

/* This code creates a Maya pencil context allowing to draw a curve on a selected
mesh. For more implementation explanations, it is based on the Maya devkit
lassoTool example.
*/

#ifndef _LP_DRAWCURVEONGEO_CONTEXT_H_
#define _LP_DRAWCURVEONGEO_CONTEXT_H_

#include <maya/MPxContext.h>

#include <maya/M3dView.h>
#include <maya/MDagPath.h>
#include <maya/MPointArray.h>
#include <maya/MUIDrawManager.h>


namespace LivingPuppet
{

  class DrawCurveOnGeoContext: public MPxContext
  {
  public:
    DrawCurveOnGeoContext();
    virtual ~DrawCurveOnGeoContext() { reset(); }
    void* creator() { return new DrawCurveOnGeoContext; }
    // The following is mandatory so Properties and Values mel functions can be
    // executed when the Tool Settings UI is brought up. Base name must match.
    void getClassName(MString& name) const { name.set("drawCurveOnGeoContext"); }

    void toolOnSetup(MEvent& event) override {}

    MStatus doPress(MEvent& event) override;
    MStatus doPress(MEvent& event,
                    MHWRender::MUIDrawManager& drawManager,
                    const MHWRender::MFrameContext& context) override { return doPress(event); }

    MStatus doDrag(MEvent& event) override;
    MStatus doDrag(MEvent& event,
                   MHWRender::MUIDrawManager& drawManager,
                   const MHWRender::MFrameContext& context) override;

    MStatus doRelease(MEvent& event) override;
    MStatus doRelease(MEvent& event,
                      MHWRender::MUIDrawManager& drawManager,
                      const MHWRender::MFrameContext& context) override { return doRelease(event); }

    unsigned int getRebuildMode() { return m_rebuildMode; }
    void setRebuildMode(short rebuildMode) { m_rebuildMode = rebuildMode; }

    unsigned int getRebuildValue() { return m_rebuildValue; }
    void setRebuildValue(unsigned int rebuildValue) { m_rebuildValue = rebuildValue; }

  private:
    void reset();

    void initViewportDimensions(); // used by doPress()
    bool accumulateScreenPositions(MEvent& event); // used by doDrag()

    bool getTargetDagPath(); // used by doRelease()
    MPointArray project2dPointsOnMesh(); // used by doRelease()

    M3dView m_3dView;
    int m_viewportX{0};
    int m_viewportY{0};
    int m_viewportWidth{0};
    int m_viewportHeight{0};
    // using MPointArray only because drawManager.lineStrip in Viewport 2.0
    // takes that as an input so we don't have to cast an entire array there
    MPointArray m_2dPoints;
    MDagPath m_targetDagPath;

    unsigned int m_rebuildMode{0};
    unsigned int m_rebuildValue{4};
  };

} // namespace LivingPuppet

#endif
