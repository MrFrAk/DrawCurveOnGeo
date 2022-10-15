// This code is under LGPL-2.1 license.
// https://www.gnu.org/licenses/old-licenses/lgpl-2.1-standalone.html
//
// Author: Francois Coulon
//

#ifndef _LP_DRAWCURVEONGEO_CONTEXTCOMMAND_H_
#define _LP_DRAWCURVEONGEO_CONTEXTCOMMAND_H_

#include <maya/MPxContextCommand.h>

#include "drawCurveOnGeoContext.h"


namespace LivingPuppet
{

  class DrawCurveOnGeoContextCommand: public MPxContextCommand
  {
  public:
    DrawCurveOnGeoContextCommand() {}
    virtual ~DrawCurveOnGeoContextCommand() {}
    static void* creator() { return new DrawCurveOnGeoContextCommand; }
    MPxContext* makeObj() override;

    virtual MStatus appendSyntax();
    virtual MStatus doEditFlags();

  protected:
    DrawCurveOnGeoContext* m_context;
  };

} // namespace LivingPuppet

#endif
