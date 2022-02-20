coco_sources = [
  'main.c',
  'coco-window.c',
  'coco-application.c',
  'components/base64/base64.c',
  'components/clipboard/clipboard.c',
]

coco_deps = [
  dependency('gtk4'),
  dependency('libadwaita-1'),
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
