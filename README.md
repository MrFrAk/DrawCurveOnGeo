# DrawCurveOnGeo

This is an Autodesk Maya plugin that allows a user to draw a free hand curve "on screen" that will be projected onto a selected geometry (mesh only).

The curve(s) can be rebuilt on-the-fly based on initial settings(via tool property window) to either a fixed number of CVs or to a fraction of the number of CVs created.

Future potential improvements:
- make it faster using accelerated structure for ray-intersection query.
- make it work with nurbs surfaces.
- add CVs that are not projected on the mesh but the world position based on previous point depth position from camera.
- just project the first CV on a mesh to draw attached curve to a geometery.
- ...
