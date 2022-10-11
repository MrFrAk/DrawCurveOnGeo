// This code is under LGPL-2.1 license.
// https://www.gnu.org/licenses/old-licenses/lgpl-2.1-standalone.html
//
// Author: Francois Coulon
//

#include <maya/MFnPlugin.h>

#include "drawCurveOnGeoContextCommand.h"

using namespace LivingPuppet;


MStatus initializePlugin(MObject object)
{
  MFnPlugin plugin(object, "Francois Coulon", "1.0", "Any");
  return plugin.registerContextCommand("drawCurveOnGeoContext",
                                       DrawCurveOnGeoContextCommand::creator);
}


MStatus uninitializePlugin(MObject object)
{
  MFnPlugin plugin(object);
  return plugin.deregisterContextCommand("drawCurveOnGeoContext");
}
