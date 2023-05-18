# DrawCurveOnGeo

This is an Autodesk Maya plugin that allows a user to draw a free hand curve "on screen" that will be projected onto a selected geometry (mesh only available, for now...).

The curve(s) can be rebuilt on-the-fly based on initial settings(via tool property window) to either a static number of CVs or to a ffraction of the number of created CVs.

Future potential improvements:
- make it faster but that would require to previously select (a) mesh(es) in the outliner.
- add CVs that are not projected on the mesh but the world position is based on previous point Z position from camera.
- just project the first CV on a mesh to draw attached curve to a geometery.
- ...
