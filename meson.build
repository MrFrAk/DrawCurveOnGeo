project('drawcurveongeo', 'cpp',
        version: '1.0.0',
        default_options: ['warning_level=1',
                          'cpp_std=c++17',
                          'b_ndebug=if-release',
                          'libdir=lib',
                          'pkgconfig.relocatable=true'])

printenv = find_program('printenv')
maya_version = run_command(printenv, 'MAYA_VERSION_MAJOR').stdout().strip()

shared_module('drawCurveOnGeo',
              'src/drawCurveOnGeoContext.cpp',
              'src/drawCurveOnGeoContextCommand.cpp',
              'src/drawCurveOnGeoToolCommand.cpp',
              'src/pluginMain.cpp',
              name_prefix: '',
              include_directories: 'src',
              dependencies: [dependency('maya', include_type: 'system')],
              install: true,
              install_dir: 'plugins')
