coco_sources = [
  'main.c',
  'coco-window.c',
  'coco-application.c',
  'components/base64/base64.c',
  'components/base64/lib/encode.c',
  'components/base64/lib/decode.c',
  'components/base64/lib/buffer.c',
  'components/time_convert/time_convert.c',
  'components/clipboard/clipboard.c',
  'components/upyun_upload/upload.c',
  'components/password_generate/password_generate.c',
  'components/hash/hash.c',
]

request_c_dep = dependency('request-c', required : true)
if not request_c_dep.found()
  request_c_proj = subproject('request-c')
  request_c_dep = request_c_proj.get_variable('request_c_dep')
endif

coco_deps = [
  request_c_dep,
  dependency('gtk4'),
  dependency('libadwaita-1'),
  dependency('libcurl'),
  dependency('json-c'),
]

gnome = import('gnome')

coco_sources += gnome.compile_resources('coco-resources',
  'coco.gresource.xml',
  c_name: 'coco'
)

executable('coco', coco_sources,
  dependencies: coco_deps,
  install: true,
)
