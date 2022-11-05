sources = [
    'main.c',
    'application.c',
    'window.c',
    'upload.c',
]

request_c_dep = dependency('request-c', required : true)
if not request_c_dep.found()
  request_c_proj = subproject('request-c')
  request_c_dep = request_c_proj.get_variable('request_c_dep')
endif

deps = [
    request_c_dep,
    dependency('gtk4'),
    dependency('libadwaita-1'),
    dependency('libcurl'),
    dependency('json-c'),
]

gnome=import('gnome')
resources = gnome.compile_resources('resources', 'gresource.xml')

executable('coco-uploader', sources, resources, dependencies : deps, install: true)
