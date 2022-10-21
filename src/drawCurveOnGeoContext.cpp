// This code is under LGPL-2.1 license.
// https://www.gnu.org/licenses/old-licenses/lgpl-2.1-standalone.html
//
// Author: Francois Coulon
//

#include "drawCurveOnGeoContext.h"
#include "drawCurveOnGeoToolCommand.h"

#include <maya/MCursor.h>
#include <maya/MFnMesh.h>
#include <maya/MFnNurbsCurve.h>
#include <maya/MGlobal.h>
#include <maya/MItSelectionList.h>
#include <maya/MSelectionList.h>

using namespace LivingPuppet;


DrawCurveOnGeoContext::DrawCurveOnGeoContext()
{
  setTitleString("drawCurveOnGeoContext");
  setImage("pencil.png", MPxContext::kImage1);
  setCursor(MCursor::pencilCursor);
  reset();
}


MStatus DrawCurveOnGeoContext::doPress(MEvent& event)
{
  // Ideas:
  //- Allow selecting a geometry with kRightMouse.
  //- Handle multiple geometries? If so, check shift key modifier is pressed.
  if (event.mouseButton() != MEvent::kLeftMouse)
    return MStatus::kFailure;

  MStatus status;
  m_3dView = M3dView::active3dView(&status);
  if (status == MStatus::kFailure)
    return status;

  if (!getTargetDagPath())
    return MStatus::kUnknownParameter;

  initViewportDimensions();

  short x;
  short y;
  event.getPosition(x, y);
  m_2dPoints.append(MPoint(static_cast<double>(x), static_cast<double>(y), 0.0));

  return MStatus::kSuccess;
}


// Legacy Viewport
MStatus DrawCurveOnGeoContext::doDrag(MEvent& event)
{
  if (!accumulateScreenPositions(event))
    return MStatus::kUnknownParameter;

  m_3dView.beginXorDrawing(true, true, 1.0f, M3dView::kStippleDashed, MColor(0.65, 0.856, 0.975));
  glBegin(GL_LINE_STRIP);
  for (size_t i = 0; i < m_2dPoints.length(); ++i)
    glVertex2d(m_2dPoints[i].x, m_2dPoints[i].y);
  glEnd();
  m_3dView.endXorDrawing();

  return MStatus::kSuccess;
}


// Viewport 2.0
MStatus DrawCurveOnGeoContext::doDrag(MEvent& event,
                                      MHWRender::MUIDrawManager& drawManager,
                                      const MHWRender::MFrameContext& context)
{
  if (!accumulateScreenPositions(event))
    return MStatus::kUnknownParameter;

  drawManager.setColor(MColor(0.65, 0.856, 0.975));
  drawManager.setLineStyle(MUIDrawManager::kDashed);
  drawManager.lineStrip(m_2dPoints, true);
  return MStatus::kSuccess;
}



MStatus DrawCurveOnGeoContext::doRelease(MEvent& event)
{
  short x;
  short y;
  event.getPosition(x, y);
  m_2dPoints.append(MPoint(static_cast<double>(x), static_cast<double>(y), 0.0));

  MPointArray editPoints = project2dPointsOnMesh();
  unsigned int numEditPoints = editPoints.length();
  // We need at least 2 points to have hit the target to create a curve
  if (numEditPoints > 1)
  {
    DrawCurveOnGeoToolCommand* cmd = (DrawCurveOnGeoToolCommand*)newToolCommand();
    cmd->setEPs(editPoints);
    switch(m_rebuildMode)
    {
      case 1: // to a fixed number of CVs
        cmd->setSpans((m_rebuildValue - 3)); // here m_rebuildValue == numCVs
        cmd->setKeepControlPoints(false);
        break;
      case 2: // to a fraction of the number of CVs
        {
          unsigned int numCVs = (numEditPoints + 2) / m_rebuildValue;
          unsigned int numSpans = (numCVs > 4)? (numCVs - 3): 1;
          cmd->setSpans(static_cast<int>(numSpans));
          cmd->setKeepControlPoints(false);
        }
        break;
      default: // just set the parameter range to [0.0 - 1.0]
        {
          unsigned int numCVs = (numEditPoints + 2) / m_rebuildValue;
          unsigned int numSpans = (numCVs > 4)? (numCVs - 3): 1;
          cmd->setSpans(static_cast<int>(numSpans));
          cmd->setKeepControlPoints(true);
        }
        break;
    }
    cmd->redoIt();
    cmd->finalize();
    m_3dView.refresh();
  }
  m_2dPoints.clear();
  return MStatus::kSuccess;
}


// -> Private member functions

void DrawCurveOnGeoContext::reset()
{
  m_2dPoints.clear();
  m_targetDagPath = MDagPath();
}


void DrawCurveOnGeoContext::initViewportDimensions()
{
  unsigned int viewX;
  unsigned int viewY;
  unsigned int viewW;
  unsigned int viewH;
  m_3dView.viewport(viewX, viewY, viewW, viewH);

  m_viewportX = static_cast<int>(viewX);
  m_viewportY = static_cast<int>(viewY);
  m_viewportWidth = static_cast<int>(viewW);
  m_viewportHeight = static_cast<int>(viewH);
}


bool DrawCurveOnGeoContext::accumulateScreenPositions(MEvent& event)
{
  short x;
  short y;
  event.getPosition(x, y);

  if (x < m_viewportX || x > (m_viewportX + m_viewportWidth) ||
      y < m_viewportY || y > (m_viewportY + m_viewportHeight))
    return false;

  m_2dPoints.append(MPoint(static_cast<double>(x), static_cast<double>(y), 0.0));
  return true;
}


bool DrawCurveOnGeoContext::getTargetDagPath()
{
  MSelectionList activeSelectionList;
  MGlobal::getActiveSelectionList(activeSelectionList);
  if (activeSelectionList.length() == 0)
  {
    MGlobal::displayError("DrawCurveOnGeoContext - Nothing selected. Please select a target mesh.");
    return false;
  }

  bool foundOne = false;
  MItSelectionList itSelectionList(activeSelectionList);
  while (!itSelectionList.isDone())
  {
    MDagPath dagPath;
    itSelectionList.getDagPath(dagPath);
    if (dagPath.hasFn(MFn::kMesh))
    {
      m_targetDagPath = dagPath;
      foundOne = true;
      break;
    }
    itSelectionList.next();
  }

  if (!foundOne)
  {
    MGlobal::displayError("DrawCurveOnGeoContext - No target mesh selected.");
    return false;
  }

  return true;
}


MPointArray DrawCurveOnGeoContext::project2dPointsOnMesh()
// Here we cast a ray from screen point postion towards the target mesh.
// viewToWorld: Takes a point in port coordinates and returns a corresponding
// ray in world coordinates.
{
  MFnMesh fnMesh(m_targetDagPath);
  MPointArray points;
  for (size_t i = 0; i < m_2dPoints.length(); ++i)
  {
    MPoint source;
    MVector ray;
    // Skip if 2d point isn't over the active viewport
    if (!m_3dView.viewToWorld(static_cast<int>(m_2dPoints[i].x), static_cast<int>(m_2dPoints[i].y), source, ray))
      continue;

    MPointArray intersectingPoints;
    // note: We could use closestIntersection but that would mean we'd have to
    // do a bit of setup to build an MMeshIsectAccelParams structure and really
    // take advantage of the function...
    if (fnMesh.intersect(source, ray, intersectingPoints, 1e-5, MSpace::kWorld))
      points.append(intersectingPoints[0]); // storing the closest point only
  }
  return points;
}
