project('uploader', 'c',
          version: '0.0.1',
    meson_version: '>= 0.50.0',
  default_options: [ 'warning_level=2',
                     'c_std=gnu11',
                     'wrap_mode=default',
                   ],
)

add_project_arguments([
  '-I' + meson.build_root(),
], language: 'c')

subdir('src')
